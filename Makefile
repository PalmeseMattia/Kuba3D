NAME	:= Cube
CFLAGS	:= -Wextra -Wall -Werror
LIBMLX	:= ./lib/mlx
LIBLIBFT := ./lib/libft

# Apple Silicon
APPLE_SILICON_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit
GLFW_LIB = -L"/opt/homebrew/Cellar/glfw/3.4/lib/"
# / Apple Silicon

HEADERS	:=  -I ./includes/ -I $(LIBLIBFT) -I $(LIBMLX)/include -I ./includes/cube/mock/ -I ./includes/cube/parsing/
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm $(GLFW_LIB) $(LIBLIBFT)/libft.a
SRCS	:= $(shell find ./src -iname "*.c")
OBJS	:= ${SRCS:.c=.o}

all: liblibft libmlx $(NAME)

liblibft:
	@cd $(LIBLIBFT) && make 

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)"

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(LIBMLX)/build
	@cd $(LIBLIBFT) && make clean

fclean: clean
	@rm -rf $(NAME)
	@rm $(LIBLIBFT)/libft.a

re: clean all

.PHONY: all, clean, fclean, re, libmlx