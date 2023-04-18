#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Implementado con metodo de montecarlo
#define NUM_HILOS 1
#define NUM_PUNTOS 100000000

double estimate_pi(int num_points) {
    int i, count = 0;
    double x, y;

    srand(time(NULL));

    for (i = 0; i < num_points; i++) {
        x = (double) rand() / RAND_MAX;
        y = (double) rand() / RAND_MAX;
        if (x*x + y*y <= 1.0) {
            count++;
        }
    }
//multiplica *4 por la formula del metodo de montecarlo
    return 4.0 * count / num_points;
}

//Entrada a subprocesos, por metodo de montecarlo
void *worker(void *arg) {
    int tid = *(int*)arg;
  //el puntero arg ayuda a almacenar los resultados de los subprocesos
    int start = tid * (NUM_PUNTOS / NUM_HILOS);
    int end = (tid + 1) * (NUM_PUNTOS / NUM_HILOS);
  //restamos proceso final e inicial
    int num_points = end - start;

    double *result = (double*)malloc(sizeof(double));
    *result = estimate_pi(num_points);
//Retorna el valor del puntero 
    return (void*)result;
}

int main() {
    int i, tids[NUM_HILOS];
    double pi = 0.0;
    pthread_t threads[NUM_HILOS];
    void *result;

    for (i = 0; i < NUM_HILOS; i++) {
        tids[i] = i;
      //creamos un nuevo hilo
        pthread_create(&threads[i], NULL, worker, (void*)&tids[i]);
    }

    for (i = 0; i < NUM_HILOS; i++) {
      //retorna el valor del hilo si su resultado es distinto a NULL
        pthread_join(threads[i], &result);
        pi += *(double*)result;
      //guarda en un espacio de memoria generado por malloc
        free(result);
    }

    printf("El valor estimado de Pi es: %.8f\n", pi / NUM_HILOS);

    return 0;
}
