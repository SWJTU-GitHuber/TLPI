file : ./lib/print_wait_status.c ./lib/error_functions.c
	gcc -o closeonexec ./execve/closeonexec.c ./lib/print_wait_status.c ./lib/error_functions.c ./lib/get_num.c
	