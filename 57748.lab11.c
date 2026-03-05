// 221A LAB11
// Artur Kovalenko
// ka57748@zut.edu.pl

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// zmienne globalne
float* data;
int n, w, mode; // 0 = SUM 1 = VAR
double global_result;
float mean_val = 0;
HANDLE hMutex;

typedef struct { int idx, start, len; } Arg;

// pomiar czasu
double get_time() {
    LARGE_INTEGER f, c;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart / f.QuadPart;
}

// funkcja watku
DWORD WINAPI worker(LPVOID arg) {
    Arg* a = (Arg*)arg;
    char* tag = mode ? "VAR" : "SUM";
    double local = 0;
    int i;

    DWORD tid = GetCurrentThreadId();

    printf("%s: Thread %lx first=%d size=%d\n", tag, tid, a->start, a->len);

    for (i = 0; i < a->len; i++) {
        float val = data[a->start + i];
        // 1 kwadrat roznicy 0 sama wartosc
        local += mode ? (val - mean_val) * (val - mean_val) : val;
    }

    // sekcja krytyczna
    WaitForSingleObject(hMutex, INFINITE);
    global_result += local;
    ReleaseMutex(hMutex);

    printf("%s: Thread %lx %s=%.20lf\n", tag, tid, mode ? "var" : "sum", local);
    return 0;
}

// funkcja zarzadzajaca watkami
void run_threads(HANDLE* th, Arg* args) {
    int i;
    int pos = 0, base = n / w, rem = n % w;
    global_result = 0;

    for (i = 0; i < w; i++) {
        args[i].start = pos;
        args[i].len = base + (i == w - 1 ? rem : 0); // ostatni bierze reszte
        pos += args[i].len;

        th[i] = CreateThread(NULL, 0, worker, &args[i], 0, NULL);
    }

    WaitForMultipleObjects(w, th, TRUE, INFINITE);

    for (i = 0; i < w; i++) CloseHandle(th[i]);
}

int main(int argc, char* argv[]) {
    HANDLE* th;
    Arg* args;
    double t1;
    double seq_sum = 0, seq_var = 0, diff;
    int i;

    // walidacja argumentow
    if (argc != 3) {
        fprintf(stderr, "Blad: Podaj 2 argumenty (n, w)\n");
        return 1;
    }
    n = atoi(argv[1]);
    w = atoi(argv[2]);
    if (n > 10000000 || w < 1 || w >(n < 16 ? n : 16)) {
        fprintf(stderr, "Blad: Zle wartosci n lub w\n");
        return 1;
    }

    // alokacja pamieci
    data = (float*)malloc(n * sizeof(float));
    th = (HANDLE*)malloc(w * sizeof(HANDLE));
    args = (Arg*)malloc(w * sizeof(Arg));

    // inicjalizacja Mutexu
    hMutex = CreateMutex(NULL, FALSE, NULL);

    // generowanie danych
    srand(0);
    for (i = 0; i < n; i++) data[i] = rand() * 100.f / (float)RAND_MAX;

    // czesc rownolegla
    t1 = get_time();

    mode = 0; // suma
    run_threads(th, args);
    mean_val = (float)(global_result / n);

    mode = 1; // wariancja
    run_threads(th, args);

    printf("w/Threads StdDev=%.20lf time=%f\n", sqrt(global_result / n), get_time() - t1);

    // sprawdzenie
    t1 = get_time();
    for (i = 0; i < n; i++) seq_sum += data[i];
    float seq_mean = (float)(seq_sum / n);

    for (i = 0; i < n; i++) {
        diff = data[i] - seq_mean;
        seq_var += (diff * diff);
    }

    printf("wo/Threads StdDev=%.20lf time=%f\n", sqrt(seq_var / n), get_time() - t1);

    // sprzatanie
    CloseHandle(hMutex);
    free(data); free(th); free(args);
    return 0;
}