#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdarg.h>
#include "../src/libft/libft.h"

#ifndef MINISHELL_H
#define MINISHELL_H

typedef struct Node
{
    char* data;
    struct Node* next;
} Node;

char	*ft_substr(const char *s, unsigned int start, size_t len);
//static size_t	ft_substring(const char *s, char c);
int ft_strcmp(const char *s1, const char *s2);
char	**ft_split(const char *s, char c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
size_t ft_strlen(const char *str);
char *ft_strdup(const char *str);
char *ft_strcat(char *dest, const char *src);
char *ft_strcpy(char *dest, const char *src);
char *ft_strncpy(char *dest, const char *src, size_t n);
char *ft_strtok(char *str, const char *delim);
int ft_sprintf(char *buffer, const char *format, ...);
char *findPath(char *cmd, char **env);
char **parseArguments(char *input);
void executeCommand(char *command, char **argv, char **envp);

#endif