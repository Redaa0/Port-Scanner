NAME = port_scanner
CFLAGS = -O2 -fsanitize=thread -std=c++17 -pthread


SRC = classes.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re