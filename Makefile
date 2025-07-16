# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vgarcia <vgarcia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/22 18:42:56 by vincentgarc       #+#    #+#              #
#    Updated: 2025/07/16 09:36:43 by vgarcia          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv 

SRCFILE = main Config Server Location parsing parsingUtils running Client
SRCDIR= src/
SRCFORMAT=.cpp
SRCFILE := $(addprefix $(SRCDIR), $(SRCFILE))
SRCFILE := $(addsuffix $(SRCFORMAT), $(SRCFILE))
CXX = c++
INCLUDE = include
FLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INCLUDE) -MMD -g3

OBJ_DIR = .obj/

OBJ = $(addprefix $(OBJ_DIR), $(SRCFILE:.cpp=.o))

DEPS =  $(addprefix $(OBJ_DIR), $(SRCFILE:.cpp=.d))

$(OBJ_DIR)%.o : %$(SRCFORMAT)
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJ) Makefile
	$(CXX) $(FLAGS) $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean 
	rm -f $(NAME)

re : fclean all

-include $(DEPS)

.PHONY: all clean fclean re
