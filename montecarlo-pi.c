#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
    // Vérifier le nombre d'itérations
    if (argc != 2) {
        printf("Usage: %s <nombre_iterations>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Le nombre d'itérations doit être positif\n");
        return 1;
    }

    // Afficher la date/heure de compilation
    printf("Compiled on: %s at %s\n", __DATE__, __TIME__);

    // Afficher l'heure de début
    time_t start_time = time(NULL);
    struct tm *start_tm = localtime(&start_time);
    printf("Execution started on: %s", asctime(start_tm));

    // Initialiser le générateur aléatoire avec l'heure actuelle
    srand(time(NULL));

    // Monte Carlo : compter les points dans le quart de cercle
    int inside = 0;
    for (int i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x*x + y*y <= 1.0) {
            inside++;
        }
    }

    // Calculer l'estimation de π
    double pi_estimate = 4.0 * inside / n;

    // Afficher le résultat
    printf("The approximation of PI using %d iterations is %f\n", n, pi_estimate);

    // Afficher l'heure de fin
    time_t end_time = time(NULL);
    struct tm *end_tm = localtime(&end_time);
    printf("Execution stopped on: %s", asctime(end_tm));

    // Afficher la durée (optionnel)
    printf("Duration: %ld seconds\n", end_time - start_time);

    return 0;
}