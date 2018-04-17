UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	GCC_FLAG = -Wextra -Wall
	DL_INC = -I./include
	DL_FLAG_DIR =
	DL_FLAG = -lGLU -lGL -lglfw -lGLEW -lGLU -lGL -ldl -lm -lpthread -lXxf86vm -lX11 -lassimp -lnoise -lOpenCL
endif
ifeq ($(UNAME), Darwin)
	GCC_FLAG = -Werror -Wextra -Wall
	DL_INC = -I./include
	DL_FLAG_DIR =
	DL_FLAG = -L $(HOME)/.brew/lib -framework OPENCL -framework OpenGL -lglfw -lGLEW -lpthread -lassimp  -lnoise
endif

CC = g++ $(GCC_FLAG) $(DL_INC)

NAME = opencl

dir_guard=@mkdir -p $(@D)

SRC =  	main.cpp \
		Ocl.cpp

SRCDIR = ./src
OBJDIR = ./obj

SRCS=$(addprefix $(SRCDIR),$(SRC))
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(addprefix $(OBJDIR)/, %.o): $(addprefix $(SRCDIR)/, %.cpp)
	$(dir_guard)
	@$(CC)  -o $@ -c $^

$(NAME): $(OBJS)
	@$(CC) -o $@ $^  $(DL_FLAG)
	@echo "[$@] Complete"

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
