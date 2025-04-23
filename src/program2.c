/**
 * Another file lock acquirer, doomed to failure
 *
 * @file program2.c
 * @author Joseph Leskey <josleskey@mail.mvnu.edu>
 * @date 23 April 2025
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LOCK_COUNT 10000

int main(const int argc, const char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <BYTE PAIR INDEX> <FILE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int index = atoi(argv[1]);
    const char *filename = argv[2];

    int descriptor = open(filename, O_RDWR);

    if (descriptor == -1) {
        fprintf(stderr, "Could not open file `%s`.\n", filename);
        return EXIT_FAILURE;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = index;
    lock.l_len = 1;

    int successes = 0;

    for (int i = 0; i < LOCK_COUNT; i++) {
        // This is intended to fail, so we won't handle errors.
        if (fcntl(descriptor, F_SETLK, &lock) != -1) {
            successes++;
        }
    }

    printf("Attempted %d locks. %d %s successful.\n", LOCK_COUNT,
           successes, successes == 1 ? "was" : "were");
    close(descriptor);
}
