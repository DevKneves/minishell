#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

// Implementação da função ft_strcmp
int ft_strcmp(const char *s1, const char *s2) {
    unsigned char *p1 = (unsigned char *)s1;
    unsigned char *p2 = (unsigned char *)s2;

    while (*p1 && (*p1 == *p2)) {
        p1++;
        p2++;
    }

    return (*p1 - *p2);
}

// Implementação da função ft_strncmp
int ft_strncmp(const char *s1, const char *s2, size_t n) {
    unsigned char *p1 = (unsigned char *)s1;
    unsigned char *p2 = (unsigned char *)s2;
    size_t i = 0;

    while ((p1[i] || p2[i]) && i < n) {
        if (p1[i] != p2[i]) {
            return (p1[i] - p2[i]);
        }
        i++;
    }
    return (0);
}

// Implementação da função ft_strlen
size_t ft_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Implementação da função ft_strdup
char *ft_strdup(const char *str) {
    size_t len = ft_strlen(str) + 1;
    char *dup = (char *)malloc(len);
    if (dup) {
        for (size_t i = 0; i < len; i++) {
            dup[i] = str[i];
        }
    }
    return dup;
}

// Implementação da função ft_strcat
char *ft_strcat(char *dest, const char *src) {
    char *d = dest;
    while (*d) d++;
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

// Implementação da função ft_strcpy
char *ft_strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++)) {}
    return dest;
}

// Implementação da função ft_strncpy
char *ft_strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    while (n-- && (*d++ = *src++)) {}
    while (n--) *d++ = '\0';
    return dest;
}

// Implementação da função ft_strtok
char *ft_strtok(char *str, const char *delim) {
    static char *save_ptr;
    char *token;

    if (str == NULL) {
        str = save_ptr;
    }

    if (str == NULL) {
        return NULL;
    }

    // Skip leading delimiters
    while (*str && strchr(delim, *str)) {
        str++;
    }

    if (*str == '\0') {
        return NULL;
    }

    token = str;
    
    // Find the end of the token
    while (*str && !strchr(delim, *str)) {
        str++;
    }

    if (*str) {
        *str = '\0';
        save_ptr = str + 1;
    } else {
        save_ptr = NULL;
    }

    return token;
}

// Implementação da função ft_sprintf
int ft_sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = 0;
    char c;
    const char *p = format;
    char *s;
    int d;

    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'd':
                    d = va_arg(args, int);
                    count += sprintf(buffer + count, "%d", d);
                    break;
                case 's':
                    s = va_arg(args, char *);
                    count += sprintf(buffer + count, "%s", s);
                    break;
                case 'c':
                    c = (char)va_arg(args, int);
                    buffer[count++] = c;
                    break;
                default:
                    buffer[count++] = '%';
                    buffer[count++] = *p;
                    break;
            }
        } else {
            buffer[count++] = *p;
        }
        p++;
    }
    buffer[count] = '\0';
    va_end(args);
    return count;
}

// Implementação da função findPath
char *findPath(char *cmd, char **env) {
    char *path_env = NULL;
    char *path;
    char *full_path;
    char *paths[256];
    int i = 0;
    char *token;

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

    token = ft_strtok(path_env, ":");
    while (token != NULL && i < 256) {
        paths[i] = ft_strdup(token);
        if (!paths[i]) {
            perror("malloc");
            exit(1);
        }
        i++;
        token = ft_strtok(NULL, ":");
    }
    paths[i] = NULL;

    for (int j = 0; paths[j] != NULL; j++) {
        full_path = (char *)malloc(ft_strlen(paths[j]) + ft_strlen(cmd) + 2);
        if (!full_path) {
            perror("malloc");
            exit(1);
        }
        ft_strcpy(full_path, paths[j]);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, cmd);

        if (access(full_path, X_OK) == 0) {
            for (int k = 0; k < i; k++) {
                free(paths[k]);
            }
            return full_path; // Caminho encontrado
        }
        free(full_path);
    }

    for (int k = 0; k < i; k++) {
        free(paths[k]);
    }
    return NULL; // Comando não encontrado
}

// Função para dividir uma string em argumentos
char **parseArguments(char *input) {
    char **args = (char **)malloc(256 * sizeof(char *));
    if (!args) {
        perror("malloc");
        exit(1);
    }

    int i = 0;
    char *token = ft_strtok(input, " ");
    while (token != NULL && i < 255) {
        args[i] = ft_strdup(token);
        if (!args[i]) {
            perror("malloc");
            exit(1);
        }
        i++;
        token = ft_strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}

// Função para executar comandos com múltiplos pipes
void executeCommand(char *command, char **envp) {
    char *tokens[256];
    int num_tokens = 0;
    char *token = ft_strtok(command, "|");
    
    while (token != NULL && num_tokens < 256) {
        tokens[num_tokens++] = ft_strdup(token);
        token = ft_strtok(NULL, "|");
    }
    tokens[num_tokens] = NULL;

    int fd[2];
    int prev_fd = -1;

    for (int i = 0; i < num_tokens; i++) {
        if (pipe(fd) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) { // Processo filho
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < num_tokens - 1) {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            close(fd[1]);

            char *path = findPath(ft_strtok(tokens[i], " "), envp);
            if (path) {
                char **args = parseArguments(tokens[i]);
                execvp(path, args); // Usar execvp para passar argumentos
                perror("execvp");
                free(path);
                exit(1);
            } else {
                fprintf(stderr, "Comando não encontrado\n");
                exit(1);
            }
        } else { // Processo pai
            close(fd[1]);
            if (prev_fd != -1) {
                close(prev_fd);
            }
            prev_fd = fd[0];
        }
    }

    // Fechar o último fd e esperar pelos filhos
    if (prev_fd != -1) {
        close(prev_fd);
    }

    for (int i = 0; i < num_tokens; i++) {
        free(tokens[i]);
    }

    while (wait(NULL) > 0);
}

// Função principal
int main(int argc, char **argv, char **envp) {
    char *input;

    while (1) {
        // Lê a entrada do usuário
        input = readline("\033[32mminishell \033[0m");
        if (!input) {
            perror("readline");
            exit(1);
        }

        // Se o usuário digitar "exit", encerra o shell
        if (ft_strcmp(input, "exit") == 0) {
            printf("Saindo do minishell.\n");
            free(input);
            exit(0);
        }

        // Executa o comando com suporte a múltiplos pipes
        executeCommand(input, envp);

        // Libera a memória alocada pela readline
        free(input);
    }

    return 0;
}
