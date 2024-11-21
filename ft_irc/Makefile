NAME = ircserv

SRC = main.cpp \
	  src/server.cpp \

CC = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98


OBJ_DIR = src/obj
OBJS = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

RM = rm -fr

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	
re: fclean all

.PHONY: all clean fclean re