// q4_merger.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char *input_file;
    int out_fd;
    off_t offset;
    size_t size;
    pthread_mutex_t *write_lock;
} thread_arg_t;

void* merge_file(void* arg) {
    thread_arg_t *targ = (thread_arg_t*)arg;
    int in_fd = open(targ->input_file, O_RDONLY);
    if (in_fd < 0) {
        perror(targ->input_file);
        pthread_exit(NULL);
    }

    char *buffer = malloc(targ->size);
    if (!buffer) {
        perror("malloc");
        close(in_fd);
        pthread_exit(NULL);
    }

    ssize_t bytes_read = read(in_fd, buffer, targ->size);
    if (bytes_read < 0) {
        perror("read");
        free(buffer);
        close(in_fd);
        pthread_exit(NULL);
    }
    close(in_fd);

    pthread_mutex_lock(targ->write_lock);
    if (lseek(targ->out_fd, targ->offset, SEEK_SET) < 0) {
        perror("lseek");
        pthread_mutex_unlock(targ->write_lock);
        free(buffer);
        pthread_exit(NULL);
    }

    ssize_t bytes_written = write(targ->out_fd, buffer, bytes_read);
    if (bytes_written != bytes_read) {
        perror("write");
    }
    pthread_mutex_unlock(targ->write_lock);

    free(buffer);
    pthread_exit(NULL);
}

off_t get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) {
        perror(filename);
        return -1;
    }
    return st.st_size;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s merged_log.txt file1.txt file2.txt ... <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[argc - 1]);
    int num_files = argc - 3;
    if (num_threads < 1 || num_threads > num_files) {
        fprintf(stderr, "Number of threads must be between 1 and number of files (%d)\n", num_files);
        return 1;
    }

    int out_fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (out_fd < 0) {
        perror("open output");
        return 1;
    }

    off_t *file_sizes = malloc(sizeof(off_t) * num_files);
    if (!file_sizes) {
        perror("malloc");
        close(out_fd);
        return 1;
    }

    // Get file sizes
    off_t total_size = 0;
    for (int i = 0; i < num_files; i++) {
        file_sizes[i] = get_file_size(argv[2 + i]);
        if (file_sizes[i] < 0) {
            free(file_sizes);
            close(out_fd);
            return 1;
        }
        total_size += file_sizes[i];
    }

    // Preallocate output file size
    if (ftruncate(out_fd, total_size) < 0) {
        perror("ftruncate");
        free(file_sizes);
        close(out_fd);
        return 1;
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    thread_arg_t *targs = malloc(sizeof(thread_arg_t) * num_files);
    pthread_mutex_t write_lock;
    pthread_mutex_init(&write_lock, NULL);

    off_t offset = 0;
    for (int i = 0; i < num_files; i++) {
        targs[i].input_file = argv[2 + i];
        targs[i].out_fd = out_fd;
        targs[i].offset = offset;
        targs[i].size = file_sizes[i];
        targs[i].write_lock = &write_lock;
        offset += file_sizes[i];
    }

    // Create one thread per file (up to num_threads)
    for (int i = 0; i < num_files; i++) {
        if (pthread_create(&threads[i], NULL, merge_file, &targs[i]) != 0) {
            perror("pthread_create");
            // Continue creating others
        }
    }

    for (int i = 0; i < num_files; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&write_lock);

    free(file_sizes);
    free(threads);
    free(targs);
    close(out_fd);

    return 0;
}
