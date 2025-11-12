// q2_buffered.c
#include <stdio.h>
#include <time.h>

int main() {
    FILE *fp = fopen("log_buffered.txt", "w");
    if (!fp) {
        perror("fopen failed");
        return 1;
    }

    for (int i = 0; i < 1000000; i++) {
        time_t t = time(NULL);
        fprintf(fp, "%d: Timestamp: %ld\n", i, t);
    }

    fclose(fp);
    return 0;
}
