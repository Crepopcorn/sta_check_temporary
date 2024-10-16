#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_PATHS 1000000

typedef struct {
    char path_name[MAX_LINE_LEN];
    float slack;
    float data_arrival;
    float data_required;
    float clock_arrival;
    float clock_required;
    int is_setup_violation;
    int is_hold_violation;
} PathTiming;

float extract_value(const char* line, const char* keyword);
void check_timing_violations(const char* file_name);
void tokenize_and_parse_line(char* line, PathTiming* path_timing);
void print_timing_report(PathTiming paths[], int count);
void check_clock_skew(PathTiming paths[], int count);
void analyze_paths(PathTiming paths[], int count);

float extract_value(const char* line, const char* keyword) {
    char *pos = strstr(line, keyword);
    if (pos != NULL) {
        return atof(pos + strlen(keyword));  // Move past the keyword and extract value
    }
    return 0.0;
}

void tokenize_and_parse_line(char* line, PathTiming* path_timing) {
    if (strstr(line, "slack")) {
        path_timing->slack = extract_value(line, "slack:");
    } else if (strstr(line, "data arrival time")) {
        path_timing->data_arrival = extract_value(line, "data arrival time:");
    } else if (strstr(line, "data required time")) {
        path_timing->data_required = extract_value(line, "data required time:");
    } else if (strstr(line, "clock arrival time")) {
        path_timing->clock_arrival = extract_value(line, "clock arrival time:");
    } else if (strstr(line, "clock required time")) {
        path_timing->clock_required = extract_value(line, "clock required time:");
    }
}

void check_timing_violations(const char* file_name) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", file_name);
        return;
    }

    char line[MAX_LINE_LEN];
    int path_count = 0;
    PathTiming paths[MAX_PATHS];
    PathTiming *current_path = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Path")) {  // Beginning of a new path
            current_path = &paths[path_count++];
            strcpy(current_path->path_name, line);  // Save path name
        } else if (current_path) {
            // Tokenize and extract timing information
            tokenize_and_parse_line(line, current_path);
        }
    }

    analyze_paths(paths, path_count);

    print_timing_report(paths, path_count);
    
    fclose(file);
}

void check_clock_skew(PathTiming paths[], int count) {
    for (int i = 0; i < count; i++) {
        float skew = paths[i].clock_required - paths[i].clock_arrival;
        printf("Path: %s, Clock Skew: %.2f\n", paths[i].path_name, skew);
    }
}

void analyze_paths(PathTiming paths[], int count) {
    for (int i = 0; i < count; i++) {
        if (paths[i].slack < 0) {
            if (paths[i].data_required > paths[i].data_arrival) {
                paths[i].is_setup_violation = 1;
                printf("Setup violation detected: Path = %s, Slack = %.2f\n", paths[i].path_name, paths[i].slack);
            } else {
                paths[i].is_hold_violation = 1;
                printf("Hold violation detected: Path = %s, Slack = %.2f\n", paths[i].path_name, paths[i].slack);
            }
        }
    }
}

void print_timing_report(PathTiming paths[], int count) {
    printf("\n---------------- Timing Report ----------------\n");
    for (int i = 0; i < count; i++) {
        printf("Path: %s\n", paths[i].path_name);
        printf("  Slack: %.2f\n", paths[i].slack);
        printf("  Data Arrival: %.2f\n", paths[i].data_arrival);
        printf("  Data Required: %.2f\n", paths[i].data_required);
        printf("  Clock Arrival: %.2f\n", paths[i].clock_arrival);
        printf("  Clock Required: %.2f\n", paths[i].clock_required);

        if (paths[i].is_setup_violation) {
            printf("  Status: Setup violation\n");
        } else if (paths[i].is_hold_violation) {
            printf("  Status: Hold violation\n");
        } else {
            printf("  Status: No violation\n");
        }
        printf("-------------------------------------------------\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <STA Report File>\n", argv[0]);
        return 1;
    }

    const char *file_name = argv[1];
    check_timing_violations(file_name);
    
    return 0;
}
