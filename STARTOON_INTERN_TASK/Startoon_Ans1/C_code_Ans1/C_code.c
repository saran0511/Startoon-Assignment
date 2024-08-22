#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_DATA_SIZE 1000
#define PLOT_WIDTH 70
#define PLOT_HEIGHT 20

void load_data(const char *filename, double *data, int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    *size = 0;
    while (fscanf(file, "%lf", &data[(*size)++]) != EOF);
    fclose(file);
}

void find_peaks(double *data, int size, int *peaks, int *num_peaks, int distance, double prominence) {
    *num_peaks = 0;
    for (int i = distance; i < size - distance; i++) {
        bool is_peak = true;
        for (int j = 1; j <= distance; j++) {
            if (data[i] <= data[i - j] || data[i] <= data[i + j]) {
                is_peak = false;
                break;
            }
        }
        if (is_peak && data[i] > prominence) {
            peaks[(*num_peaks)++] = i;
        }
    }
}

void plot_ascii(double *data, int size, int *peaks, int num_peaks, int *minima, int num_minima, const char *label) {
    double max_val = data[0], min_val = data[0];
    for (int i = 1; i < size; i++) {
        if (data[i] > max_val) max_val = data[i];
        if (data[i] < min_val) min_val = data[i];
    }

    char plot[PLOT_HEIGHT][PLOT_WIDTH];
    for (int i = 0; i < PLOT_HEIGHT; i++) {
        for (int j = 0; j < PLOT_WIDTH; j++) {
            plot[i][j] = ' ';
        }
    }

    for (int i = 0; i < size; i++) {
        int x = (i * PLOT_WIDTH) / size;
        int y = (int)((data[i] - min_val) / (max_val - min_val) * (PLOT_HEIGHT - 1));
        plot[PLOT_HEIGHT - 1 - y][x] = '*';
    }

    for (int i = 0; i < num_peaks; i++) {
        int x = (peaks[i] * PLOT_WIDTH) / size;
        int y = (int)((data[peaks[i]] - min_val) / (max_val - min_val) * (PLOT_HEIGHT - 1));
        plot[PLOT_HEIGHT - 1 - y][x] = 'M';
    }

    for (int i = 0; i < num_minima; i++) {
        int x = (minima[i] * PLOT_WIDTH) / size;
        int y = (int)((data[minima[i]] - min_val) / (max_val - min_val) * (PLOT_HEIGHT - 1));
        plot[PLOT_HEIGHT - 1 - y][x] = 'm';
    }

    printf("\n%s\n", label);
    for (int i = 0; i < PLOT_HEIGHT; i++) {
        for (int j = 0; j < PLOT_WIDTH; j++) {
            printf("%c", plot[i][j]);
        }
        printf("\n");
    }
}

void find_and_plot_peaks(double *data, int size, const char *label) {
    int peaks[MAX_DATA_SIZE], num_peaks;
    double inversed_data[MAX_DATA_SIZE];

    // Find maxima (peaks)
    find_peaks(data, size, peaks, &num_peaks, 20, 0.5);

    // Find minima by inverting the signal and finding peaks
    for (int i = 0; i < size; i++) {
        inversed_data[i] = -data[i];
    }
    int minima[MAX_DATA_SIZE], num_minima;
    find_peaks(inversed_data, size, minima, &num_minima, 20, 0.5);

    // Plot the data, peaks, and minima using ASCII art
    plot_ascii(data, size, peaks, num_peaks, minima, num_minima, label);
}

int main() {
    double data1[MAX_DATA_SIZE], data2[MAX_DATA_SIZE];
    int size1, size2;

    load_data("Data_1.txt", data1, &size1);
    load_data("Data_2.txt", data2, &size2);

    find_and_plot_peaks(data1, size1, "Data_1");
    find_and_plot_peaks(data2, size2, "Data_2");

    return 0;
}
