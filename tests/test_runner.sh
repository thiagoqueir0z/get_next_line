#!/bin/bash

# Define the C source files (Assuming they are in the current folder)
SRCS="get_next_line.c get_next_line_utils.c main.c"
EXEC="gnl_test"

# --- Helper Functions ---

# Function to compile and run the test with a specific BUFFER_SIZE
compile_and_run() {
	local b_size=$1
	local test_target=$2
	local test_file=$3

	echo -e "\n=================================================================="
	echo -e "⚙️  COMPILING: BUFFER_SIZE = $b_size"
	echo -e "=================================================================="

	# Manual compilation with the -D BUFFER_SIZE flag
	gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC

	if [ $? -ne 0 ]; then
		echo -e "\n❌ COMPILATION ERROR for BUFFER_SIZE = $b_size"
		exit 1
	fi
	
	echo -e "\n>>> EXECUTING TEST: $test_target ($test_file) <<<\n"
	
	if [ "$test_target" == "STDIN" ]; then
		echo -e ">>> Please type some lines (Ctrl+D to finish):\n"
		./$EXEC
	else
		./$EXEC $test_file
	fi
	
	echo -e "\n--- END OF TEST ---"
}

# Function to run the memory check with Valgrind
run_valgrind() {
	local b_size=$1

	echo -e "\n=================================================================="
	echo -e "🧠 MEMORY TEST (VALGRIND) | BUFFER_SIZE = $b_size"
	echo -e "=================================================================="
	
	# 1. Recompile with the critical size 1
	gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC
	
	if [ $? -ne 0 ]; then
		echo "❌ COMPILATION ERROR for Valgrind."
		exit 1
	fi
	
	# 2. Execute Valgrind on the stress file
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --errors-for-leak-kinds=all --error-exitcode=1 ./$EXEC test_boundary.txt
	
	if [ $? -eq 0 ]; then
		echo -e "\n✅ VALGRIND: No memory leaks or errors found."
	else
		echo -e "\n❌ VALGRIND: FAILURE! Memory leaks or errors were found."
	fi
	echo -e "\n--- END OF VALGRIND REPORT ---"
}

# --- Setup and Cleanup ---
rm -f $EXEC test_boundary.txt file_exact.txt
echo "Temporary files and old executable cleaned."

# 1. Create the boundary stress file (Long, Short, Empty Lines)
echo -e "Short Line\nA\nB.\n$(head -c 2000 < /dev/zero | tr '\0' 'C')\nAlmost short.\n\nEmpty line above.\n\nThis is the final line with no newline at the end." > test_boundary.txt
echo "File 'test_boundary.txt' created."

# 2. Create the exact test file (20 characters)
echo -e "0123456789ABCDEFGHIJ\nExact Test." > file_exact.txt
echo "File 'file_exact.txt' created."

# --- TEST MATRIX ---

# A. Boundary Tests (test_boundary.txt)
compile_and_run 4096 "Large Buffer" "test_boundary.txt" # Large
compile_and_run 8 "Small Buffer" "test_boundary.txt"    # Small
compile_and_run 1 "Critical Buffer" "test_boundary.txt" # Critical

# B. Intersection Tests (file_exact.txt)
compile_and_run 20 "Exact Buffer" "file_exact.txt"      # Exact (20)
compile_and_run 21 "Buffer +1" "file_exact.txt"        # Buffer +1
compile_and_run 19 "Buffer -1" "file_exact.txt"        # Buffer -1

# C. STDIN Test (Standard Input)
compile_and_run 42 "STDIN" "" # Uses 42 as a common value for STDIN

# D. Valgrind Test
run_valgrind 1
