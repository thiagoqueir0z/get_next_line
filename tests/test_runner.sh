#!/bin/bash

# Define os arquivos fonte C (Assumindo que estão na pasta atual)
SRCS="get_next_line.c get_next_line_utils.c main.c"
EXEC="gnl_test"

# --- Funções de Ajuda ---

compile_and_run() {
    local b_size=$1
    local test_target=$2
    local test_file=$3

    echo -e "\n=================================================================="
    echo -e "⚙️  COMPILANDO: BUFFER_SIZE = $b_size"
    echo -e "=================================================================="

    # Compilação manual com o flag -D BUFFER_SIZE
    gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC

    if [ $? -ne 0 ]; then
        echo -e "\n❌ ERRO DE COMPILAÇÃO para BUFFER_SIZE = $b_size"
        exit 1
    fi
    
    echo -e "\n>>> EXECUTANDO TESTE: $test_target ($test_file) <<<\n"
    
    if [ "$test_target" == "STDIN" ]; then
        echo -e ">>> Por favor, digite algumas linhas (Ctrl+D para terminar):\n"
        ./$EXEC
    else
        ./$EXEC $test_file
    fi
    
    echo -e "\n--- FIM DO TESTE ---"
}

run_valgrind() {
    local b_size=$1

    echo -e "\n=================================================================="
    echo -e "🧠 TESTE DE MEMÓRIA (VALGRIND) | BUFFER_SIZE = $b_size"
    echo -e "=================================================================="
    
    # 1. Recompila com o tamanho crítico 1
    gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC
    
    if [ $? -ne 0 ]; then
        echo "❌ ERRO DE COMPILAÇÃO para Valgrind."
        exit 1
    fi
    
    # 2. Executa Valgrind no arquivo de estresse
    valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --errors-for-leak-kinds=all --error-exitcode=1 ./$EXEC test_boundary.txt
    
    if [ $? -eq 0 ]; then
        echo -e "\n✅ VALGRIND: Não foram encontrados leaks ou erros de memória."
    else
        echo -e "\n❌ VALGRIND: Falha! Foram encontrados leaks ou erros de memória."
    fi
    echo -e "\n--- FIM DO RELATÓRIO VALGRIND ---"
}

# --- Preparação e Limpeza ---
rm -f $EXEC test_boundary.txt file_exato.txt
echo "Arquivos temporários e executável antigo limpos."

# 1. Cria o arquivo de estresse de limites (Linhas Longas, Curtas, Vazias)
echo -e "Linha Curta\nA\nB.\n$(head -c 2000 < /dev/zero | tr '\0' 'C')\nQuase curta.\n\nLinha vazia acima.\n\nEsta é a linha final sem quebra de linha no final." > test_boundary.txt
echo "Arquivo 'test_boundary.txt' criado."

# 2. Cria o arquivo de teste exato (20 caracteres)
echo -e "0123456789ABCDEFGHIJ\nTeste Exato." > file_exato.txt
echo "Arquivo 'file_exato.txt' criado."

# --- MATRIZ DE TESTES ---

# A. Testes de Limites (test_boundary.txt)
compile_and_run 4096 "Buffer Grande" "test_boundary.txt" # Grande
compile_and_run 8 "Buffer Pequeno" "test_boundary.txt"   # Pequeno
compile_and_run 1 "Buffer Crítico" "test_boundary.txt"  # Crítico

# B. Testes de Intersecção (file_exato.txt)
compile_and_run 20 "Buffer Exato" "file_exato.txt"    # Exato (20)
compile_and_run 21 "Buffer +1" "file_exato.txt"       # Buffer +1
compile_and_run 19 "Buffer -1" "file_exato.txt"      # Buffer -1

# C. Teste de STDIN (Entrada Padrão)
compile_and_run 42 "STDIN" "" # Usa 42 como um valor comum para STDIN

# D. Teste de Valgrind
run_valgrind 1
