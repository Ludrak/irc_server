NAME=ft_irc

# Locations 
SRC_DIR		= srcs
INC_DIR		= includes
BIN_DIR		= bin

# Sources
SRCS		= main.cpp
HEADERS		= 


SRC_FILES=$(shell find $(SRC_DIR) | grep -E '$(shell echo $(SRCS) | tr ' ' '|')')
OBJS=$(addprefix $(BIN_DIR)/, $(SRC_FILES:.cpp=.o))


LIBRARY_LOCATION=lib
LIBRARYS=

CLANG=clang++
CPP_FLAGS=-Wextra -Wall -Werror -std=c++98
CPP_IFLAGS=$(addprefix -I,$(INC_DIR))
CPP_LFLAGS=$(addprefix -L,$(addprefix $(LIBRARY_LOCATION), $(LIBRARYS)))

PREFIX_PROJECT=[\033[1;32m$(NAME)\033[0m]
PREFIX_COMP=[\033[1;32m✔\033[0m]
PREFIX_LINK=[\033[1;32mLINK\033[0m]
PREFIX_WARN=[\033[0;33mWARN\033[0m]
PREFIX_CLEAN=[\033[1;31mCLEAN\033[0m]

all: $(NAME)
	@echo "$(PREFIX_PROJECT) Nothing else to do"

$(NAME): $(BIN_DIR) $(OBJS)
	@$(CLANG) $(OBJS) -o $(NAME) $(CPP_FLAGS) $(CPP_LFLAGS)
	@echo "$(PREFIX_PROJECT)$(PREFIX_LINK) Linking done for: $(NAME)"

$(BIN_DIR):
	@echo "$(PREFIX_PROJECT)$(PREFIX_WARN) No bin dir found. Creating one."
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(SRC_DIR)/%.o : $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(BIN_DIR)/$(shell dirname $<)
	@$(CLANG) -c $< -o $@ $(CPP_FLAGS) $(CPP_IFLAGS)
	@printf "$(PREFIX_PROJECT)$(PREFIX_COMP) Compiled: $(shell basename $<) \n"

clean:
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning $(BIN_DIR)/"
	@rm -rf $(BIN_DIR)

fclean: clean
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
