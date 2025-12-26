#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * FIXES + VALIDATION ADDED:
 *  - FIX: BITSET now supports up to MAX_SYMBOLS (128) safely (no UB shifts).
 *  - VALIDATION: detect LL(1) conflicts while building parse table (fail fast).
 *  - VALIDATION: bounds checks for symbols/productions/input length.
 */

#define MAX_SYMBOLS      128
#define MAX_PRODUCTIONS  128
#define MAX_RHS_LEN      16
#define MAX_STRING_LEN   1024
#define MAX_STACK        1024
#define MAX_DERIV_STEPS  256

/* symbol types */
#define SYM_NONTERM  0
#define SYM_TERM     1
#define SYM_SPECIAL  2  /* eps and $ */

/* parse-table actions */
#define ACTION_ERR   -1
#define ACTION_POP   -2
#define ACTION_ACC   -3

typedef struct {
    int lhs;
    int rhs[MAX_RHS_LEN];
    int rhs_len;  /* 0 for eps */
} Production;

static char *symbolName[MAX_SYMBOLS];
static int   symbolType[MAX_SYMBOLS];
static int   symbolCount = 0;

static int startSymbol = -1;
static int epsSymbol   = -1;
static int dollarSymbol= -1;

static Production productions[MAX_PRODUCTIONS];
static int prodCount = 0;

/* --------- FIXED BITSET (supports MAX_SYMBOLS up to 128 safely) --------- */

#define BITWORDS ((MAX_SYMBOLS + 63) / 64)

typedef struct {
    uint64_t w[BITWORDS];
} BITSET;

static BITSET FIRSTset[MAX_SYMBOLS];
static BITSET FOLLOWset[MAX_SYMBOLS];

static int parseTable[MAX_SYMBOLS][MAX_SYMBOLS];

/* ---------- utilities ---------- */

