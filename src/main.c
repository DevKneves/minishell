#include "src/exec.c"
#include "../inc/minishell.h"

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