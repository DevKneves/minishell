#include "../inc/minishell.h"

int main(int argc, char **argv, char **envp)
{
    char *input;
    char **splited_input;

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

        splited_input = ft_split(input, ' ');
        // Executa o comando com suporte a múltiplos pipes
        executeCommand(input, splited_input, envp);

        // Libera a memória alocada pela readline
        free(input);
    }
    return 0;
}