static void die(const char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int addSymbol(const char *name, int type) {
    for (int i = 0; i < symbolCount; ++i) {
        if (strcmp(symbolName[i], name) == 0)
            return i;
    }
    if (symbolCount >= MAX_SYMBOLS) {
        die("ERROR: Too many symbols (MAX_SYMBOLS exceeded).");
    }
    symbolName[symbolCount] = (char*)malloc(strlen(name)+1);
    if (!symbolName[symbolCount]) die("ERROR: malloc failed in addSymbol().");
    strcpy(symbolName[symbolCount], name);
    symbolType[symbolCount] = type;
    return symbolCount++;
}

int getSymbol(const char *name) {
    for (int i = 0; i < symbolCount; ++i) {
        if (strcmp(symbolName[i], name) == 0)
            return i;
    }
    return -1;
}

int addProduction(int lhs, int *rhs, int rhs_len) {
    if (prodCount >= MAX_PRODUCTIONS) {
        die("ERROR: Too many productions (MAX_PRODUCTIONS exceeded).");
    }
    if (rhs_len > MAX_RHS_LEN) {
        die("ERROR: Production RHS too long (MAX_RHS_LEN exceeded).");
    }
    productions[prodCount].lhs = lhs;
    productions[prodCount].rhs_len = rhs_len;
    for (int i = 0; i < rhs_len; i++)
        productions[prodCount].rhs[i] = rhs[i];
    return ++prodCount; /* production numbers start at 1 */
}

/* bitset helpers */
void bitset_clear(BITSET *s) {
    for (int i = 0; i < BITWORDS; i++) s->w[i] = 0;
}

void bitset_add(BITSET *s, int elem) {
    s->w[elem >> 6] |= (1ULL << (elem & 63));
}

int bitset_has(const BITSET *s, int elem) {
    return (int)((s->w[elem >> 6] >> (elem & 63)) & 1ULL);
}

int bitset_equals(const BITSET *a, const BITSET *b) {
    return memcmp(a->w, b->w, sizeof(a->w)) == 0;
}

void bitset_copy(BITSET *dst, const BITSET *src) {
    memcpy(dst->w, src->w, sizeof(dst->w));
}

int bitset_or_into(BITSET *dst, const BITSET *src) {
    int changed = 0;
    for (int i = 0; i < BITWORDS; i++) {
        uint64_t before = dst->w[i];
        dst->w[i] |= src->w[i];
        if (dst->w[i] != before) changed = 1;
    }
    return changed;
}

/* print helpers */
void printSymbol(int s) { printf("%s", symbolName[s]); }

void printFirstFollowSets() {
    printf("FIRST sets:\n");
    for (int i = 0; i < symbolCount; ++i) {
        if (symbolType[i] == SYM_NONTERM) {
            printf("FIRST(%s) = { ", symbolName[i]);
            for (int t = 0; t < symbolCount; ++t) {
                if (symbolType[t] == SYM_TERM || t == epsSymbol) {
                    if (bitset_has(&FIRSTset[i], t)) {
                        printSymbol(t);
                        printf(" ");
                    }
                }
            }
            printf("}\n");
        }
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < symbolCount; ++i) {
        if (symbolType[i] == SYM_NONTERM) {
            printf("FOLLOW(%s) = { ", symbolName[i]);
            for (int t = 0; t < symbolCount; ++t) {
                if (symbolType[t] == SYM_TERM || t == dollarSymbol) {
                    if (bitset_has(&FOLLOWset[i], t)) {
                        printSymbol(t);
                        printf(" ");
                    }
                }
            }
            printf("}\n");
        }
    }
    printf("\n");
}

/* ---------- FIRST / FOLLOW ---------- */

/* FIRST of a string X1..Xn (Xi are symbol indices) */
BITSET FIRST_of_string(int *symbols, int len) {
    BITSET result;
    bitset_clear(&result);

    if (len == 0) {
        bitset_add(&result, epsSymbol);
        return result;
    }

    for (int i = 0; i < len; ++i) {
        int X = symbols[i];

        /* add FIRST(X) without eps */
        for (int t = 0; t < symbolCount; ++t) {
            if (t != epsSymbol && bitset_has(&FIRSTset[X], t))
                bitset_add(&result, t);
        }

        if (!bitset_has(&FIRSTset[X], epsSymbol)) {
            return result;
        }
    }

    bitset_add(&result, epsSymbol);
    return result;
}

/* FIRST computation */
void compute_FIRST() {
    for (int i = 0; i < symbolCount; ++i)
        bitset_clear(&FIRSTset[i]);

    /* FIRST(terminals) = the terminal, FIRST(eps) = eps */
    for (int i = 0; i < symbolCount; ++i) {
        if (symbolType[i] == SYM_TERM || i == epsSymbol)
            bitset_add(&FIRSTset[i], i);
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int p = 0; p < prodCount; ++p) {
            int A = productions[p].lhs;

            BITSET before;
            bitset_copy(&before, &FIRSTset[A]);

            int nullable_prefix = 1;

            if (productions[p].rhs_len == 0) {
                /* A -> eps */
                bitset_add(&FIRSTset[A], epsSymbol);
            } else {
                for (int i = 0; i < productions[p].rhs_len; ++i) {
                    int X = productions[p].rhs[i];

                    /* add FIRST(X) minus eps */
                    for (int t = 0; t < symbolCount; ++t) {
                        if (t != epsSymbol && bitset_has(&FIRSTset[X], t))
                            bitset_add(&FIRSTset[A], t);
                    }

                    if (!bitset_has(&FIRSTset[X], epsSymbol)) {
                        nullable_prefix = 0;
                        break;
                    }
                }
                if (nullable_prefix)
                    bitset_add(&FIRSTset[A], epsSymbol);
            }

            if (!bitset_equals(&FIRSTset[A], &before))
                changed = 1;
        }
    }
}

