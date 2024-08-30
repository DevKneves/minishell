#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "../src/libft/libft.h"

#ifndef MINISHELL_H
#define MINISHELL_H

typedef struct Node
{
    char* data;
    struct Node* next;
} Node;

Node* createNode(char* data);
void insertNode(Node** head, char* data);
void freeList(Node* head);
char *printEnv(char **envp);
char	ft_exec(char **argv, int i, int tmp_fd, char **env);
void	ft_error(char *err, char *arg);
char    *printCommand(char *input, char **argv, char **envp);
char    *findPath(char **env);

#endif