#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_cpu_info();
void print_kernel_info();
void print_uptime_info();

int main(int argc, char *argv[]) {
    if ((strcmp(argv[1], "-c")) == 0)
        print_cpu_info();
    if ((strcmp(argv[1], "-k")) == 0)
        print_kernel_info();
    if ((strcmp(argv[1], "-u")) == 0)
        print_uptime_info();

    puts("Press enter to continue");
    getchar();

    return 0;
}

void print_cpu_info() {
    FILE *fp;
    char *filename = "/proc/cpuinfo";
    char buffer[80];

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("\nError opening file '%s' for reading", filename);
        exit(1);
    }

    while (true) {
        fgets(buffer, 80, fp);

        if (feof(fp) == true)
            break;

        char *s = strstr(buffer, "model name");

        if (s) {
            puts(s);
            break;
        }
    }

    if (feof(fp) != true) {
        char *substring = strchr(buffer, ':');
        printf("\n%s", substring);
    }

    fclose(fp);
}

void print_kernel_info() {
    FILE *fp;
    char *filename = "/proc/version";
    char buffer[80];

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("\nError opening file '%s' for reading", filename);
        exit(1);
    }

    while (true) {
        fgets(buffer, 80, fp);

        if (feof(fp) == true)
            break;

        // printf( "\n%s", buffer );

        char *s = strstr(buffer, "Linux version");
        if (s) {
            puts(s);
            break;
        }
    }

    if (feof(fp) != true) {
        char *substring = strchr(buffer, '2');
        substring++;
        puts(substring);
    }

    fclose(fp);
}

void print_uptime_info() {
    FILE *fp;
    char *filename = "/proc/uptime";
    char buffer[80];

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("\nError opening file '%s' for reading", filename);
        exit(1);
    }

    while (true) {
        fgets(buffer, 80, fp);

        if (feof(fp) == true)
            break;

        if (strstr(buffer, " ")) {
            char *s = strstr(buffer, " ");
            printf("\n%s", s);
            break;
        }
    }

    if (feof(fp) != true) {
        char *substring = strchr(buffer, ' ');
        substring++;
        printf("\n%s", substring);
    }

    fclose(fp);
}
