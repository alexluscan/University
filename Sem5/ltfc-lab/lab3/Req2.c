#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BSTNode {
    char* symbol;
    int id;
    struct BSTNode *left, *right;
} BSTNode;

BSTNode* bstRoot = NULL;
int current_id = 0;

// Create a new BST node
BSTNode* createBSTNode(const char* symbol, int id) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->symbol = strdup(symbol);
    node->id = id;
    node->left = node->right = NULL;
    return node;
}

// Insert into BST
BSTNode* insertBST(BSTNode* root, const char* symbol, int id) {
    if (!root) return createBSTNode(symbol, id);
    int cmp = strcmp(symbol, root->symbol);
    if (cmp < 0)
        root->left = insertBST(root->left, symbol, id);
    else if (cmp > 0)
        root->right = insertBST(root->right, symbol, id);
    return root;
}

// Search BST
BSTNode* searchBSTNode(BSTNode* root, const char* symbol) {
    if (!root) return NULL;
    int cmp = strcmp(symbol, root->symbol);
    if (cmp == 0) return root;
    if (cmp < 0) return searchBSTNode(root->left, symbol);
    return searchBSTNode(root->right, symbol);
}

// Print BST inorder
void inorderBSTPrint(FILE* file, BSTNode* root) {
    if (!root) return;
    inorderBSTPrint(file, root->left);
    fprintf(file, "\"%s\" -> %d\n", root->symbol, root->id);
    printf("\"%s\" -> %d\n", root->symbol, root->id);
    inorderBSTPrint(file, root->right);
}

// Add symbol to BST if not exists
void handleSymbol(const char* symbol) {
    if (!searchBSTNode(bstRoot, symbol)) {
        bstRoot = insertBST(bstRoot, symbol, current_id);
        current_id++;
    }
}

int main() {
    char buf[256];

    printf("Reading scanner output (from stdin)...\n");
    printf("Storing identifiers and numbers in symbol table...\n\n");

    while (fgets(buf, sizeof(buf), stdin)) {
        // Only lines containing ID or NUMBER
        if (strstr(buf, "TOKEN: ID <") || strstr(buf, "TOKEN: NUMBER <")) {
            char *start = strchr(buf, '<');
            char *end = strchr(buf, '>');
            if (start && end && end > start + 1) {
                char symbol[256];
                size_t len = end - start - 1;
                strncpy(symbol, start + 1, len);
                symbol[len] = '\0';
                handleSymbol(symbol);
            }
        }
    }

    // Save symbol table to file
    FILE* stFile = fopen("st.txt", "w");
    if (!stFile) {
        perror("Could not open st.txt for writing");
        return 1;
    }

    inorderBSTPrint(stFile, bstRoot);
    fclose(stFile);

    printf("\nSymbol table saved in st.txt\n");
    return 0;
}
