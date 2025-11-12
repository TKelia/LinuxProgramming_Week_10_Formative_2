// q2_unbuffered.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main() {
    int fd = open("log_unbuffered.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    char buffer[100];
    for (int i = 0; i < 1000000; i++) {
        time_t t = time(NULL);
        int len = snprintf(buffer, sizeof(buffer), "%d: Timestamp: %ld\n", i, t);
        if (write(fd, buffer, len) != len) {
            perror("write failed");
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
