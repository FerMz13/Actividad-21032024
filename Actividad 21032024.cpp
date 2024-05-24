#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    int x, y;
} Punto;

Punto* hull;
int tamanoHull = 0;

int distancia(Punto p, Punto q, Punto r) {
    return abs((r.y - p.y) * (q.x - p.x) - (q.y - p.y) * (r.x - p.x));
}

int orientacion(Punto p, Punto q, Punto r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

void agregarPuntoAHull(Punto p) {
    hull[tamanoHull++] = p;
}

void encontrarHull(Punto* puntos, int tamano, Punto p, Punto q) {
    int idx = -1;
    int maxDist = 0;
    for (int i = 0; i < tamano; i++) {
        int dist = distancia(p, q, puntos[i]);
        if (dist > maxDist) {
            idx = i;
            maxDist = dist;
        }
    }
    if (idx == -1) return;
    Punto masLejano = puntos[idx];
    agregarPuntoAHull(masLejano);
    Punto* s1 = (Punto*)malloc(tamano * sizeof(Punto));
    Punto* s2 = (Punto*)malloc(tamano * sizeof(Punto));
    int tamanoS1 = 0, tamanoS2 = 0;
    for (int i = 0; i < tamano; i++) {
        if (orientacion(p, masLejano, puntos[i]) == 2) s1[tamanoS1++] = puntos[i];
        else if (orientacion(masLejano, q, puntos[i]) == 2) s2[tamanoS2++] = puntos[i];
    }
    encontrarHull(s1, tamanoS1, p, masLejano);
    encontrarHull(s2, tamanoS2, masLejano, q);
    free(s1);
    free(s2);
}

void quickHull(Punto* puntos, int tamano) {
    if (tamano < 3) return;
    int minX = 0, maxX = 0;
    for (int i = 1; i < tamano; i++) {
        if (puntos[i].x < puntos[minX].x) minX = i;
        if (puntos[i].x > puntos[maxX].x) maxX = i;
    }
    Punto A = puntos[minX];
    Punto B = puntos[maxX];
    agregarPuntoAHull(A);
    agregarPuntoAHull(B);
    Punto* s1 = (Punto*)malloc(tamano * sizeof(Punto));
    Punto* s2 = (Punto*)malloc(tamano * sizeof(Punto));
    int tamanoS1 = 0, tamanoS2 = 0;
    for (int i = 0; i < tamano; i++) {
        if (orientacion(A, B, puntos[i]) == 2) s1[tamanoS1++] = puntos[i];
        else if (orientacion(B, A, puntos[i]) == 2) s2[tamanoS2++] = puntos[i];
    }
    encontrarHull(s1, tamanoS1, A, B);
    encontrarHull(s2, tamanoS2, B, A);
    free(s1);
    free(s2);
}

int main() {
    int n;
    printf("Introduce el numero de puntos: ");
    scanf("%d", &n);
    
    Punto* puntos = (Punto*)malloc(n * sizeof(Punto));
    hull = (Punto*)malloc(n * sizeof(Punto));
    
    srand(time(0));
    for (int i = 0; i < n; i++) {
        puntos[i].x = rand() % 1000000;
        puntos[i].y = rand() % 1000000;
    }
    
    clock_t inicio = clock();
    quickHull(puntos, n);
    clock_t fin = clock();
    
    double tiempoTranscurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo transcurrido: %.6f segundos\n", tiempoTranscurrido);
    
    printf("Puntos del Convex Hull:\n");
    for (int i = 0; i < tamanoHull; i++) {
        printf("(%d, %d)\n", hull[i].x, hull[i].y);
    }
    
    free(puntos);
    free(hull);
    return 0;
}

