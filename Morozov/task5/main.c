//
// Created by vks on 19.05.16.
//


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int v = 7;
const int e = 9;
const int graph[][2] = {
        {0, 1},
        {0, 2},
        {1, 5},
        {2, 3},
        {2, 4},
        {3, 5},
        {4, 3},
        {4, 5},
        {5, 6},
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int *visited;
int *passed;

void *dfs(void *arg) {
    int ver = *((int *) arg);
    int i;
    for (i = 0; i < e; ++i) {
        if (graph[i][0] == ver) {
            int u = graph[i][1];
            pthread_mutex_lock(&mutex);
            if (passed[i]) {
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
            if (visited[u]) {
                printf("Thread %ld: creating edge #%i-#%i, but vertex #%i is already visited (not creating new thread).\n",
                       pthread_self(), ver, u, u);
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
            visited[u] = 1;
            passed[i] = 1;
            pthread_mutex_unlock(&mutex);
            printf("Thread %ld: creating edge #%i-#%i\n", pthread_self(), ver, u);
            pthread_t thr;
            pthread_create(&thr, NULL, dfs, &u);
            pthread_join(thr, NULL);
        }
    }
    pthread_exit(NULL);
}

int main() {
    visited = (int *) malloc(v * sizeof(int));
    passed = (int *) malloc(e * sizeof(int));
    memset(visited, 0, v * sizeof(int));
    int v0 = 0;
    pthread_t init;
    pthread_create(&init, NULL, dfs, &v0);
    pthread_join(init, NULL);
    return 0;
}