/* FOLLOW computation */
void compute_FOLLOW() {
    for (int i = 0; i < symbolCount; ++i)
        bitset_clear(&FOLLOWset[i]);

    /* add $ to FOLLOW(start) */
    bitset_add(&FOLLOWset[startSymbol], dollarSymbol);

    int changed = 1;
    while (changed) {
        changed = 0;

        for (int p = 0; p < prodCount; ++p) {
            int A = productions[p].lhs;
            int len = productions[p].rhs_len;

            for (int i = 0; i < len; ++i) {
                int B = productions[p].rhs[i];
                if (symbolType[B] != SYM_NONTERM) continue;

                /* compute FIRST(beta), beta = symbols after B */
                BITSET firstBeta;
                bitset_clear(&firstBeta);

                int nullable = 1;

                for (int j = i + 1; j < len; ++j) {
                    int X = productions[p].rhs[j];

                    /* FIRST(X) minus eps */
                    for (int t = 0; t < symbolCount; ++t) {
                        if (t != epsSymbol && bitset_has(&FIRSTset[X], t))
                            bitset_add(&firstBeta, t);
                    }

                    if (!bitset_has(&FIRSTset[X], epsSymbol)) {
                        nullable = 0;
                        break;
                    }
                }

                BITSET before;
                bitset_copy(&before, &FOLLOWset[B]);

                /* add FIRST(beta) to FOLLOW(B) */
                bitset_or_into(&FOLLOWset[B], &firstBeta);

                /* if beta nullable, add FOLLOW(A) to FOLLOW(B) */
                if (nullable) {
                    bitset_or_into(&FOLLOWset[B], &FOLLOWset[A]);
                }

                if (!bitset_equals(&FOLLOWset[B], &before))
                    changed = 1;
            }
        }
    }
}

static void set_table_entry_checked(int rowSym, int colTerm, int prodNumber) {
    int cur = parseTable[rowSym][colTerm];
    if (cur != ACTION_ERR && cur != prodNumber) {
        printf("LL(1) conflict at M[%s, %s]: %d vs %d\n",
               symbolName[rowSym], symbolName[colTerm], cur, prodNumber);
        exit(1);
    }
    parseTable[rowSym][colTerm] = prodNumber;
}

void build_LL1_table() {
    /* init all to error */
    for (int i = 0; i < symbolCount; ++i)
        for (int j = 0; j < symbolCount; ++j)
            parseTable[i][j] = ACTION_ERR;

    /* rule 1: productions */
    for (int p = 0; p < prodCount; ++p) {
        int A = productions[p].lhs;
        int *rhs = productions[p].rhs;
        int len = productions[p].rhs_len;

        BITSET f = FIRST_of_string(rhs, len);

        /* table entries for terminals in FIRST */
        for (int t = 0; t < symbolCount; ++t) {
            if (symbolType[t] == SYM_TERM && bitset_has(&f, t)) {
                set_table_entry_checked(A, t, p + 1);
            }
        }

        /* if eps in FIRST, use FOLLOW(A) */
        if (bitset_has(&f, epsSymbol)) {
            for (int b = 0; b < symbolCount; ++b) {
                if ((symbolType[b] == SYM_TERM || b == dollarSymbol) &&
                    bitset_has(&FOLLOWset[A], b)) {
                    set_table_entry_checked(A, b, p + 1);
                }
            }
        }
    }

    /* rule 2: terminals pop */
    for (int a = 0; a < symbolCount; ++a) {
        if (symbolType[a] == SYM_TERM) {
            parseTable[a][a] = ACTION_POP;
        }
    }

    /* rule 3: accept */
    parseTable[dollarSymbol][dollarSymbol] = ACTION_ACC;
}

