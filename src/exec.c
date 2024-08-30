#include "../inc/minishell.h"

char *ft_strtok(char *str, const char *delim)
{
    static char *save_ptr;
    char *token;

    if (str == NULL)
        str = save_ptr;
    if (str == NULL)
        return NULL;
    while (*str && strchr(delim, *str))
        str++;
    if (*str == '\0')
        return NULL;
    token = str;
    while (*str && !strchr(delim, *str))
        str++;
    if (*str)
    {
        *str = '\0';
        save_ptr = str + 1;
    }
    else
        save_ptr = NULL;
    return (token);
}

char *findPath(char *cmd, char **env) {
    char *path_env = NULL;
    char *path;
    char *full_path;
    char *paths[256];
    int i = 0;
    char *token;

    while (*env)
    {
        if (ft_strncmp(*env, "PATH=", 5) == 0)
        {
            path_env = *env + 5;
            break;
        }
        env++;
    }
    if (!path_env) 
        return NULL;
    token = ft_strtok(path_env, ":");
    while (token != NULL && i < 256)
    {
        paths[i] = ft_strdup(token);
        if (!paths[i])
        {
            perror("malloc");
            exit(1);
        }
        i++;
        token = ft_strtok(NULL, ":");
    }
    paths[i] = NULL;
    for (int j = 0; paths[j] != NULL; j++)
    {
        full_path = (char *)malloc(ft_strlen(paths[j]) + ft_strlen(cmd) + 2);
        if (!full_path)
        {
            perror("malloc");
            exit(1);
        }
        ft_strcpy(full_path, paths[j]);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, cmd);
        if (access(full_path, X_OK) == 0)
        {
            for (int k = 0; k < i; k++)
                free(paths[k]);
            return full_path;
        }
        free(full_path);
    }
    for (int k = 0; k < i; k++)
        free(paths[k]);
    return NULL;
}
char **parseArguments(char *input)
{
    char **args;
    int i;
    char *token = ft_strtok(input, " ");

    if (!args)
    {
        perror("malloc");
        exit(1);
    }
    i = 0;
    args = (char **)malloc(256 * sizeof(char *));
    while (token != NULL && i < 255)
    {
        args[i] = ft_strdup(token);
        if (!args[i])
        {
            perror("malloc");
            exit(1);
        }
        i++;
        token = ft_strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

void executeCommand(char *command, char **argv, char **envp)
{
    char *tokens[256];
    int num_tokens = 0;
    char *token = ft_strtok(command, "|");
    int fd[2];
    int prev_fd = -1;
    
    while (token != NULL && num_tokens < 256)
    {
        tokens[num_tokens++] = ft_strdup(token);
        token = ft_strtok(NULL, "|");
    }
    tokens[num_tokens] = NULL;
    for (int i = 0; i < num_tokens; i++)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(1);
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < num_tokens - 1)
                dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            char *path = findPath(ft_strtok(tokens[i], " "), envp);
            if (path)
            {
                char **args = parseArguments(tokens[i]);
                execve(path, argv, args);
                perror("execvp");
                free(path);
                exit(1);
            } 
            else 
            {
                fprintf(stderr, "Comando não encontrado\n");
                exit(1);
            }
        } 
        else 
        {
            close(fd[1]);
            if (prev_fd != -1) {
                close(prev_fd);
            }
            prev_fd = fd[0];
        }
    }
    if (prev_fd != -1)
        close(prev_fd);
    for (int i = 0; i < num_tokens; i++)
        free(tokens[i]);
    while (wait(NULL) > 0);
}