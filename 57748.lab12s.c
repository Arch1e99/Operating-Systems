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
    if(n<0) { write(1,"-",1); n=-n; }
    do { b[i++] = n%10+'0'; } while((n/=10)>0);	
    while(i--) write(1, &b[i], 1);
    write(1, "\n", 1);
}

int main(int c, char **v) {
    if(c!=2) return 1;
    
    key_t k = ftok(v[1], 1);
    int id = shmget(k, 101, IPC_CREAT | 0600);
    volatile char *m = shmat(id, 0, 0);
    int fd = open(v[1], O_RDONLY);
    
    if(k==-1||id==-1||m==(void*)-1||fd==-1) return 1;

    l("shared memory attached, key=", k);
    l("waiting for receiver..\n", -999);

    *m = -3;
    while(*m == -3);

    char b[100];
    int r;
    while((r = read(fd, b, 100)) > 0) {
        while(*m != 0);
        for(int i=0; i<r; i++) m[i+1] = b[i];
        *m = r;
        l(" byte(s) sent...", r);
    }

    while(*m != 0);
    *m = -1;

    while(*m != -2);
    l("copying finished, receiver finished\n", -999);

    close(fd);
    shmdt((void*)m);
    shmctl(id, IPC_RMID, 0);
    l("shared memory detached\n", -999);
    return 0;
}
