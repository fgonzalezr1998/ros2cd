#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NARGS 2
#define MAXPATH 128
#define PATHSFILE "/.ros2cd/paths.txt"

int
pkg_found(char * path)
{
    return 1;
}

int
move_to_pkg(int fd, char * dst_pkg)
{
    int bytes, finish, i;
    char c;
    char pkgs_path[MAXPATH];

    i = 0;
    finish = 0;
    do {
        bytes = read(fd, &c, 1);
        if (bytes < 0) {
            fprintf(stderr, "[ERROR] error reading from file!");
            return 0;
        }
        if (bytes > 0 && c != '\n') {
            pkgs_path[i] = c;
            i++;
        }
        if (c == '\n' || bytes == 0) {
            pkgs_path[i] = '\0';
            i = 0;
            if (pkg_found(pkgs_path)) {
                // move to it
                finish = 1;
            }
        }
    } while(bytes > 0 && !finish);

    return 1;
}

int
main(int argc, char * argv[])
{
    int fd;
    char * paths_file = getenv("HOME");

    if (argc != NARGS) {
        errx(EXIT_FAILURE, "%s\n", "Usage Error!");
    }

    strncat(paths_file, PATHSFILE, MAXPATH);

    fd = open(paths_file, O_RDONLY);
    if (fd < 0) {
        errx(
            EXIT_FAILURE, "[%s] %s\n", paths_file, "can't be opened!");
    }

    if (!move_to_pkg(fd, argv[1])) {
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    exit(EXIT_SUCCESS);
}
