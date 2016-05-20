#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>

#define PERMS 0666

char *parse_dir_name(char *path) {
    size_t len = strlen(path);
    while (len != 0 && path[--len] != '/') { }
    char *ans = (char *) malloc(len + 1);
    for (int i = 0; i < len; ++i) {
        ans[i] = path[i];
    }
    ans[len] = '\0';
    return ans;
}

char *substr(char *src, size_t len) {
    char *ans = (char *) malloc(len + 1);
    for (int i = 0; i < len; ++i) {
        ans[i] = src[i];
    }
    ans[len] = 0;
    return ans;
}

void create_dir_sequence(char *path) {
    size_t len = strlen(path);
    size_t ptr = 0;
    while (ptr < len) {
        while (ptr < len && path[++ptr] != '/') { }
        mkdir(substr(path, ptr), 0777);
    }
}

int main(int argc, char **argv) {
    int f_flag = 0;
    if (argc == 1) {
        fprintf(stderr, "Error: missing arguments.\n");
        return 1;
    }
    if (argc == 2) {
        fprintf(stderr, "Error: missing destination argument.\n");
        return 1;
    }
    for (int i = 3; i < argc; ++i) {
        if (strcmp(argv[i], "-f") == 0) {
            f_flag = 1;
        }
    }
    char *src = argv[1];
    char *dst = argv[2];
    char *dir = parse_dir_name(dst);
    create_dir_sequence(dir);
    int src_d = open(src, O_RDONLY);
    if (src_d == -1) {
        fprintf(stderr, "Error while opening source file.\n");
        return 1;
    }
    if ((access(dst, F_OK) == 0) && (f_flag == 0)) {
        char ans = '\0';
        fprintf(stderr, "File already exists. Rewrite? (y/n)\n");
        while (ans != 'y' && ans != 'n') {
            scanf("%c", &ans);
            if (ans == 'y') {
                break;
            }
            if (ans == 'n') {
                return 0;
            }
            if (ans == 13 || ans == 10) {
                continue;
            }
            fprintf(stderr, "Sorry?");
        }
    }
    int dst_d = creat(dst, PERMS);
    if (dst_d == -1) {
        fprintf(stderr, "Error while creating destination file.\n");
        return 1;
    }
    char buf[BUFSIZ];
    long count = 0;
    while ((count = read(src_d, buf, BUFSIZ)) > 0) {
        if (write(dst_d, buf, (size_t) count) < count) {
            fprintf(stderr, "Error while copying file.\n");
            return 1;
        }
    }
    return 0;
}