// 221A LAB09
// Artur Kovalenko
// ka57748@zut.edu.pl

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int isnum(const char* s) {
    if (!s || !*s) return 0;
    for (int i = 0; s[i]; i++) if (!isdigit((unsigned char)s[i])) return 0;
    return 1;
}

int main(int argc, char** argv) {
    if (argc != 2) { fprintf(stderr, "Blad: niepoprawna liczba argumentow\n"); return 1; }
    if (!isnum(argv[1])) { fprintf(stderr, "Blad: argument musi byc liczba naturalna\n"); return 2; }

    int n = atoi(argv[1]);
    if (n < 1 || n > 13) { fprintf(stderr, "Blad: argument musi byc z przedzialu <1..13>\n"); return 3; }

    if (n == 1 || n == 2) return 1;

    char exePath[512];
    if (!GetModuleFileNameA(NULL, exePath, (DWORD)sizeof(exePath))) return 1;

    char cmd1[256], cmd2[256];
    snprintf(cmd1, sizeof(cmd1), "\"%s\" %d", exePath, n - 1);
    snprintf(cmd2, sizeof(cmd2), "\"%s\" %d", exePath, n - 2);

    STARTUPINFOA si1, si2;
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&si1, sizeof(si1)); si1.cb = sizeof(si1);
    ZeroMemory(&si2, sizeof(si2)); si2.cb = sizeof(si2);
    ZeroMemory(&pi1, sizeof(pi1));
    ZeroMemory(&pi2, sizeof(pi2));

    CreateProcessA(NULL, cmd1, NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1);
    CreateProcessA(NULL, cmd2, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2);

    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hThread);

    HANDLE hs[2] = { pi1.hProcess, pi2.hProcess };
    DWORD first = WaitForMultipleObjects(2, hs, FALSE, INFINITE);

    int i1 = (int)(first - WAIT_OBJECT_0);
    int i2 = 1 - i1;

    WaitForSingleObject(hs[i2], INFINITE);

    DWORD ec1 = 0, ec2 = 0;
    PROCESS_INFORMATION pi[2] = { pi1, pi2 };
    GetExitCodeProcess(pi[i1].hProcess, &ec1);
    GetExitCodeProcess(pi[i2].hProcess, &ec2);

    int arg_first = (i1 == 0) ? (n - 1) : (n - 2);
    int arg_second = (i2 == 0) ? (n - 1) : (n - 2);

    DWORD ppid = GetCurrentProcessId();

    printf(" %5lu %5lu %2d %2lu\n", (unsigned long)ppid, (unsigned long)pi[i1].dwProcessId, arg_first, (unsigned long)ec1);
    printf(" %5lu %5lu %2d %2lu\n", (unsigned long)ppid, (unsigned long)pi[i2].dwProcessId, arg_second, (unsigned long)ec2);

    printf(" %5lu %5s %2s %2lu\n", (unsigned long)ppid, "", "", (unsigned long)(ec1 + ec2));

    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hProcess);

    return (int)(ec1 + ec2);
}
