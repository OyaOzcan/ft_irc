CC = c++ -std=c++98
FLAG = -Wall -Wextra -Werror
Name = ft_irc
SRC = main.cpp server.cpp

all: $(Name)

$(Name): $(SRC)
	@$(CC) $(FLAG) $(SRC) -o $(Name)

clean:
	@rm -rf $(Name)

fclean: clean

re: fclean all

.PHONY: all clean fclean re