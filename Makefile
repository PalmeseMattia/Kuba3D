NAME := Cube
CFLAGS := -Wextra -Wall -Werror -g # -O0
LIBLIBFT := ./lib/libft
HEADERS := -I ./includes/ -I $(LIBLIBFT) -I ./includes/cube/mock/ -I ./includes/cube/parsing/ -I ./includes/cube/utils/ -I ./includes/cube/controls/ -I ./includes/cube/entities/ -I ./includes/cube/map/ -I ./includes/cube/mlx_handler/ -I ./includes/cube/runtime_handler/ -I ./includes/cube/runtime_handler/ -I ./includes/cube/settings/ -I ./includes/cube/dda/ -I ./includes/cube/drawing/ -I ./includes/cube/lifecycle/  -I ./includes/cube/animations/

# OS detection
UNAME_S := $(shell uname -s)

# Linux configuration
ifeq ($(UNAME_S),Linux)
    LIBMLX := ./lib/mlx_linux
    LIBS := $(LIBMLX)/libmlx.a -lXext -lX11 -lm $(LIBLIBFT)/libft.a
    HEADERS += -I $(LIBMLX) -I /usr/include/X11
endif

# macOS configuration
ifeq ($(UNAME_S),Darwin)
    LIBMLX := ./lib/mlx_mac
    # Check for Apple Silicon
    UNAME_M := $(shell uname -m)
    ifeq ($(UNAME_M),arm64)
        # Apple Silicon
        GLFW_LIB = -L"/opt/homebrew/Cellar/glfw/3.4/lib/"
    else
        # Intel Mac
        GLFW_LIB = -L"/usr/local/Cellar/glfw/3.4/lib/"
    endif
    LIBS := $(LIBMLX)/libmlx.a -framework Cocoa -framework OpenGL -framework IOKit -ldl -lglfw -pthread -lm $(GLFW_LIB) $(LIBLIBFT)/libft.a
    HEADERS += -I $(LIBMLX) -I /usr/X11/include
    X11_FLAGS = -L/usr/X11/lib -lXext -lX11
endif

SRCS := $(shell find ./src/ -iname "*.c")
OBJS := ${SRCS:.c=.o}

all: liblibft libmlx $(NAME)

liblibft:
	@cd $(LIBLIBFT) && make

libmlx:
	@cd $(LIBMLX) && make

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) $(X11_FLAGS) -o $(NAME)
else
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
endif

clean:
	@rm -rf $(OBJS)
	@cd $(LIBLIBFT) && make clean
	@cd $(LIBMLX) && make clean

fclean: clean
	@rm -rf $(NAME)
	@rm -f $(LIBLIBFT)/libft.a

re: clean all

.PHONY: all clean fclean re libmlx liblibft