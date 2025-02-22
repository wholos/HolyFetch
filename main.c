// HolyFetch
// https://github.com/wholos
#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256

void get_distro_name() {
    FILE *file = fopen("/etc/os-release", "r");
    if (!file) {
        perror("Error!");
        return;
    }

    char line[BUF_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            char *start = strchr(line, '"');
            if (start) {
                char *end = strrchr(line, '"');
                if (end && end > start) {
                    *end = '\0';
                    printf("OS: %s\n", start + 1);
                }
            }
            break;
        }
    }
    fclose(file);
}

const char* get_shell_name() {
    char *shell = getenv("SHELL");
    if (!shell) return "unknown";

    char *last_slash = strrchr(shell, '/');
    return last_slash ? last_slash + 1 : shell;
}

int main() {
    struct utsname sysinfo;
    if (uname(&sysinfo) == 0) {
	    char *user = getenv("USER");
        printf("%s@%s\n", user, sysinfo.nodename);
		printf("-------------\n");
	    get_distro_name();
        printf("Kernel: %s\n", sysinfo.release);
		printf("Shell: %s\n", get_shell_name());
  }
  return 0;
}
