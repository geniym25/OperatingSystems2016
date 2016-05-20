#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

void prt_mtr(int *m, int rows, int cols) {
    int i;
    for (i = 0; i < rows * cols; ++i) {
        if (i % rows == 0) {
            printf("\n");
        }
        printf("%i  ", m[i]);
    }
    printf("\n\n");
}

int main() {
    int pipes[2];
    if (pipe(pipes) == -1) {
        fprintf(stderr, "Error while creating pipes.\n");
        return 1;
    }
    int rows = 4;
    int cols = 2;
    int *mtr1;
    int *mtr2;
    char *cmd;
    int shmem1 = shmget(0, rows * cols * sizeof(int), IPC_CREAT | 0666);
    int shmem2 = shmget(0, rows * cols * sizeof(int), IPC_CREAT | 0666);
    int shmem3 = shmget(0, sizeof(char), IPC_CREAT | 0666);
    if (shmem1 == -1 || shmem2 == -1 || shmem3 == -1) {
        fprintf(stderr, "Error while allocating shared memory!\n");
        return 1;
    }
    mtr1 = (int *) shmat(shmem1, 0, 0);
    mtr2 = (int *) shmat(shmem2, 0, 0);
    cmd = (char *) shmat(shmem3, 0, 0);
    if (mtr1 == (int *) -1 || mtr2 == (int *) -1 || cmd == (char *) -1) {
        fprintf(stderr, "Error while assigning shared memory!\n");
        return 1;
    }
    pid_t crt = -1;
    pid_t sum = -1;
    pid_t prt = -1;
    crt = fork();
    if (crt > 0) {
        sum = fork();
    }
    if (crt > 0 && sum > 0) {
        prt = fork();
    }
    cmd[0] = 'c';
    while (1) {
        if (crt > 0 && sum > 0 && prt > 0) {
            char op;
            scanf("%c", &op);
            if (op == 'x') {
                cmd[0] = 'x';
                printf("Parent process (pid %i): received shutdown command.\n", getpid());
                return 0;
            }
        }
        if (crt == 0) {
            if (cmd[0] == 'c') {
                printf("pid %i: creating two %ix%i matrices.\n", getpid(), rows, cols);
                int i;
                for (i = 0; i < rows * cols; ++i) {
                    mtr1[i] = (int) (random() % 100 - 50);
                    mtr2[i] = (int) (random() % 100 - 50);
                }
                prt_mtr(mtr1, rows, cols);
                prt_mtr(mtr2, rows, cols);
                if (cmd[0] != 'x') {
                    cmd[0] = 's';
                }
            } else if (cmd[0] == 'x') {
                return 0;
            }
        }
        if (sum == 0) {
            if (cmd[0] == 's') {
                printf("pid %i: evaluating summation.\n", getpid());
                int i;
                for (i = 0; i < rows * cols; ++i) {
                    mtr1[i] += mtr2[i];
                }
                if (cmd[0] != 'x') {
                    cmd[0] = 'p';
                }
            } else if (cmd[0] == 'x') {
                return 0;
            }
        }
        if (prt == 0) {
            if (cmd[0] == 'p') {
                printf("pid %i: printing result.\n", getpid());
                prt_mtr(mtr1, rows, cols);
                if (cmd[0] != 'x') {
                    cmd[0] = 'c';
                    sleep(3);
                }
            } else if (cmd[0] == 'x') {
                return 0;
            }
        }
    }
}

