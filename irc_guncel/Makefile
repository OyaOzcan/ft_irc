CPP = c++
FLAGS = -std=c++98 -Wall -Werror -Wextra
NAME = ircserv

SRC_DIR = ./
OBJ_DIR = obj/

SRC = Server/Status.cpp Server/Message.cpp Server/Server.cpp Server/Utils.cpp \
      Client/Client.cpp Commands/Nick.cpp Commands/User.cpp Commands/Pass.cpp Commands/Join.cpp \
      Commands/Part.cpp Commands/Kick.cpp Commands/Quit.cpp Commands/Who.cpp Commands/List.cpp \
      Commands/Privmsg.cpp Commands/Invite.cpp Channel/Channel.cpp Commands/Topic.cpp Commands/Mode.cpp main.cpp

OBJ = $(SRC:%.cpp=$(OBJ_DIR)%.o)

RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m
BLUE = \033[0;34m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "${GREEN}Linking...${NC}"
	@$(CPP) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "${GREEN}Compilation successful!${NC}"

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo "${BLUE}Compiling $<...${NC}"
	@$(CPP) $(FLAGS) -c $< -o $@

clean:
	@echo "${RED}Cleaning object files...${NC}"
	@rm -rf $(OBJ_DIR)
	@echo "${RED}Cleaned.${NC}"

fclean: clean
	@echo "${RED}Removing executable...${NC}"
	@rm -rf $(NAME)
	@echo "${RED}Executable removed.${NC}"

re: fclean all

.PHONY: clean all re fclean
