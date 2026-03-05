// 221A LAB12
// Artur Kovalenko
// ka57748@zut.edu.pl

#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

void l(char *s, int n) {
    while(*s) write(1, s++, 1);
    if(n == -999) return;
    char b[12]; int i=0;
    do { b[i++] = n%10+'0'; } while((n/=10)>0);
    while(i--) write(1, &b[i], 1);
    write(1, "\n", 1);
}

int main(int c, char **v) {
    if(c!=3) return 1;

    int id = shmget(atoi(v[1]), 101, 0600);
    char *m = shmat(id, 0, 0);
    int fd = open(v[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);

    if(id==-1||m==(void*)-1||fd==-1) return 1;

    l("shared memory attached, ready to receive\n", -999);
    *m = 0;
    while(1) {
        while(*m == 0);
        char s = *m;
        if(s == -1) break;
        if(s > 0) {
            write(fd, (void*)(m+1), s);
            l(" byte(s) received...", s);
            *m = 0;
        }
    }

    l("copying finished\n", -999);
    close(fd);
    *m = -2;
    shmdt((void*)m);
    l("shared memory detached\n", -999);
    return 0;
}
