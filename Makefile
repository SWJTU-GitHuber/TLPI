file : ./lib/print_wait_status.c ./lib/error_functions.c
	gcc -o mutex_pthread ./pthread/thread_incr_mutex.c ./lib/print_wait_status.c ./lib/error_functions.c ./lib/get_num.c -lpthread
	