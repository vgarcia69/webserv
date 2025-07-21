NAME = webserv 

SRCFILE = main Config Server Location parsing parsingUtils running Client parsingLocation
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
