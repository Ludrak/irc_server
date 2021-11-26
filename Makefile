.PHONY: clean fclean re all check_sources check_headers

# Name of target executable
NAME		= ft_irc

# Locations 
SRC_DIR		= srcs
INC_DIR		= includes
BIN_DIR		= bin
LIB_DIR		= lib

# Sources & Headers 
# - fill only with name of the file
# - make will check for the file in SRC_DIR
# - use "-" if empty
SRCS		=	SockStream.cpp \
				SockHandler.cpp \
				SockManager.cpp \
				AClient.cpp \
				AServer.cpp \
				ANode.cpp \
				CommonInfo.cpp \
				ClientInfo.cpp \
				ServerInfo.cpp \
				ChannelInfo.cpp \
				UnRegisteredConnectionInfo.cpp \
				AEntity.cpp \
				NetworkEntity.cpp \
				RelayedEntity.cpp \
				UnRegisteredConnection.cpp \
				Client.cpp \
				Server.cpp \
				Channel.cpp \
				Package.cpp \
				IRCServer.cpp \
				Protocol.cpp \
				IRCProtocol.cpp \
				Parser.cpp \
				Logger.cpp \
				main.cpp	

HEADERS		=	SockStream.hpp \
				SockHandler.hpp \
				SockManager.hpp \
				AServer.hpp \
				AClient.hpp \
				ANode.hpp \
				CommonInfo.hpp \
				ClientInfo.hpp \
				ServerInfo.hpp \
				ChannelInfo.hpp \
				AEntity.hpp \
				NetworkEntity.hpp \
				RelayedEntity.hpp \
				UnRegisteredConnection.hpp \
				Client.hpp \
				Server.hpp \
				Channel.hpp \
				IRCServer.hpp \
				Package.hpp \
				IProtocol.hpp \
				Protocol.hpp \
				IRCProtocol.hpp \
				Parser.hpp \
				Logger.hpp \
				status.hpp \
				ntos.hpp

COMMANDS_SRCS		=	CommandHandler.cpp \
						ACommand.cpp \
						CommandPass.cpp \
						CommandNick.cpp \
						CommandUser.cpp \
						CommandPrivmsg.cpp \
						CommandJoin.cpp \
						CommandMode.cpp \
						CommandQuit.cpp \
						CommandSquit.cpp \
						CommandOper.cpp \
						CommandMotd.cpp \
						CommandConnect.cpp \
						CommandDie.cpp \
						CommandPong.cpp \
						CommandError.cpp
					
COMMANDS_HEADERS	=	CommandHandler.hpp \
						ACommand.hpp \
						CommandPass.hpp \
						CommandNick.hpp \
						CommandUser.hpp \
						CommandPrivmsg.hpp \
						CommandJoin.hpp \
						CommandMode.hpp \
						CommandQuit.hpp \
						CommandSquit.hpp \
						CommandOper.hpp \
						CommandMotd.hpp \
						CommandConnect.hpp \
						CommandDie.hpp \
						CommandPong.hpp \
						CommandError.hpp
# CommandNjoin.hpp

# Librarys (only for local archives in project folder)
LIBRARYS	= 

# Compiler and flags (include and library include are autogenerated if they are local)
OPENSSL_PATH= $(shell brew --prefix openssl)

CLANG		=	clang++
CPP_FLAGS	=	-Wextra -Wall -Werror -std=c++98 -g3 -fsanitize=address
CPP_IFLAGS	=	-I $(OPENSSL_PATH)/include 

CPP_LFLAGS	= -lssl -lcrypto \
				-L  $(OPENSSL_PATH)/lib/ 

