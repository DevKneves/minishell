SRC = src/libft/ft_strcmp.c src/main.c  parse/printEnv.c src/exec.c src/printCommand.c

NAME = minishell
OBJ = $(SRC:.c=.o)
CC = cc
RM = rm -rf
CFLAGS =  -Wall -Wextra -Werror

Reset = \033[0m
G = \033[32m
R = \033[31m
Y = \033[33m
blink = \033[6;7;1;3m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline
	@echo "$(G)"
	#-@timeout 3 curl ascii.live/rick
	@echo "-------------------------------------------------------------------------------------"
	@echo "$(blink)$(G) ✅ $(NAME) successfully compiled $(Reset)"
	@echo "$(G)-------------------------------------------------------------------------------------$(Reset)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

val:
	valgrind --leak-check=full ./minishell ls

normalclean:
	-@$(RM) $(OBJ)

clean: normalclean
	-@$(RM) $(OBJ)
	@echo "$(Y)-------------------------------------------------------------------------------------"
	@echo " 😉 Objects from the $(NAME) project have been removed"
	@echo "-------------------------------------------------------------------------------------$(Reset)"

fclean: normalclean
	@echo "$(R)"
	#-@timeout 2 curl ascii.live/forrest
	@$(RM) $(NAME)
	@echo "-------------------------------------------------------------------------------------"
	@echo "$(blink)$(R) ❌ removed $(NAME) executable $(Reset)"
	@echo "$(R)-------------------------------------------------------------------------------------$(Reset)"

re: normalclean all

.PHONY: all clean fclean re
