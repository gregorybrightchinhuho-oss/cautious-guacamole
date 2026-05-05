#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_EVENTS 20000
#define MAX_RESIDUAL 20000
#define TOLERANCE 0.5

int load_csv(const char *fname, double *t) {
    FILE *f = fopen(fname, "r");
    if (!f) { printf("File error\n"); return 0; }
    char line[64]; int n = 0;
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f) && n < MAX_EVENTS) {
        t[n++] = atof(line);
    }
    fclose(f);
    return n;
}

void subtract_pulsar(double *times, int *n, double period, double phase) {
    int keep = 0;
    for (int i = 0; i < *n; i++) {
        double rel = fmod(times[i] - phase + period, period);
        if (rel > period/2) rel -= period;
        if (fabs(rel) > TOLERANCE) {
            times[keep++] = times[i];
        }
    }
    *n = keep;
}

int main(int argc, char *argv[]) {
    if (argc!= 2) { printf("Usage: %s data.csv\n", argv[0]); return 1; }

    static double times[MAX_RESIDUAL];
    int n = load_csv(argv[1], times);
    if (n == 0) return 1;

    printf("DengaScan v5.2 C - Tight Tolerance\n");
    printf("Loaded %d events from %s\n\n", n, argv[1]);

    double test_periods[] = {7.0, 11.0, 15.0, 22.0, 30.0, 45.0, 60.0, 90.0};
    int num_periods = 8;
    int discovery = 3;

    while (n > 50) {
        double best_period = 0, best_phase = 0, best_sigma = 0;
        int best_hits = 0;
