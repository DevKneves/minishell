#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Implementação da função ft_strncmp e ft_strcmp (já fornecidas)
// Não precisamos alterar essas funções.

// Função para encontrar o caminho do comando
int ft_strcmp(const char *s1, const char *s2)
{
    unsigned char *p1;
    unsigned char *p2;

    p1 = (unsigned char *)s1;
    p2 = (unsigned char *)s2;

    while (*p1 && (*p1 == *p2))
    {
        p1++;
        p2++;
    }

    return (*p1 - *p2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;
	size_t			i;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	i = 0;
	while ((p1[i] || p2[i]) && i < n)
	{
		if (p1[i] != p2[i])
			return (p1[i] - p2[i]);
		i++;
	}
	return (0);
}

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

int main(int argc, char **argv, char **envp) {
    char *input;
    int fd[2];
    int status;
    char *command;
    (void)argc;

    while (1) {
        input = readline("\033[32mminishell \033[0m");
        if (!input) {
            perror("readline");
            exit(1);
        }

        if (ft_strcmp(input, "exit") == 0) {
            printf("Saindo do loop.\n");
            free(input);
            exit(0);
        }
            if (pipe(fd) == -1) {
                perror("pipe");
                free(input);
                exit(1);
            }

            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                free(input);
                exit(1);
            }

            if (pid == 0)
            { // Processo filho
                close(fd[0]); // Fechar o lado de leitura
                dup2(fd[1], STDOUT_FILENO); // Redirecionar STDOUT para o pipe
                close(fd[1]);

                char *path = findPath(input, envp);
                if (path) {
                    execl(path, input, NULL); // Usar execl para simplificar
                    perror("execl"); // execl deve falhar se o comando não for executado
                    free(path);
                } else {
                    fprintf(stderr, "Comando não encontrado\n");
                    exit(1);
                }
            } 
            else
            { // Processo pai
                close(fd[1]); // Fechar o lado de escrita
                char buffer[1024];
                ssize_t bytes_read;

                while ((bytes_read = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    printf("%s", buffer);
                }
                close(fd[0]); // Fechar o fd após o uso

                // Esperar pelo processo filho
                waitpid(pid, &status, 0);
            }
        }
        free(input); // Liberar a memória alocada por readline
    return 0;
}