void print_LL1_table() {
    printf("Parse table (rows: N U Σ U {$}, cols: Σ U {$})\n   ");
    for (int a = 0; a < symbolCount; ++a) {
        if (symbolType[a] == SYM_TERM || a == dollarSymbol) {
            printf("%8s", symbolName[a]);
        }
    }
    printf("\n");

    for (int A = 0; A < symbolCount; ++A) {
        if (symbolType[A] == SYM_NONTERM ||
            symbolType[A] == SYM_TERM ||
            A == dollarSymbol) {

            printf("%3s", symbolName[A]);

            for (int a = 0; a < symbolCount; ++a) {
                if (symbolType[a] == SYM_TERM || a == dollarSymbol) {
                    int val = parseTable[A][a];
                    if (val > 0) {
                        printf("%8d", val);
                    } else if (val == ACTION_POP) {
                        printf("%8s", "pop");
                    } else if (val == ACTION_ACC) {
                        printf("%8s", "acc");
                    } else {
                        printf("%8s", ".");
                    }
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

/* ---------- parse tree: father-sibling table ---------- */

typedef struct {
    int id;
    int sym;      /* symbol index */
    int parent;   /* node id of parent, -1 for root */
    int sibling;  /* right sibling node id, -1 if none */
} Node;

static Node nodes[MAX_STACK];
static int nodeCount = 0;

/* create new node */
int newNode(int sym, int parent, int sibling) {
    if (nodeCount >= MAX_STACK) {
        die("ERROR: Parse tree too large (nodeCount >= MAX_STACK).");
    }
    nodes[nodeCount].id = nodeCount;
    nodes[nodeCount].sym = sym;
    nodes[nodeCount].parent = parent;
    nodes[nodeCount].sibling = sibling;
    return nodeCount++;
}

void printParseTreeFatherSibling() {
    printf("index  symbol  father  sibling\n");
    for (int i = 0; i < nodeCount; ++i) {
        printf("%5d  %-6s  ", nodes[i].id, symbolName[nodes[i].sym]);

        if (nodes[i].parent == -1)
            printf("%6s  ", "-");
        else
            printf("%6d  ", nodes[i].parent);

        if (nodes[i].sibling == -1)
            printf("%7s\n", "-");
        else
            printf("%7d\n", nodes[i].sibling);
    }
    printf("\n");
}

/* ---------- Grammar printing (for OUTPUT a) ---------- */

void printGrammar_numbered() {
    for (int i = 0; i < prodCount; ++i) {
        printf("%d) ", i + 1);
        printSymbol(productions[i].lhs);
        printf(" -> ");
        if (productions[i].rhs_len == 0) {
            printSymbol(epsSymbol);
        } else {
            for (int j = 0; j < productions[i].rhs_len; ++j) {
                printSymbol(productions[i].rhs[j]);
                if (j + 1 < productions[i].rhs_len) printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int parse_LL1(int *inputSymbols, int len,
              int buildTree,
              int showDeriv,
              int printProdSeq,
              int prodSeqOut[MAX_PRODUCTIONS],
              int *prodSeqLen,
              int verbose)
{
    (void)printProdSeq; /* kept for compatibility */

    int alpha[MAX_STACK];  /* input stack */
    int beta[MAX_STACK];   /* working stack: symbols */
    int nodeStack[MAX_STACK]; /* parallel with beta when buildTree */
    int topAlpha = -1, topBeta = -1;

    if (len + 1 >= MAX_STACK) {
        die("ERROR: Input too long for stacks (MAX_STACK exceeded).");
    }

    alpha[++topAlpha] = dollarSymbol;
    for (int i = len-1; i >= 0; --i)
        alpha[++topAlpha] = inputSymbols[i];

    beta[++topBeta] = dollarSymbol;
    int rootNode = -1;
    if (buildTree) {
        rootNode = newNode(startSymbol, -1, -1);
        beta[++topBeta] = startSymbol;
        nodeStack[topBeta] = rootNode;
    } else {
        beta[++topBeta] = startSymbol;
    }

    /* derivation: sentential form */
    int derivation[MAX_DERIV_STEPS][MAX_STACK];
    int derivLen[MAX_DERIV_STEPS];
    int derivSteps = 0;
    if (showDeriv) {
        derivLen[0] = 1;
        derivation[0][0] = startSymbol;
        derivSteps = 1;
    }

    int prodSeqIdx = 0;
    int go = 1;
    int accepted = 0;

    while (go) {
        int a = alpha[topAlpha];  /* head(alpha) */
        int X = beta[topBeta];    /* head(beta) */

        int action = parseTable[X][a];

        if (action > 0) {
            /* PUSH move: use production 'action' */
            int pIndex = action - 1;
            Production *p = &productions[pIndex];
            int A = p->lhs;

            /* pop A */
            int A_node = -1;
            if (buildTree) {
                A_node = nodeStack[topBeta];
            }
            topBeta--;

            /* record production sequence (always) */
            if (prodSeqIdx >= MAX_PRODUCTIONS) {
                die("ERROR: Production sequence too long (MAX_PRODUCTIONS exceeded).");
            }
            prodSeqOut[prodSeqIdx++] = action;

            /* derivation: leftmost; replace first A */
            if (showDeriv) {
                if (derivSteps >= MAX_DERIV_STEPS) {
                    die("ERROR: Too many derivation steps (MAX_DERIV_STEPS exceeded).");
                }
                int last = derivSteps - 1;
                int newForm[MAX_STACK];
                int newLen = 0;
                int replaced = 0;
                for (int i = 0; i < derivLen[last]; ++i) {
                    int sym = derivation[last][i];
                    if (!replaced && sym == A) {
                        if (p->rhs_len == 0) {
                            /* eps: nothing */
                        } else {
                            for (int j = 0; j < p->rhs_len; ++j)
                                newForm[newLen++] = p->rhs[j];
                        }
                        replaced = 1;
                    } else {
                        newForm[newLen++] = sym;
                    }
                }
                ++derivSteps;
                derivLen[derivSteps-1] = newLen;
                for (int i = 0; i < newLen; ++i)
                    derivation[derivSteps-1][i] = newForm[i];
            }

            /* tree: create children nodes for RHS and push them */
            if (buildTree) {
                int childIds[MAX_RHS_LEN];
                int childCount = 0;

                if (p->rhs_len == 0) {
                    /* epsilon: create node but do NOT push on beta */
                    int epsNode = newNode(epsSymbol, A_node, -1);
                    childIds[childCount++] = epsNode;
                } else {
                    for (int i = 0; i < p->rhs_len; ++i) {
                        int sym = p->rhs[i];
                        int node = newNode(sym, A_node, -1);
                        childIds[childCount++] = node;
                    }
                }
                /* set siblings */
                for (int i = 0; i < childCount-1; ++i) {
                    nodes[childIds[i]].sibling = childIds[i+1];
                }

                /* push children on beta in reverse order, except eps */
                if (p->rhs_len > 0) {
                    for (int i = childCount-1; i >= 0; --i) {
                        int sym = nodes[childIds[i]].sym;
                        beta[++topBeta] = sym;
                        nodeStack[topBeta] = childIds[i];
                    }
                }
            } else {
                /* push RHS symbols on beta in reverse order */
                if (p->rhs_len > 0) {
                    for (int i = p->rhs_len-1; i >= 0; --i)
                        beta[++topBeta] = p->rhs[i];
                }
            }

        } else if (action == ACTION_POP) {
            /* POP move: terminals match */
            topAlpha--;
            topBeta--;
        } else if (action == ACTION_ACC) {
            go = 0;
            accepted = 1;
        } else {
            go = 0;
            accepted = 0;
            printf("Syntax error: unexpected symbol '%s'\n", symbolName[a]);
        }
    }

    if (prodSeqLen) *prodSeqLen = prodSeqIdx;

    if (accepted && verbose) {
        printf("OUTPUT:\n");

        /* a) production rules (full grammar, numbered) */
        printf("a) production rules\n");
        printGrammar_numbered();

        /* b) derivation string in one line, with ⇒(k) */
        if (showDeriv) {
            printf("b) derivation string\n");
            /* start symbol (step 0) */
            printSymbol(derivation[0][0]);  /* always just startSymbol */

            for (int step = 1; step < derivSteps; ++step) {
                int prodIdx = prodSeqOut[step-1]; /* production used */
                printf(" =>(%d) ", prodIdx);
                for (int i = 0; i < derivLen[step]; ++i) {
                    printSymbol(derivation[step][i]);
                    if (i + 1 < derivLen[step]) printf(" ");
                }
            }
            printf("\n\n");
        }

        /* c) father/sibling table */
        if (buildTree) {
            printf("c) table – father / sibling:\n");
            printParseTreeFatherSibling();
        }
    } else if (!accepted) {
        printf("Sequence NOT accepted.\n");
    }

    return accepted;
}

/* ---------- Build grammars ---------- */

/* Seminar grammar (Requirement 1) */
void build_grammar_seminar() {
    symbolCount = 0;
    prodCount = 0;
    nodeCount = 0;

    epsSymbol    = addSymbol("eps", SYM_SPECIAL);
    dollarSymbol = addSymbol("$",   SYM_SPECIAL);

    int S = addSymbol("S", SYM_NONTERM);
    int B = addSymbol("B", SYM_NONTERM);
    int C = addSymbol("C", SYM_NONTERM);

    int a = addSymbol("a", SYM_TERM);
    int b = addSymbol("b", SYM_TERM);
    int c = addSymbol("c", SYM_TERM);
    int d = addSymbol("d", SYM_TERM);

    startSymbol = S;

    int rhs[3];

    /* 1: S -> B b */
    rhs[0] = B; rhs[1] = b;
    addProduction(S, rhs, 2);

    /* 2: S -> C d */
    rhs[0] = C; rhs[1] = d;
    addProduction(S, rhs, 2);

    /* 3: B -> a B */
    rhs[0] = a; rhs[1] = B;
    addProduction(B, rhs, 2);

    /* 4: B -> eps */
    addProduction(B, NULL, 0);

    /* 5: C -> c C */
    rhs[0] = c; rhs[1] = C;
    addProduction(C, rhs, 2);

    /* 6: C -> eps */
    addProduction(C, NULL, 0);

    compute_FIRST();
    compute_FOLLOW();
    build_LL1_table();
}


/* Mini-DSL grammar (Requirement 2) */
void build_grammar_minidsl() {
    symbolCount = 0;
    prodCount = 0;
    nodeCount = 0;

    epsSymbol    = addSymbol("eps", SYM_SPECIAL);
    dollarSymbol = addSymbol("$",   SYM_SPECIAL);

    /* nonterminals */
    int PROGRAM   = addSymbol("PROGRAM",   SYM_NONTERM);
    int STMT_LIST = addSymbol("STMT_LIST", SYM_NONTERM);
    int STMT      = addSymbol("STMT",      SYM_NONTERM);
    int COND      = addSymbol("COND",      SYM_NONTERM);
    int COMP      = addSymbol("COMP",      SYM_NONTERM);

    /* terminals */
    int LOAD       = addSymbol("LOAD", SYM_TERM);
    int SAVE       = addSymbol("SAVE", SYM_TERM);
    int REPLACE    = addSymbol("REPLACE", SYM_TERM); /* unused but declared */
    int PRINT      = addSymbol("PRINT", SYM_TERM);
    int DELETE_T   = addSymbol("DELETE", SYM_TERM);
    int COUNT      = addSymbol("COUNT", SYM_TERM);   /* unused */
    int APPEND     = addSymbol("APPEND", SYM_TERM);
    int IF_T       = addSymbol("IF", SYM_TERM);
    int THEN_T     = addSymbol("THEN", SYM_TERM);
    int ENDIF_T    = addSymbol("ENDIF", SYM_TERM);
    int INTO       = addSymbol("INTO", SYM_TERM);
    int TO         = addSymbol("TO", SYM_TERM);
    int EQUALS     = addSymbol("EQUALS", SYM_TERM);
    int CONTAINS   = addSymbol("CONTAINS", SYM_TERM);
    int STARTSWITH = addSymbol("STARTSWITH", SYM_TERM);
    int ENDSWITH   = addSymbol("ENDSWITH", SYM_TERM);
    int ID         = addSymbol("ID", SYM_TERM);
    int STRING     = addSymbol("STRING", SYM_TERM);

    (void)REPLACE; (void)COUNT;

    startSymbol = PROGRAM;

    int rhs[MAX_RHS_LEN];

    /* 1: PROGRAM -> STMT_LIST */
    rhs[0] = STMT_LIST;
    addProduction(PROGRAM, rhs, 1);

    /* 2: STMT_LIST -> STMT STMT_LIST */
    rhs[0] = STMT; rhs[1] = STMT_LIST;
    addProduction(STMT_LIST, rhs, 2);

    /* 3: STMT_LIST -> eps */
    addProduction(STMT_LIST, NULL, 0);

    /* 4: STMT -> LOAD STRING INTO ID */
    rhs[0] = LOAD; rhs[1] = STRING; rhs[2] = INTO; rhs[3] = ID;
    addProduction(STMT, rhs, 4);

    /* 5: STMT -> SAVE ID TO STRING */
    rhs[0] = SAVE; rhs[1] = ID; rhs[2] = TO; rhs[3] = STRING;
    addProduction(STMT, rhs, 4);

    /* 6: STMT -> PRINT ID */
    rhs[0] = PRINT; rhs[1] = ID;
    addProduction(STMT, rhs, 2);

    /* 7: STMT -> DELETE ID */
    rhs[0] = DELETE_T; rhs[1] = ID;
    addProduction(STMT, rhs, 2);

    /* 8: STMT -> APPEND STRING TO ID */
    rhs[0] = APPEND; rhs[1] = STRING; rhs[2] = TO; rhs[3] = ID;
    addProduction(STMT, rhs, 4);

    /* 9: STMT -> IF COND THEN STMT_LIST ENDIF */
    rhs[0] = IF_T; rhs[1] = COND; rhs[2] = THEN_T; rhs[3] = STMT_LIST; rhs[4] = ENDIF_T;
    addProduction(STMT, rhs, 5);

    /* 10: COND -> ID COMP STRING */
    rhs[0] = ID; rhs[1] = COMP; rhs[2] = STRING;
    addProduction(COND, rhs, 3);

    /* 11-14: COMP */
    rhs[0] = EQUALS;     addProduction(COMP, rhs, 1);
    rhs[0] = CONTAINS;   addProduction(COMP, rhs, 1);
    rhs[0] = STARTSWITH; addProduction(COMP, rhs, 1);
    rhs[0] = ENDSWITH;   addProduction(COMP, rhs, 1);

    compute_FIRST();
    compute_FOLLOW();
    build_LL1_table(); /* includes LL(1) conflict validation */
}

/* ---------- Input helpers ---------- */

int read_seminar_input(int *buffer) {
    char line[MAX_STRING_LEN];
    printf("Enter sequence over {a,b,c,d}: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;

    int len = 0;
    for (int i = 0; line[i] && line[i] != '\n'; ++i) {
        char ch = line[i];
        if (ch=='a' || ch=='b' || ch=='c' || ch=='d') {
            char s[2] = {ch,0};
            int id = getSymbol(s);
            if (id < 0) {
                printf("Unknown terminal '%c'\n", ch);
                return 0;
            }
            if (len >= MAX_STACK) {
                printf("ERROR: Input too long (MAX_STACK exceeded).\n");
                return 0;
            }
            buffer[len++] = id;
        } else if (ch==' ') {
            continue;
        } else {
            printf("Invalid character '%c' in input.\n", ch);
            return 0;
        }
    }
    return len;
}

/* read mini-DSL tokens from PIF.txt (format: TOKEN_NAME ST_INDEX) */
int read_minidsl_input_from_pif(int *buffer) {
    FILE *f = fopen("PIF.txt", "r");
    if (!f) {
        printf("ERROR: Could not open PIF.txt\n");
        return 0;
    }

    char token[256];
    char stIndex[256];
    int len = 0;

    while (fscanf(f, "%255s %255s", token, stIndex) == 2) {
        int id = getSymbol(token);
        if (id < 0) {
            printf("ERROR: Unknown token '%s' in PIF.txt\n", token);
            fclose(f);
            return 0;
        }

        if (symbolType[id] != SYM_TERM) {
            printf("ERROR: '%s' in PIF.txt is not a terminal in the miniDSL grammar.\n", token);
            fclose(f);
            return 0;
        }

        if (len >= MAX_STACK) {
            printf("ERROR: Input too long (MAX_STACK exceeded).\n");
            fclose(f);
            return 0;
        }

        buffer[len++] = id;
    }

    fclose(f);
    printf("Loaded %d terminals from PIF.txt\n", len);
    return len;
}

int main(void) {

    build_grammar_seminar();

    int input1[MAX_STACK];
    int len1 = read_seminar_input(input1);
    if (len1 > 0) {
        int prodSeq1[MAX_PRODUCTIONS];
        int prodLen1 = 0;

        int ok = parse_LL1(input1, len1,
                           0,  /* buildTree */
                           0,  /* showDeriv */
                           0,  /* printProdSeq inside */
                           prodSeq1,
                           &prodLen1,
                           0); /* verbose */

        if (ok) {
            printf("Production sequence:\n");
            for (int i = 0; i < prodLen1; ++i)
                printf("%d ", prodSeq1[i]);
            printf("\n");
        }
    }

    build_grammar_minidsl();

    printf("\nFIRST / FOLLOW sets:\n");
    printFirstFollowSets();

    printf("Parsed Table:\n");
    print_LL1_table();

    int input2[MAX_STACK];
    int len2 = read_minidsl_input_from_pif(input2);
    if (len2 > 0) {
        int prodSeq2[MAX_PRODUCTIONS];
        int prodLen2 = 0;

        parse_LL1(input2, len2,
                  1,  /* buildTree */
                  1,  /* showDeriv */
                  1,  /* printProdSeq */
                  prodSeq2,
                  &prodLen2,
                  1); /* verbose */
    }

    return 0;
}

