SRCS = ./Mandatory/main.c
OBJBS = $(SRCS:.c=.o)
NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address   #thread -g 

all : $(NAME)

$(NAME) : $(OBJBS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJBS)
	@echo "\033[1;32mthe Mandatory target has been mad with success\033[0m"

%.o : %.c ./Mandatory/philo.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJBS)
	@echo "\033[1;31mthe Mandatory part object files had been deleted\033[0m"

fclean : clean
	rm -rf $(NAME)
	@echo "\033[1;31mthe Mandatory part target has been deleted\033[0m"
re : fclean all