# Fancy prefixes 
PREFIX_PROJECT=[\033[1;32m$(NAME)\033[0m]
PREFIX_COMP=\033[1;30m-\033[0m-\033[1;37m>\033[0m[\033[1;32m✔\033[0m]
PREFIX_LINK=[\033[1;32mLINK\033[0m]
PREFIX_WARN=[\033[0;33mWARN\033[0m]
PREFIX_DUPL=[\033[1;33mDUPLICATES\033[0m]
PREFIX_CLEAN=[\033[1;31mCLEAN\033[0m]


############################################################################################


SRC_FILES	=	$(shell find $(SRC_DIR) | grep -E '$(shell echo $(SRCS) $(COMMANDS_SRCS) | tr ' ' '|')')
HEADER_FILES=	$(shell find $(INC_DIR) | grep -E '$(shell echo $(HEADERS) $(COMMANDS_HEADERS) | tr ' ' '|')')
OBJS		=	$(addprefix $(BIN_DIR)/, $(SRC_FILES:.cpp=.o))
CPP_IFLAGS	+=	$(addprefix -I,$(INC_DIR)) $(addprefix -I,$(shell echo $(HEADER_FILES) | tr ' ' '\n' | rev | cut -d'/' -f2- | rev | sort | uniq))
CPP_LFLAG	+=	$(addprefix -L,$(addprefix $(LIB_DIR), $(LIBRARYS)))

#   Main rule
all: select
	@echo done

select: CPP_FLAGS += -DSELECT
select: $(NAME)
# select: clean $(NAME)

kqueue: CPP_FLAGS += -DKQUEUE
kqueue: clean $(NAME)
# kqueue: $(NAME)

poll: CPP_FLAGS += -DPOLL
poll: clean $(NAME)
# poll: $(NAME)

#	check_sources :
#	simple bash script to check duplicates sources files 
check_sources:
	@duplicates=$$( echo $(SRC_FILES) | tr ' ' '\n' | rev | cut -d'/' -f1 | rev | sort | uniq -c | sed 's/ //g' ) ; \
	error=0 ; \
	for source in $$duplicates ; do \
			if [ $$(echo $$source | sed 's/[^0-9]//g') -gt 1 ] ; then \
				echo "$(PREFIX_PROJECT)$(PREFIX_DUPL)" Duplicates source files found for \"$$(echo $$source | sed 's/[0-9]//g')\" in: ; \
				find $(SRC_DIR) | grep $$(echo $$source | sed 's/[0-9]//g') ; \
				echo ; \
				error=1 ; \
			fi \
	done ; \
	if [ $$error -eq 1 ] ; then \
		echo "Error: cannot manage duplicates files, aborting..." ; \
		exit 1 ; \
	fi

#	check_headers :
#	simple bash script to check duplicates header files 
check_headers:
	@duplicates=$$( echo $(HEADER_FILES) | tr ' ' '\n' | rev | cut -d'/' -f1 | rev | sort | uniq -c | sed 's/ //g' ) ; \
	error=0 ; \
	for source in $$duplicates ; do \
			if [ $$(echo $$source | sed 's/[^0-9]//g') -gt 1 ] ; then \
				echo "$(PREFIX_PROJECT)$(PREFIX_DUPL)" Duplicates header files found for \"$$(echo $$source | sed 's/[0-9]//g')\" in: ; \
				find $(INC_DIR) | grep $$(echo $$source | sed 's/[0-9]//g') ; \
				echo ; \
				error=1 ; \
			fi \
	done ; \
	if [ $$error -eq 1 ] ; then \
		echo "Error: cannot manage duplicates files, aborting..." ; \
		exit 1 ; \
	fi

#	Linking rule
$(NAME): check_headers check_sources $(BIN_DIR) $(OBJS)
	@$(CLANG) $(OBJS) -o $(NAME) $(CPP_FLAGS) $(CPP_LFLAGS)
	@echo "$(PREFIX_PROJECT)$(PREFIX_LINK) Linking done for: $(NAME)"

#	Bin directory
$(BIN_DIR):
	@echo "$(PREFIX_PROJECT)$(PREFIX_WARN) No bin dir found. Creating one."
	@mkdir -p $(BIN_DIR)

# Compilation rule 
$(BIN_DIR)/$(SRC_DIR)/%.o : $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@mkdir -p $(BIN_DIR)/$(shell dirname $<)
	@$(CLANG) -c $< -o $@ $(CPP_FLAGS) $(CPP_IFLAGS)
	@echo "$(PREFIX_PROJECT)$(PREFIX_COMP) Compiled: $(shell basename $<)"

# clean rule
clean:
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning $(BIN_DIR)/"
	@rm -rf $(BIN_DIR)

# final clean rule
fclean: clean
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning $(NAME)"
	@rm -f $(NAME)

# remake rule
re: fclean all
