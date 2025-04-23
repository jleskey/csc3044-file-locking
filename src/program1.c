/**
 * A file lock acquirer
 *
 * @file program1.c
 * @author Joseph Leskey <josleskey@mail.mvnu.edu>
 * @date 22 April 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define LOCK_COUNT 40001

int main(const int argc, const char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <FILE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int descriptor = open(argv[1], O_RDWR | O_CREAT, 0644);

    if (descriptor == -1) {
        fprintf(stderr, "Could not open file `%s`.\n", argv[1]);
        return EXIT_FAILURE;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_len = 1;

    for (int i = 0; i < LOCK_COUNT; i++) {
        lock.l_start = i * 2;

        if (fcntl(descriptor, F_SETLK, &lock) == -1) {
            fprintf(stderr, "Failed to set lock at byte %lld\n", lock.l_start);
            close(descriptor);
            return EXIT_FAILURE;
        }
    }

    printf("Set %d locks successfully. Time to sleep.\n", LOCK_COUNT);
    pause();
    close(descriptor);
}
