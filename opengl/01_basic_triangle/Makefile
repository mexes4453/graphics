
#=== COLORS ====
COL_Y = "\033[1;32m"
COL_G = "\033[1;33m"
COL_D = "\033[0m"

DIR_PROJECT = $(shell pwd)
DIR_SRC = $(DIR_PROJECT)/src
DIR_OBJ = $(DIR_PROJECT)/obj
DIR_DEP = $(DIR_PROJECT)/dep

SRCS_FILES_CPP:= main.cpp
SRCS_FILES_C:= glad.c

OBJ_FILES:= $(SRCS_FILES_CPP:.cpp=.o) $(SRCS_FILES_C:.c=.o)
$(info $(OBJ_FILES))
OBJS := $(patsubst %, $(DIR_OBJ)/%, $(OBJ_FILES))
SRCS_C := $(patsubst %, $(DIR_SRC)/%, $(SRCS_FILES_C))
SRCS_CPP := $(patsubst %, $(DIR_SRC)/%, $(SRCS_FILES_CPP))
#$(info $(OBJ_FILES))
#$(info $(OBJS))


DEP_FILES:= $(OBJS:.o=.d)
DEPS := $(patsubst %, $(DIR_DEP)/%, $(OBJ_FILES))

MSG = 
REP = mexes

NAME = prog.out

CC = g++
GCC = gcc
STD = c++98
DEBUG = 
VALGRIND =
VAL =
USR_LIB_PRINTF = libftprintf.a
USR_LIB_PATH_PRINTF= ./ft_printf

# FLAGS
CFLAGS = -Werror -Wall -Wextra -g -pthread -lrt -lSDL2 -ldl -lGL#-std=$(STD) 
LIBFLAGS_STATIC = #-L$(USR_LIB_PATH_PRINTF) -lftprintf
#CFLAGS += -D__thread__

DEPSFLAG =: -MM $($@:.o=.d)

# INCLUDE
INCLUDES = -I./ -I./inc #-I$(USR_LIB_PATH_PRINTF) 

#=== DEBUG ====
ifeq ($(DEBUG), 1)
	CFLAGS := $(CFLAGS) -g -D_DEBUG_=1
endif

ifeq ($(VALGRIND), 1)
	VAL = valgrind
endif
	

NAME : all
all : $(NAME) 
$(NAME) : $(OBJS) 
	@echo "\033[1;33mCompiling Executables: $(NAME) \033[0m"
	$(CC) $^ $(LIBFLAGS_STATIC) $(CFLAGS) $(INCLUDES) -o $@ -Wl,-Map=$(NAME).map
	@echo; echo "\033[1;32mCompilation Successful. \033[0m"
	@echo; echo


-include $(DEPS)

# obj files output
$(DIR_OBJ)/%.o : $(DIR_SRC)/%.cpp
	@mkdir -p $(dir $@) 
	@mkdir -p $(DIR_DEP) 
	@echo
	@echo "\033[1;33mCompiling OBJ files \033[0m"
	$(CC) -c $^ $(CFLAGS) $(INCLUDES) -o $@
	$(CC) -MM $^ $(CFLAGS) -o $*.d
	@mv $*.d $(DIR_DEP)

# obj files output
$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	@mkdir -p $(dir $@) 
	@mkdir -p $(DIR_DEP) 
	@echo
	@echo "\033[1;33mCompiling OBJ files \033[0m"
	$(GCC) -c $^ $(CFLAGS) $(INCLUDES) -o $@
	$(GCC) -MM $^ $(CFLAGS) -o $*.d
	@mv $*.d $(DIR_DEP)
#	@mv *.d $(DIR_DEP)
	@echo




# remove all object files
fclean:
	rm -rf $(DIR_OBJ) $(DIR_DEP) *.map

# remove final target files
clean: fclean
	rm -rf $(NAME)

# recompile everything
re: clean all

#=================== GIT ===============================
#define comments
push:	clean
	$(info Pushing to Git Repository)
ifeq ($(REP), 42)
	@git push wolfsburg main
	@echo $(COL_G)Pushed to repo: $(REP)$(COL_D)
#	@git rm -f --cached *.pdf *.md			#	2> /dev/null
else 
	@git add ../*
	@git ls-files; git status
	@git commit -m "$(MSG)"
	@git push mexes main
	@echo $(COL_G)Pushed to repo: mexes$(COL_D)
	@echo $(COL_G)$(MSG)$(COL_D)
endif

#endef # comments


# ========== TEST ====================
test:
	@echo
#	@echo $(COL_G)=== TEST === 1 = NO ARGS$(COL_D)
#	@$(VAL) ./$(NAME)
.PHONY : all fclean clean re push test
