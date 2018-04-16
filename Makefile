file : ./lib/print_wait_status.c ./lib/error_functions.c
	gcc -o simple_pthread ./pthread/simple_thread.c ./lib/print_wait_status.c ./lib/error_functions.c ./lib/get_num.c -lpthread
	