// 221A LAB10
// Artur Kovalenko
// ka57748@zut.edu.pl

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// zmienne globalne
float *data;
int n, w, mode; // 0 = SUM 1 = VAR
double global_result;
float mean_val = 0;
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

typedef struct { int idx, start, len; } Arg;

// pomiar czasu
double get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

// funkcja watku
void* worker(void* arg) {
    Arg* a = (Arg*)arg;
    char *tag = mode ? "VAR" : "SUM";
    printf("%s: Thread %lx first=%d size=%d\n", tag, pthread_self(), a->start, a->len);

    double local = 0;
    for (int i = 0; i < a->len; i++) {
        float val = data[a->start + i];
        // 1 kwadrat roznicy 0 sama wartosc
        local += mode ? (val - mean_val) * (val - mean_val) : val;
    }

    // sekcja krytyczna
    pthread_mutex_lock(&mx);
    global_result += local;
    pthread_mutex_unlock(&mx);

    printf("%s: Thread %lx %s=%.20lf\n", tag, pthread_self(), mode ? "var" : "sum", local);
    return NULL;
}

// funkcja zarzadzajaca watkami fork-join
void run_threads(pthread_t *th, Arg *args) {
    global_result = 0;
    int pos = 0, base = n / w, rem = n % w;
    
    for (int i = 0; i < w; i++) {
        args[i].start = pos;
        // ostatni watek bierze reszte z dzielenia
        args[i].len = base + (i == w - 1 ? rem : 0);
        pos += args[i].len;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }
    // czekanie na zakonczenie wszystkich watkow
    for (int i = 0; i < w; i++) pthread_join(th[i], NULL);
}

int main(int argc, char *argv[]) {
    // walidacja argumentow
    if (argc != 3) {
        fprintf(stderr, "Blad: Podaj 2 argumenty (n, w)\n");
        return 1;
    }
    n = atoi(argv[1]); 
    w = atoi(argv[2]);
    if (n > 10000000 || w < 1 || w > (n < 16 ? n : 16)) {
        fprintf(stderr, "Blad: Zle wartosci n lub w\n");
        return 1;
    }

    // alokacja pamieci
    data = malloc(n * sizeof(float));
    pthread_t *th = malloc(w * sizeof(pthread_t));
    Arg *args = malloc(w * sizeof(Arg));
    
    // generowanie danych
    srand(0);
    for (int i = 0; i < n; i++) data[i] = rand() * 100.f / RAND_MAX;

    // czesc rownolegla
    double t1 = get_time();
    
    mode = 0; //suma
    run_threads(th, args);
    mean_val = global_result / n; // srednia
    
    mode = 1; // wariancja
    run_threads(th, args); 
    double std_dev_par = sqrt(global_result / n); //OS
    
    printf("w/Threads StdDev=%.20lf time=%f\n", std_dev_par, get_time() - t1);

    // sprawdzenie
    t1 = get_time();
    double seq_sum = 0, seq_var = 0;
    for (int i = 0; i < n; i++) seq_sum += data[i];
    float seq_mean = seq_sum / n;
    for (int i = 0; i < n; i++) seq_var += (data[i] - seq_mean) * (data[i] - seq_mean);
    
    printf("wo/Threads StdDev=%.20lf time=%f\n", sqrt(seq_var / n), get_time() - t1);

    // sprzatanie
    free(data); free(th); free(args);
    return 0;
}



