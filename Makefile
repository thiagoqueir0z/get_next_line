NAME = gnl_test

SRCS = get_next_line.c get_next_line_utils.c main.c

BONUS = get_next_line_bonus.c get_next_line_utils_bonus.c main.c

OBJS = $(SRCS:.c=.o)

OBJ_BONUS = $(BONUS:.c=.o)

HEADER = get_next_line.h

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Mudar valor para testar diferentes tamanhos de buffer

BUFFER_SIZE = 42

#----------------- RULES -----------------

all: $(NAME)

$(NAME): $(OBJS)
# O compilador junta todos os objetos e define o BUFFER_SIZE
	$(CC) $(CFLAGS) $(OBJS) -D BUFFER_SIZE=$(BUFFER_SIZE) -o $(NAME)

%.o: %.c $(HEADER)
# Compila cada arquivo .c, incluindo o header e definindo o BUFFER_SIZE
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -c $< -o $@

bonus: .bonus

.bonus: $(OBJS) $(OBJ_BONUS)

clean:
	rm -f $(OBJS) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME)
	rm -f .bonus

re: fclean all

#----------------- REGRA DE TESTE -----------------

TEST_FILE = test_file.txt

test: all
	@echo "\n--- GNL TESTE | BUFFER_SIZE = $(BUFFER_SIZE) ---"
@echo "1. Gerando um arquivo de teste: $(TEST_FILE)"
	@echo "A linha 1.\nA linha 2 (curta).\nA linha 3 e um pouco maior para testar.\n" > $(TEST_FILE)
@echo "2. Executando o teste:"
# O executável gnl_test será chamado com o arquivo de teste como argumento
	./$(NAME) $(TEST_FILE)
	@echo "\n--- FIM DO TESTE ---"

memcheck: all
	@echo "\n--- GNL TESTE DE MEMÓRIA (VALGRIND) | BUFFER_SIZE = $(BUFFER_SIZE) ---"
	@echo "1. Gerando/Atualizando arquivo de teste..."
	@echo -e "A linha 1.\nA linha 2 (curta).\nA linha 3 e um pouco maior para testar.\nLinha Final Sem Newline." > $(TEST_FILE)
@echo "2. Executando Valgrind (Verificação de Erros de Acesso e Leaks):"
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --errors-for-leak-kinds=all --error-exitcode=1 ./$(NAME) $(TEST_FILE)
	@echo "\n--- FIM DO RELATÓRIO VALGRIND ---"

.PHONY: all clean fclean re test memcheck