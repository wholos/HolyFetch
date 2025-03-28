// HolyFetch
// https://github.com/wholos
#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

#define BUF_SIZE 256
#define CONFIG_DIR ".config/holyfetch"
#define CONFIG_FILE "config.cfg"
#define DEFAULT_LOGO " ### \n#    \n#    \n#    \n ### "

const char* get_logo_from_config() {
    struct passwd *pw = getpwuid(getuid());
    if (!pw) return DEFAULT_LOGO;

    char config_path[BUF_SIZE];
    snprintf(config_path, sizeof(config_path), "%s/%s/%s", pw->pw_dir, CONFIG_DIR, CONFIG_FILE);

    FILE *file = fopen(config_path, "r");
    if (!file) return DEFAULT_LOGO;

    static char logo[BUF_SIZE * 5] = {0};
    char line[BUF_SIZE];
    size_t total_len = 0;

    while (fgets(line, sizeof(line), file) && total_len < sizeof(logo) - 1) {
        strncat(logo, line, sizeof(logo) - total_len - 1);
        total_len += strlen(line);
    }

    fclose(file);

    return strlen(logo) > 0 ? logo : DEFAULT_LOGO;
}

void ensure_config_exists() {
    struct passwd *pw = getpwuid(getuid());
    if (!pw) return;

    char config_dir_path[BUF_SIZE];
    snprintf(config_dir_path, sizeof(config_dir_path), "%s/%s", pw->pw_dir, CONFIG_DIR);

    mkdir(config_dir_path, 0755);

    char config_file_path[BUF_SIZE];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", config_dir_path, CONFIG_FILE);

    if (access(config_file_path, F_OK) == 0) return;

    FILE *file = fopen(config_file_path, "w");
    if (file) {
        fprintf(file, DEFAULT_LOGO);
        fclose(file);
    }
}

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
    ensure_config_exists();

    struct utsname sysinfo;
    if (uname(&sysinfo) == 0) {
        char *user = getenv("USER");

        const char *logo = get_logo_from_config();

        const char *logo_lines[5] = {0};
        char logo_copy[BUF_SIZE * 5];
        strncpy(logo_copy, logo, sizeof(logo_copy));

        char *line = strtok(logo_copy, "\n");
        int i = 0;
        while (line && i < 5) {
            logo_lines[i++] = line;
            line = strtok(NULL, "\n");
        }

        printf("%s %s@%s\n", logo_lines[0], user, sysinfo.nodename);
        printf("%s -------------\n", logo_lines[1]);
        printf("%s ", logo_lines[2]);
        get_distro_name();
        printf("%s ", logo_lines[3]);
        printf("Kernel: %s\n", sysinfo.release);
        printf("%s ", logo_lines[4]);
        printf("Shell: %s\n", get_shell_name());
    }
    return 0;
}
