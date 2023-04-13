# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htam <htam@student.42berlin.de>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/09 16:04:35 by htam              #+#    #+#              #
#    Updated: 2023/04/10 14:51:44 by htam             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= minishell

CFLAGS	:= -g3 -fsanitize=address -Wall -Wextra -Werror
CC		:= @cc
RM		:= rm -rf

SCRS			= main.c parser.c parser2.c parser3.c\
				builtins.c echo.c utility.c utility2.c\
				utility3.c utility_lst.c cd_pwd.c signal.c\
				env_getset.c utility_envl.c\
				env_export_unset.c env_convert.c\
				pipex.c get_path.c\
				lexer.c lexer_redir.c lexer_str.c\
				lex_merger.c interp_helper.c\
				mini_lexer.c\

# Libft
LIBFT_PATH		= libft/
LIBFT_NAME		= libft.a
LIBFT			= $(LIBFT_PATH)$(LIBFT_NAME)

INC				= -I ./includes
LIBRARY			= -Llibft -lft -lreadline

#all:	$(LIBFT) $(NAME)

$(NAME): $(SCRS) $(LIBFT)
		@echo "Compiling Minishell..."
		$(CC) $(CFLAGS) $(INC) $(SCRS) $(LIBRARY) -o $(NAME)
		@echo "Minishell compiled!"

$(LIBFT):
		@echo "Compiling libft..."
		@make -sC $(LIBFT_PATH)
		@echo "libft compiled!"

all:	$(LIBFT) $(NAME)

clean:
		$(RM) $(NAME)

fclean:
		make fclean -sC $(LIBFT_PATH)
		$(RM) $(NAME)

re:		fclean all

.PHONY: all clean fclean re