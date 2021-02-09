#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define NARGS 2
#define MAXPATH 128
#define PATHSFILE "/.ros2cd/paths.txt"

int
is_ros_pkg(char * path)
{
    DIR *dir;
    struct dirent *folder_entry;
    int found;

    dir = opendir(path);
    if (dir == NULL) {
        return 0;
    }

    found = 0;
    do {
        folder_entry = readdir(dir);
        if (folder_entry != NULL) {
            if (folder_entry->d_type == DT_REG) {
                found = strncmp(folder_entry->d_name, 
                    "package.xml", strlen(folder_entry->d_name)) == 0;
            }
        }
    } while (folder_entry != NULL && !found);

    closedir(dir);

    return found;
}

int
names_are_equals(char * path, char * dst_pkg)
{
    char * p_name;

    p_name = strrchr(path, '/');
    p_name = p_name + sizeof(char);
    printf("%s\n", p_name);
    return strncmp(p_name, dst_pkg, strlen(p_name)) == 0;
}

int
pkg_ok(char * path, char * dst_pkg)
{
    if (is_ros_pkg(path)) {
        return names_are_equals(path, dst_pkg);
    }
    return 0;
}

int
pkg_found(char * path, char * dst_pkg, char *out_pkg)
{
    DIR *dir;
    struct dirent *folder_entry;
    char p[MAXPATH];
    int found;

    dir = opendir(path);
    if (dir == NULL) {
        return 0;
    }

    found = 0;
    do {
        memset(p, '\0', MAXPATH * sizeof(char));
        folder_entry = readdir(dir);
        if (folder_entry != NULL) {
            if (folder_entry->d_type == DT_DIR) {
                if (strcmp(folder_entry->d_name, ".") == 0 || 
                    strcmp(folder_entry->d_name, "..") == 0) {
                        continue;
                    }
                // Create the new sub-path:

                memcpy(p, path, strlen(path) * sizeof(char));
                strncat(p, "/", 1 * sizeof(char));
                strncat(p, folder_entry->d_name, 
                    strlen(folder_entry->d_name) * sizeof(char));
                if(pkg_ok(p, dst_pkg)) {
                    memcpy(out_pkg, p, strlen(p) * sizeof(char));
                    found = 1;
                } else {
                    printf("Probando con: %s\n", p);
                    found = pkg_found(p, dst_pkg, out_pkg);
                }
            }
        }
    } while (folder_entry != NULL && !found);

    closedir(dir);

    return found;
}

int
move_to_pkg(int fd, char * dst_pkg)
{
    int bytes, found, i;
    char c;
    char pkgs_path[MAXPATH];
    char out_pkg[MAXPATH];

    i = 0;
    found = 0;
    memset(out_pkg, '\0', MAXPATH * sizeof(char));
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
            if (pkg_found(pkgs_path, dst_pkg, out_pkg)) {
                // move to 'out_pkg'!
                found = 1;
            }
        }
    } while(bytes > 0 && !found);

    if (!found)
        printf("%s\n", "[WARN] package not found!");

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
