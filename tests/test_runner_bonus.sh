#!/bin/bash

# Define the C source files for the bonus
SRCS="get_next_line_bonus.c get_next_line_utils.c main_bonus.c"
EXEC="gnl_bonus_test"

# Test files
FILE1="test_boundary.txt"
FILE2="file_exact.txt"
FILE3="test_ten_lines.txt"

# --- Helper Functions ---

# Function to compile and run the test with a specific BUFFER_SIZE
compile_and_run() {
	local b_size=$1
	local test_target=$2
	
	echo -e "\n=================================================================="
	echo -e "⚙️  COMPILING: BUFFER_SIZE = $b_size"
	echo -e "=================================================================="

	# Manual compilation
	gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC

	if [ $? -ne 0 ]; then
		echo -e "\n❌ COMPILATION ERROR for BUFFER_SIZE = $b_size. Check 'main_bonus.c', 'get_next_line_bonus.c', and 'get_next_line_utils.c'"
		exit 1
	fi
	
	echo -e "\n>>> EXECUTING TEST: $test_target | FDs: $FILE1, $FILE2, $FILE3 <<<\n"
	
	# Execute the driver with multiple files
	./$EXEC
	
	echo -e "\n--- END OF TEST: $test_target ---"
}

# Function to run the memory check with Valgrind
run_valgrind_bonus() {
	local b_size=$1

	echo -e "\n=================================================================="
	echo -e "🧠 MEMORY TEST (VALGRIND BONUS) | BUFFER_SIZE = $b_size"
	echo -e "=================================================================="
	
	# 1. Recompile
	gcc -Wall -Wextra -Werror -g -D BUFFER_SIZE=$b_size $SRCS -o $EXEC
	
	if [ $? -ne 0 ]; then
		echo "❌ COMPILATION ERROR for Valgrind Bonus."
		exit 1
	fi
	
	# 2. Execute Valgrind on the intercalated test
	echo -e "\nStarting Valgrind on intercalated read of 3 files..."
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --errors-for-leak-kinds=all --error-exitcode=1 ./$EXEC
	
	if [ $? -eq 0 ]; then
		echo -e "\n✅ VALGRIND BONUS: No memory leaks or errors found."
	else
		echo -e "\n❌ VALGRIND BONUS: FAILURE! Memory leaks or errors were found."
	fi
	echo -e "\n--- END OF VALGRIND REPORT ---"
}

# --- Setup and Cleanup ---
rm -f $EXEC $FILE1 $FILE2 $FILE3
echo "Temporary files and old executable cleaned."

# 1. Create the stress file (Long, Short, Empty Lines)
# Contents of test_boundary.txt (Boundary Lines)
echo -e "Short Line\nA\nB.\n$(head -c 2000 < /dev/zero | tr '\0' 'C')\nAlmost short.\n\nEmpty line above.\n\nThis is the final line with no newline at the end." > $FILE1
echo "File '$FILE1' (Boundary) created."

# 2. Create the exact test file (20 characters)
# Contents of file_exact.txt (Exact Buffer)
echo -e "0123456789ABCDEFGHIJ\nExact Test." > $FILE2
echo "File '$FILE2' (Exact) created."

# 3. Create the standard test file (10 Lines)
# Contents of test_ten_lines.txt (10 Varied Lines)
echo -e "This is Line 1.\nLine 2, short.\n3: This file must be robust.\n4: Four.\n5: Five.\n6: The sixth line is big. It is very long, more than 42 bytes, to force multiple read() calls if the buffer is small.\n7: Seven.\n8: Eight, another short line.\n9: Line Nine.\n10: This is the Tenth and LAST LINE. No newline at the end." > $FILE3
echo "File '$FILE3' (10 Lines) created."


# --- BONUS TEST MATRIX ---

# 1. Intercalated Read Test with different BUFFER_SIZE
compile_and_run 4096 "Large Buffer"
compile_and_run 8 "Small Buffer"
compile_and_run 1 "Critical Buffer"

# 2. Valgrind Test in Critical Mode (BUFFER_SIZE=1)
run_valgrind_bonus 1
