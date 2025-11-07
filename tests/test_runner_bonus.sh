#!/bin/bash

# Define os arquivos fonte C para o bônus
SRCS="get_next_line_bonus.c get_next_line_utils.c main_bonus.c"
EXEC="gnl_bonus_test"

# Arquivos de teste
FILE1="test_boundary.txt"
FILE2="file_exato.txt"
FILE3="test_ten_lines.txt"

# --- Funções de Ajuda ---

compile_and_run() {
    local b_size=$1
    local test_target=$2
    
    echo -e "\n=================================================================="
    echo -e "⚙️  COMPILANDO: BUFFER_SIZE = $b_size"
    echo -e "=================================================================="

    # Compilação manual
    gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC

    if [ $? -ne 0 ]; then
        echo -e "\n❌ ERRO DE COMPILAÇÃO para BUFFER_SIZE = $b_size. Verifique 'main_bonus.c', 'get_next_line_bonus.c' e 'get_next_line_utils.c'"
        exit 1
    fi
    
    echo -e "\n>>> EXECUTANDO TESTE: $test_target | FDs: $FILE1, $FILE2, $FILE3 <<<\n"
    
    # Executa o driver com múltiplos arquivos
    ./$EXEC $FILE1 $FILE2 $FILE3
    
    echo -e "\n--- FIM DO TESTE: $test_target ---"
}

run_valgrind_bonus() {
    local b_size=$1

    echo -e "\n=================================================================="
    echo -e "🧠 TESTE DE MEMÓRIA (VALGRIND BÔNUS) | BUFFER_SIZE = $b_size"
    echo -e "=================================================================="
    
    # 1. Recompila
    gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC
    
    if [ $? -ne 0 ]; then
        echo "❌ ERRO DE COMPILAÇÃO para Valgrind Bônus."
        exit 1
    fi
    
    # 2. Executa Valgrind no teste intercalado
    echo -e "\nIniciando Valgrind na leitura intercalada de 3 arquivos..."
    valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --errors-for-leak-kinds=all --error-exitcode=1 ./$EXEC $FILE1 $FILE2 $FILE3
    
    if [ $? -eq 0 ]; then
        echo -e "\n✅ VALGRIND BÔNUS: Não foram encontrados leaks ou erros de memória."
    else
        echo -e "\n❌ VALGRIND BÔNUS: FALHA! Foram encontrados leaks ou erros de memória."
    fi
    echo -e "\n--- FIM DO RELATÓRIO VALGRIND ---"
}

# --- Preparação e Limpeza ---
rm -f $EXEC $FILE1 $FILE2 $FILE3
echo "Arquivos temporários e executável antigo limpos."

# 1. Cria o arquivo de estresse (Longas, Curtas, Vazias)
# Conteúdo do test_boundary.txt (Linhas Longas)
echo -e "Linha Curta\nA\nB.\n$(head -c 2000 < /dev/zero | tr '\0' 'C')\nQuase curta.\n\nLinha vazia acima.\n\nEsta é a linha final sem quebra de linha no final." > $FILE1
echo "Arquivo '$FILE1' (Limites) criado."

# 2. Cria o arquivo de teste exato (20 caracteres)
# Conteúdo do file_exato.txt (Buffer Exato)
echo -e "0123456789ABCDEFGHIJ\nTeste Exato." > $FILE2
echo "Arquivo '$FILE2' (Exato) criado."

# 3. Cria o arquivo de teste padrão (10 Linhas)
# Conteúdo do test_ten_lines.txt (10 Linhas Variadas)
echo -e "Esta é a Linha 1.\nLinha 2, curta.\n3: Este arquivo tem de ser robusto.\n4: Quatro.\n5: Cinco.\n6: A sexta linha é grande. É bem longa mesmo, mais do que 42 bytes, para forçar várias chamadas de read() se o buffer for pequeno.\n7: Sete.\n8: Oito, outra linha curta.\n9: Linha Nove.\n10: Esta é a Décima e ÚLTIMA LINHA. Sem quebra de linha no final." > $FILE3
echo "Arquivo '$FILE3' (10 Linhas) criado."


# --- MATRIZ DE TESTES BÔNUS ---

# 1. Teste de Leitura Intercalada com diferentes BUFFER_SIZE
compile_and_run 4096 "Buffer Grande"
compile_and_run 8 "Buffer Pequeno"
compile_and_run 1 "Buffer Crítico"

# 2. Teste de Valgrind em modo crítico (BUFFER_SIZE=1)
run_valgrind_bonus 1
