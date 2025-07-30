#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BASLANGIC_KAPASITE 100

typedef struct {
    double *x1, *x2, *x3, *y;
    int boyut, kapasite;
} VeriKumesi;

void veri_kumesini_oku(VeriKumesi *vk) {
    FILE *dosya = fopen("/Users/banuoven/Desktop/data.txt", "r");
    if (!dosya) {
        perror("data.txt acilamadi");
        exit(EXIT_FAILURE);
    }

    vk->boyut = 0;
    vk->kapasite = BASLANGIC_KAPASITE;
    vk->x1 = (double*)malloc(vk->kapasite * sizeof(double));
    vk->x2 = (double*)malloc(vk->kapasite * sizeof(double));
    vk->x3 = (double*)malloc(vk->kapasite * sizeof(double));
    vk->y  = (double*)malloc(vk->kapasite * sizeof(double));

    while (fscanf(dosya, "%lf %lf %lf %lf",
                  &vk->x1[vk->boyut],
                  &vk->x2[vk->boyut],
                  &vk->x3[vk->boyut],
                  &vk->y [vk->boyut]) == 4)
    {
        vk->boyut++;
        if (vk->boyut == vk->kapasite) {
            vk->kapasite *= 2;
            vk->x1 = (double*)realloc(vk->x1, vk->kapasite * sizeof(double));
            vk->x2 = (double*)realloc(vk->x2, vk->kapasite * sizeof(double));
            vk->x3 = (double*)realloc(vk->x3, vk->kapasite * sizeof(double));
            vk->y  = (double*)realloc(vk->y , vk->kapasite * sizeof(double));
        }
    }

    fclose(dosya);
}

double ortalama(const double *dizi, int n) {
    double toplam = 0;
    for (int i = 0; i < n; i++) {
        toplam += dizi[i];
    }
    return toplam / n;
}

double korelasyon(const double *x, const double *y, int n) {
    double ort_x = ortalama(x, n);
    double ort_y = ortalama(y, n);
    double pay = 0, payda_x = 0, payda_y = 0;

    for (int i = 0; i < n; i++) {
        double dx = x[i] - ort_x;
        double dy = y[i] - ort_y;
        pay    += dx * dy;
        payda_x += dx * dx;
        payda_y += dy * dy;
    }

    return pay / sqrt(payda_x * payda_y);
}

void dogrusal_regresyon(const double *x, const double *y, int n, double *a, double *b) {
    double ort_x = ortalama(x, n);
    double ort_y = ortalama(y, n);
    double pay = 0, payda = 0;

    for (int i = 0; i < n; i++) {
        double dx = x[i] - ort_x;
        double dy = y[i] - ort_y;
        pay   += dx * dy;
        payda += dx * dx;
    }

    *b = pay / payda;
    *a = ort_y - (*b) * ort_x;
}

double SSE(const double *x, const double *y, int n, double a, double b, double *y_tahmin) {
    double toplam = 0;

    for (int i = 0; i < n; i++) {
        y_tahmin[i] = a + b * x[i];
        double hata = y[i] - y_tahmin[i];
        toplam += hata * hata;
    }

    return toplam;
}

void karistir_indeksler(int *dizi, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = dizi[i];
        dizi[i] = dizi[j];
        dizi[j] = temp;
    }
}

