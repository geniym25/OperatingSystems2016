#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int graph[][2] = {{0, 1},
                  {0, 2},
                  {0, 3},
                  {1, 3},
                  {1, 2},
                  {2, 4},
                  {3, 2},
                  {4, 5},
                  {4, 6},
                  {5, 7},
                  {5, 8},
                  {6, 9},
                  {7, 8},
                  {8, 9}
};

void dfs(int ver, int *visited) {
    visited[ver] = 1;
    for (int i = 0; i < 14; ++i) {
        if ((graph[i][0] == ver) && (visited[graph[i][1]] == 0)) {
            visited[graph[i][1]] = 1;
            pid_t pid = fork();
            if (pid > 0) {
                printf("Parent process: pid %i, creating process pid %i, edge %i-%i \n",
                       getpid(), pid, ver, graph[i][1]);
            } else {
                dfs(graph[i][1], visited);
            }
        }
    }
    int status;
    wait(&status);
}

int main() {
    printf("Main process: pid %i\n", getpid());
    int shared_mem_id;
    int *visited;
    if ((shared_mem_id = shmget(1, 6 * sizeof(int), IPC_CREAT | 0666)) < 0) {
        fprintf(stderr, "Error while allocating shared memory.");
        return 1;
    }
    if ((visited = (int *) shmat(shared_mem_id, NULL, 0)) == (int *) -1) {
        fprintf(stderr, "Error while attaching shared memory.");
        return 1;
    }
    memset(visited, 0, 10 * sizeof(int));
    dfs(0, visited);
    return 0;
}
