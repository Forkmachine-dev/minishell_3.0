# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ali <ali@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/26 21:03:24 by ali               #+#    #+#              #
#    Updated: 2024/01/26 21:04:29 by ali              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

 #SRC =  $(shell find . -type f -name "*.c")

SRC = parsing/*.c main/*.c execution/*.c built_ins/*.c

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address 

all: 
	@$(MAKE) -C libft
	@cc $(CFLAGS) $(SRC) -o minishell -Llibft -lft -lreadline
	@echo "compiling minishell..."
fclean: 
	rm -f minishell
	@$(MAKE) -C ./libft 

re: fclean all 