int main() {
    srand((unsigned)time(NULL));

    VeriKumesi vk;
    veri_kumesini_oku(&vk);
    int N = vk.boyut;

    if (N < 100) {
        fprintf(stderr, "En az 100 satırlık veri gerekli. Mevcut: %d\n", N);
        return EXIT_FAILURE;
    }

    printf("Toplam veri sayısı: %d\n\n", N);

    int *indeks = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) indeks[i] = i;
    karistir_indeksler(indeks, N);

    int n_egitim = (int)(0.7 * N);
    int n_test   = N - n_egitim;

    double *x1_egitim = (double*)malloc(n_egitim * sizeof(double));
    double *x2_egitim = (double*)malloc(n_egitim * sizeof(double));
    double *x3_egitim = (double*)malloc(n_egitim * sizeof(double));
    double *y_egitim  = (double*)malloc(n_egitim * sizeof(double));
    double *x1_test   = (double*)malloc(n_test   * sizeof(double));
    double *x2_test   = (double*)malloc(n_test   * sizeof(double));
    double *x3_test   = (double*)malloc(n_test   * sizeof(double));
    double *y_test    = (double*)malloc(n_test   * sizeof(double));

    for (int i = 0; i < n_egitim; i++) {
        int id = indeks[i];
        x1_egitim[i] = vk.x1[id];
        x2_egitim[i] = vk.x2[id];
        x3_egitim[i] = vk.x3[id];
        y_egitim[i]  = vk.y[id];
    }

    for (int i = 0; i < n_test; i++) {
        int id = indeks[n_egitim + i];
        x1_test[i] = vk.x1[id];
        x2_test[i] = vk.x2[id];
        x3_test[i] = vk.x3[id];
        y_test[i]  = vk.y[id];
    }

    double r1 = korelasyon(x1_egitim, y_egitim, n_egitim);
    double r2 = korelasyon(x2_egitim, y_egitim, n_egitim);
    double r3 = korelasyon(x3_egitim, y_egitim, n_egitim);

    printf("Korelasyonlar:\n");
    printf(" x1 - y: %.4f\n", r1);
    printf(" x2 - y: %.4f\n", r2);
    printf(" x3 - y: %.4f\n\n", r3);

    int secilen = 1;
    double *x_egitim = x1_egitim, *x_test = x1_test;

    if (fabs(r2) > fabs(r1)) { x_egitim = x2_egitim; x_test = x2_test; secilen = 2; }
    if (fabs(r3) > fabs((secilen == 1 ? r1 : r2))) {
        x_egitim = x3_egitim;
        x_test   = x3_test;
        secilen  = 3;
    }

    printf("Secilen degisken: x%d\n", secilen);

    double a, b;
    dogrusal_regresyon(x_egitim, y_egitim, n_egitim, &a, &b);
    printf("Regresyon Denklemi: y = %.4f + %.4f * x%d\n", a, b, secilen);
    printf("a : %.4f\n", a);
    printf("b : %.4f\n\n", b);

    // Eğitim verisi için tahmin ve SSE
    double *y_tahmin_egitim = (double*)malloc(n_egitim * sizeof(double));
    double sse_egitim = SSE(x_egitim, y_egitim, n_egitim, a, b, y_tahmin_egitim);

    printf("=== EGITIM VERISI ===\n");
    printf("SSE (Egitim): %.4f\n", sse_egitim);
    printf("Tahmini ŷ degerleri (Egitim):\n");
    for (int i = 0; i < n_egitim; i++) {
        printf("Tahmini ŷ: %.4f\n",y_tahmin_egitim[i]);
    }

    // Test verisi için tahmin ve SSE (Aynı model kullanılarak)
    double *y_tahmin_test = (double*)malloc(n_test * sizeof(double));
    double sse_test = SSE(x_test, y_test, n_test, a, b, y_tahmin_test);

    printf("\n=== TEST VERISI ===\n");
    printf("SSE (Test): %.4f\n", sse_test);
    printf("Tahmini ŷ degerleri (Test):\n");
    for (int i = 0; i < n_test; i++) {
        printf("Tahmini ŷ: %.4f\n",y_tahmin_test[i]);
    }

    // Bellek temizliği
    free(vk.x1); free(vk.x2); free(vk.x3); free(vk.y);
    free(indeks);
    free(x1_egitim); free(x2_egitim); free(x3_egitim); free(y_egitim);
    free(x1_test); free(x2_test); free(x3_test); free(y_test);
    free(y_tahmin_egitim); free(y_tahmin_test);

    return 0;
}
