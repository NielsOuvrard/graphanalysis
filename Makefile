##
## Makefile for cpp
##

SRC = *.cpp

NAME = game_of_graphs

# -Wall -Wextra -Werror

CMACFLAGS += -g -g3 -std=c++20

all:
	@ g++ -o $(NAME) $(SRC) $(CMACFLAGS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: $(NAME) clean fclean all re
