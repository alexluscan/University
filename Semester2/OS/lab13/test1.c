#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
        int* l;
        int capL;
        int* u;
        int capU;
        int s;
        int n;
}data;

pthread_t* t;
pthread_barrier_t* b;
pthread_mutex_t* m;

int first(int n){
        while(n > 9){
                n/=10;
        }
        return n;
}

void* f(void* a){
        data* dt = (data*)a;
        pthread_barrier_wait(b);
        if (first(dt->n) % 2 == 0){
                printf("Appending %d to U\n" , dt->n);
                dt->u[dt->capU] = dt->n;
                pthread_mutex_lock(m);
                dt->capU++;
                pthread_mutex_unlock(m);
        }
        else{
                printf("Appending %d to L\n" , dt->n);
                dt->l[dt->capL] = dt->n;
                pthread_mutex_lock(m);
                dt->capL++;
                pthread_mutex_unlock(m);
        }
        dt->s += dt->n;
        return NULL;
}

int main(int argc , char** argv){
        int n;
        scanf("%d" , &n);
        int* l;
        int* u;
        int s;
        int capU;
        int capL;
        u = malloc(sizeof(int)* n);
        l = malloc(sizeof(int)* n);
        t = malloc(sizeof(pthread_t) * n);
        b = malloc(sizeof(pthread_barrier_t));
        s = 0;
        m = malloc(sizeof(pthread_mutex_t));
        capL = 0;
        capU = 0;
        pthread_barrier_init(b , NULL , n);
        data* dt = malloc(sizeof(data) * n);
        pthread_mutex_init(m,NULL);
        for (int i = 0 ; i < n ; i++){

                dt[i].u = u;
                dt[i].l = l;
                dt[i].s = s;
                dt[i].capU = capU;
                dt[i].capL = capL;
                int x;
                scanf("%d" , &x);
                dt[i].n = x;
                pthread_create(&t[i] , NULL , f , &dt[i]);
        }

        for(int i = 0 ; i < n ; i++){
                pthread_join(t[i] , NULL);
        }

        printf("Vector U: ");
        for (int i = 0 ; i < dt->capU ; i++){
                printf("%d " , u[i]);
        }

        printf("\nVector L: ");
        for(int i = 0 ; i < dt->capL ; i++){
                printf("%d " , l[i]);
        }

        printf("\nSum: %d"  , s);
        pt
