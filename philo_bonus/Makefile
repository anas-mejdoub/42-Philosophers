SRC = main_bonus.c end_simulation_bonus.c initialize_bonus.c parsing_bonus.c print_bonus.c routine_bonus.c simulation_bonus.c utils2_bonus.c utils_bonus.c

OBJ=$(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
NAME = philo_bonus
all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "\033[1;32mthe Mandatory bonus has been mad with success\033[0m"

%.o : %.c philo_bonus.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ)
	@echo "\033[1;31mthe Bonus part object files had been deleted\033[0m"

fclean : clean
	rm -rf $(NAME)
	@echo "\033[1;31mthe Bonus part target has been deleted\033[0m"

re :fclean all
