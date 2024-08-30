#include "../inc/minishell.h"

char    *printCommand(char *input, char **argv, char **envp)
{
    input = readline("\033[32mminishell \033[0m");
        if (input == NULL)
	    {
            perror("readline");
            exit(EXIT_FAILURE);
        }
        add_history(input);
        if (ft_strcmp(input, "exit") == 0)
	    {
            printf("Saindo do loop.\n");
            free(input);
            exit(1);
        }
	    if (ft_strcmp(input, "ls") == 0)
		    execve("/bin/ls", &argv[1], NULL);
        if (ft_strcmp(input, "env") == 0)
        {
            printEnv(envp);
        }
        if (ft_strcmp(input, "clear") == 0)
            execve("/usr/bin/clear", &argv[1], envp);
        printf("Você digitou: %s\n", input);
        free(input);

        return (input);
}