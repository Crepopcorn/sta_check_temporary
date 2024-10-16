
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// used for generate random STA report to test run 

float generate_random_float(float min, float max) {
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void generate_sta_report() {
    FILE *fptr;
    fptr = fopen("../reports/sta_report.txt", "w");

    if (fptr == NULL) {
        printf("Error in creating STA report file!\n");
        exit(1);
    }

    for (int i = 1; i <= 10; i++) {
        float arrival_time = generate_random_float(5.0, 12.0);
        float required_time = generate_random_float(6.0, 12.0);
        float slack = arrival_time - required_time;

        fprintf(fptr, "Path %d: clk -> data_out%d\n", i, i);
        fprintf(fptr, "  arrival_time = %.2f\n", arrival_time);
        fprintf(fptr, "  required_time = %.2f\n", required_time);
        fprintf(fptr, "  slack = %.2f\n\n", slack);
    }

    fprintf(fptr, "Path 11: clk -> critical_data\n");
    fprintf(fptr, "  arrival_time = 11.00\n");
    fprintf(fptr, "  required_time = 10.50\n");
    fprintf(fptr, "  slack = -0.50\n\n");

    fprintf(fptr, "Path 12: clk -> boundary_case\n");
    fprintf(fptr, "  arrival_time = 9.00\n");
    fprintf(fptr, "  required_time = 9.00\n");
    fprintf(fptr, "  slack = 0.00\n\n");

    fclose(fptr);
    printf("STA report generated with 12 paths.\n");
}

int main() {
    srand(time(NULL));

    generate_sta_report();
    return 0;
}
