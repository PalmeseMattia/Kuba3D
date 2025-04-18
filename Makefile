NAME	:= Cube
CFLAGS	:= -Wextra -Wall -Werror
LIBMLX	:= ./lib/mlx
LIBLIBFT := ./lib/libft

# Apple Silicon
APPLE_SILICON_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit
GLFW_LIB = -L"/opt/homebrew/Cellar/glfw/3.4/lib/"
# / Apple Silicon

HEADERS	:=  -I ./includes/ -I $(LIBLIBFT) -I $(LIBMLX)/ -I ./includes/cube/mock/ -I ./includes/cube/parsing/ -I ./includes/cube/utils/ -I ./includes/cube/graphics/ -I ./includes/cube/controls/
LIBS	:= $(LIBMLX)/libmlx.a -ldl -lglfw -pthread -lm $(GLFW_LIB) $(LIBLIBFT)/libft.a
SRCS	:= $(shell find ./src -iname "*.c")
OBJS	:= ${SRCS:.c=.o}
X11_FLAGS = -L/usr/X11/lib -lXext -lX11

all: liblibft libmlx $(NAME)

liblibft:
	@cd $(LIBLIBFT) && make 

libmlx:
	cd $(LIBMLX) && make

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)"

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) $(X11_FLAGS) -o $(NAME)

clean:
	@rm -rf $(OBJS)
	@cd $(LIBLIBFT) && make clean
	@cd $(LIBMLX) && make clean

fclean: clean
	@rm -rf $(NAME)
	@rm $(LIBLIBFT)/libft.a

re: clean all

.PHONY: all, clean, fclean, re, libmlx