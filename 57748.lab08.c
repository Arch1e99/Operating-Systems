// 221A LAB08
// Artur Kovalenko
// ka57748@zut.edu.pl

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int isnum(char *s){
    for(int i=0;s[i];i++) if(!isdigit(s[i])) return 0;
    return 1;
}

int main(int argc, char **argv){
    if(argc != 2){ fprintf(stderr,"Blad: niepoprawna liczba argumentow\n"); exit(1); }
    if(!isnum(argv[1])){ fprintf(stderr,"Blad: argument musi byc liczba naturalna\n"); exit(2); }

    int n = atoi(argv[1]);
    if(n < 1 || n > 13){ fprintf(stderr,"Blad: argument musi byc z przedzialu <1..13>\n"); exit(3); }
    if(n == 1 || n == 2) exit(1);

    char a1[3], a2[3];
    sprintf(a1,"%d",n-1);
    sprintf(a2,"%d",n-2);

    pid_t c1 = fork();
    if(c1 == 0) execl(argv[0], argv[0], a1, NULL);

    pid_t c2 = fork();
    if(c2 == 0) execl(argv[0], argv[0], a2, NULL);

    int st1, st2;
    pid_t w1 = wait(&st1);
    pid_t w2 = wait(&st2);

    int r1 = WEXITSTATUS(st1);
    int r2 = WEXITSTATUS(st2);

    pid_t fp = w1==c1 ? c1 : c2;
    pid_t sp = w1==c1 ? c2 : c1;
    int fa = w1==c1 ? n-1 : n-2;
    int sa = w1==c1 ? n-2 : n-1;
    int fr = w1==c1 ? r1 : r1;
    int sr = w1==c1 ? r2 : r2;

    printf(" %5d %5d %2d %2d\n", getpid(), fp, fa, fr);
    printf(" %5d %5d %2d %2d\n", getpid(), sp, sa, sr);
    printf(" %5d %d\n", getpid(), fr+sr);

    exit(fr+sr);
}

