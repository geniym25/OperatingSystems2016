#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <zconf.h>
#include <time.h>
#include <stdlib.h>

#define n 228


int execute(int *array) {
    pid_t k = fork();
    if (k == -1) {
        fprintf(stderr, "Yo! U don't have enough memory, man!\n");
    } else if (k == 0) {
        printf("Process %i is generating\n", getpid());
        int a = 0;
        while (array[2]) {
            if (array[3]) {
                array[0] = a;
                array[1] = ++a;
                a++;
                array[3] = 0;
                array[4] = 1;
            }
        }
        printf("Stopped");
        return 0;
    }

    pid_t b = fork();
    if (b == -1) {
        fprintf(stderr, "Yo! U don't have enough memory, man!\n");
    } else if (b == 0) {
        printf("Process %i is calculating\n", getpid());
        while (array[2]) {
            if (array[4]) {
                array[6] = array[1] + array[0];
                array[4] = 0;
                array[5] = 1;
            }
        }

        printf("Stopped");
        return 0;
    }

    pid_t c = fork();
    if (c == -1) {
        fprintf(stderr, "Yo! U don't have enough memory, man!\n");
    } else if (c == 0) {
        printf("Process %i is printing\n", getpid());
        while (array[2]) {
            if (array[5]) {
                printf("Result is %i\n", array[6]);
                array[5] = 1;
                array[3] = 0;
            }

        }
        printf("Stopped");
        return 0;
    }

    int stop = getchar();
    if (stop == 115) {
        printf("Stopped");
        array[2 * n] = 0;
        wait(k);
        wait(b);
        wait(c);
        shmdt(array);
        return 0;
    }
}

int main() {
    int memery = shmget(0, 7 * sizeof(int), IPC_CREAT | 0666);
    if (memery == -1) {
        fprintf(stderr, "Your computer is old, man! Or you're using python. Either way, it's bad!");
        return 1;
    }
    int *array = (int *) shmat(memery, 0, 0);
    memset(array, 0, 7 * sizeof(int));
    array[2] = 1;
    array[3] = 1;
    array[4] = 0;
    array[5] = 0;
    execute(array);
    return 0;
}
