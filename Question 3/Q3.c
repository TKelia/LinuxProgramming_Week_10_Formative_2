// q3_palindrome.c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 100000
#define THREADS 20

int global_count = 0;
pthread_mutex_t lock;

int is_palindrome(int num) {
    int rev = 0, temp = num;
    while (temp > 0) {
        rev = rev * 10 + (temp % 10);
        temp /= 10;
    }
    return (rev == num);
}

typedef struct {
    int start;
    int end;
} range_t;

void* count_palindromes(void* arg) {
    range_t *range = (range_t*)arg;
    int local_count = 0;
    for (int i = range->start; i <= range->end; i++) {
        if (is_palindrome(i)) {
            local_count++;
        }
    }
    pthread_mutex_lock(&lock);
    global_count += local_count;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    range_t ranges[THREADS];

    pthread_mutex_init(&lock, NULL);

    int segment = MAX / THREADS;
    for (int i = 0; i < THREADS; i++) {
        ranges[i].start = i * segment;
        if (i == THREADS - 1)
            ranges[i].end = MAX;
        else
            ranges[i].end = (i + 1) * segment - 1;

        pthread_create(&threads[i], NULL, count_palindromes, &ranges[i]);
    }

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);

    printf("The synchronized total number of palindromic numbers from 0 to %d is: %d\n", MAX, global_count);
    return 0;
}
