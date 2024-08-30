#include "../inc/minishell.h"

char *findPath(char *cmd, char **env) {
    char *path_env = NULL;
    char *path;
    char *full_path;
    char *paths[256];
    char *token;
    int i;

    while (*env) {
        if (ft_strncmp(*env, "PATH=", 5) == 0) {
            path_env = *env + 5; // Ignorar "PATH="
            break;
        }
        env++;
    }

    if (!path_env) {
        return NULL; // PATH não encontrado
    }

    i = 0;
    token = strtok(path_env, ":");
    while (token != NULL && i < 256) {
        paths[i++] = token;
        token = strtok(NULL, ":");
    }
    paths[i] = NULL;

    for (int j = 0; paths[j] != NULL; j++) {
        full_path = malloc(strlen(paths[j]) + strlen(cmd) + 2);
        if (!full_path) {
            perror("malloc");
            exit(1);
        }
        sprintf(full_path, "%s/%s", paths[j], cmd);

        if (access(full_path, X_OK) == 0) {
            return full_path; // Caminho encontrado
        }
        free(full_path);
    }

    return NULL; // Comando não encontrado
}
