#include "../inc/minishell.h"

char *printEnv(char **envp)
{
    // Variáveis para percorrer o array de variáveis de ambiente
    char **env = envp;

    // Percorrer o array de variáveis de ambiente
    while (*env) {
        // Imprimir cada variável de ambiente
        printf("%s\n", *env);
        // Passar para o próximo elemento do array
        env++;
    }
    return (*env);
}