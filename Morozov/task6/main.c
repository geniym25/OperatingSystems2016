//
// Created by vks on 19.05.16.
//

#include <zconf.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int flag;
int exit_;
const int w = 4;
const int h = 2;
int *mtr1;
int *mtr2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void print_matrix(int *matrix) {
    int i;
    for (i = 0; i < w * h; ++i) {
        if (i % w == 0 && i != 0) {
            printf("\n");
        }
        printf("%i  ", matrix[i]);
    }
    printf("\n");
}

void *generate(void *param) {
    pthread_mutex_lock(&mutex);
    if (flag == -1) {
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    if (flag == 0) {
        int i;
        for (i = 0; i < w * h; ++i) {
            mtr1[i] = (rand() % 100) - 50;
            mtr2[i] = (rand() % 100) - 50;
        }
        printf("Generated two %ix%i matrices:\n", w, h);
        print_matrix(mtr1);
        printf("\n\n");
        print_matrix(mtr2);
        flag = 1;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *add(void *param) {
    pthread_mutex_lock(&mutex);
    if (flag == -1) {
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    if (flag == 1) {
        int i;
        for (i = 0; i < w * h; ++i) {
            mtr1[i] += mtr2[i];
        }
        printf("Matrices summation completed.\n");
        flag = 2;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *print(void *param) {
    pthread_mutex_lock(&mutex);
    if (flag == -1) {
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    if (flag == 2) {
        printf("Result: \n");
        print_matrix(mtr1);
        printf("\n---------------------------------------------------------------\n");
        flag = 0;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *listen(void *param) {
    while (1) {
        char s;
        scanf("%c", &s);
        if (s == 'x') {
            exit_ = 1;
            printf("*** Received shutdown signal.\n");
            pthread_exit(NULL);
        }
    }
}

int main() {
    exit_ = 0;
    mtr1 = (int *) malloc(w * h * sizeof(int));
    mtr2 = (int *) malloc(w * h * sizeof(int));
    pthread_t thr0;
    pthread_t thr1;
    pthread_t thr2;
    pthread_t resident;
    pthread_create(&resident, NULL, listen, NULL);
    while (!exit_) {
        pthread_create(&thr0, NULL, generate, NULL);
        pthread_create(&thr1, NULL, add, NULL);
        pthread_create(&thr2, NULL, print, NULL);
        pthread_join(thr0, NULL);
        pthread_join(thr1, NULL);
        pthread_join(thr2, NULL);
        sleep(2);
    }
    pthread_join(resident, NULL);
    return 0;
}