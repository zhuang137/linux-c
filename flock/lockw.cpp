#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main() {
    int fd, ret;
    struct flock _lock;
    fd = open("xx.txt", O_RDWR|O_CREAT, 0755);
    if (fd == -1) {
        printf("xx.txt open failed\n");
        return 0;
    }

    _lock.l_type = F_RDLCK;
    _lock.l_whence = SEEK_SET;
    _lock.l_start = 0;
    _lock.l_len = 0;
    _lock.l_pid = 100;
    if ((ret=fcntl(fd, F_GETLK, &_lock)) == 0) {
        if (_lock.l_type == F_UNLCK) {
            printf("the file is not locked for read\n");
        }
    } else {
        printf("the file is locked for read\n");
        return 0;
    }

    _lock.l_type = F_WRLCK;
    _lock.l_whence = SEEK_SET;
    _lock.l_start = 0;
    _lock.l_len = 0;
    _lock.l_pid = 100;

    if ((ret=fcntl(fd, F_SETLK, &_lock)) == 0) {
        printf("file set write lock success\n");
    } else {
        printf("file set write lock failed\n");
        return 0;
    }

    //test if we can exec lssek operation
    errno = 0;
    off_t y = lseek(fd, 1, SEEK_SET);
    if (y == -1) {
        printf("main process lseek failed for %s\n", strerror(errno));
    }

    pid_t pid = fork();

    if (pid == 0) {
        printf("run in child process\n");
        int fd1 = open("xx.txt", O_RDWR);
        if (fd1 == -1) {
            printf("child open xx.txt failed\n");
            exit(1);
        }
        struct flock _lockC;
        _lockC.l_type = F_WRLCK;
        _lockC.l_whence = SEEK_SET;
        _lockC.l_start = 0;
        _lockC.l_len = 5;
        if (fcntl(fd1, F_SETLK, &_lockC) == 0) {
            printf("child setlk write success\n");
        }
        errno = 0;
        off_t xx = lseek(fd, 2, SEEK_SET);
        if (xx == -1) {
            printf("child lseek failed for %s\n", strerror(errno));
             close(fd);
             exit(2);
        }
        close(fd);
        exit(0);

    } else {
        printf("wait child process exit\n");
        int status = 0;
        waitpid(pid, &status, 1);
        if (WIFEXITED(status)) {
            printf("child exit status: %d\n", WEXITSTATUS(status));
        }
    }

    for (int i=0; i < 20; i++) {
        sleep(1);
    }

    close(fd);
    return 0;
}
