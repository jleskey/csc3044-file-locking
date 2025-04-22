/* filelock.c
 * demonstrate file locking using POSIX fcntl interface
 * Adapted from Linux Programming Interface, chapter 55,
 * Listing 55-2.
 */
#include <sys/stat.h>
#include <fcntl.h> /* fcntl API for file locking */
#include <sys/types.h>
#include <unistd.h> /* getpid system call */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_LINE 100

static void
displayCmdFmt(void)
{
    printf("\n Open File command: o n filename, where\n");
    printf("    n is a file stream id, integer 0..3\n");
    printf(" Locking command: cmd n start length, where\n");
    printf("    'cmd' is 'w' (SETLKW), 's' (SETLK), 'g' (GETLK), or 'u' (UNLOCK)\n");
    printf("    n is a file stream id, integer 0..3\n");
    printf("    'start' and 'length' specify byte range of file to lock\n");
    printf(" Exit command: q\n\n");
}

void errExit(char *msg)
{
    printf("Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

int main()
{
    int numRead, cmd, status;
    int fd[4], fnum;
    char cmdCh, line[MAX_LINE];
    char fname[MAX_LINE];
    struct flock fl;
    long long len, st;

    displayCmdFmt();

    while (1)
    { /* Prompt for locking command and carry it out */
        printf("PID=%ld> ", (long)getpid());
        fflush(stdout);

        if (fgets(line, MAX_LINE, stdin) == NULL) /* EOF */
            exit(EXIT_SUCCESS);
        line[strlen(line) - 1] = '\0'; /* Remove trailing '\n' */

        if (*line == '\0')
            continue; /* Skip blank lines */

        if (line[0] == 'q')
        {
            break;
        }
        else if (line[0] == '?')
        {
            displayCmdFmt();
            continue;
        }
        else if (line[0] == 'o')
        {
            numRead = sscanf(line, "%c %d %s", &cmdCh, &fnum, fname);
            fd[fnum] = open(fname, O_RDWR);
            if (fd[fnum] == -1)
                printf("Error: could not open file: %s", fname);
            continue;
        }

        numRead = sscanf(line, "%c %d %lld %lld", &cmdCh, &fnum, &st, &len);
        fl.l_start = st;
        fl.l_len = len;
        if (numRead < 4 || strchr("gswu", cmdCh) == NULL)
        {
            printf("Invalid command!\n");
            continue;
        }

        cmd = (cmdCh == 'w') ? F_SETLKW : (cmdCh == 'u') ? F_SETLK
                                                         : F_SETLK;
        fl.l_type = (cmdCh == 'w') ? F_WRLCK : (cmdCh == 'u') ? F_UNLCK
                                                              : F_RDLCK;
        fl.l_whence = SEEK_SET;

        printf("Doing fcntl with fd %d, cmd %d\n", fd[fnum], cmd);
        status = fcntl(fd[fnum], cmd, &fl); /* Perform request... */

        if (cmd == F_GETLK)
        { /* ... and see what happened */
            if (status == -1)
            {
                printf("Error: fcntl - F_GETLK");
            }
            else
            {
                if (fl.l_type == F_UNLCK)
                    printf("[PID=%ld] Lock can be placed\n", (long)getpid());
                else /* Locked out by someone else */
                    printf("[PID=%ld] Denied by %s lock on %lld:%lld "
                           "(held by PID %ld)\n",
                           (long)getpid(),
                           (fl.l_type == F_RDLCK) ? "READ" : "WRITE",
                           (long long)fl.l_start,
                           (long long)fl.l_len, (long)fl.l_pid);
            }
        }
        else
        { /* F_SETLK, F_SETLKW */
            if (status == 0)
                printf("[PID=%ld] %s\n", (long)getpid(),
                       (cmdCh == 'u') ? "unlocked" : "got lock");
            else if (errno == EAGAIN || errno == EACCES) /* F_SETLK */
                printf("[PID=%ld] failed (incompatible lock)\n",
                       (long)getpid());
            else if (errno == EDEADLK) /* F_SETLKW */
                printf("[PID=%ld] failed (deadlock)\n", (long)getpid());
            else
                printf("Error: fcntl - F_SETLK(W)");
        }
    }
}
