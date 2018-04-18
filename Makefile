file : ./lib/print_wait_status.c ./lib/error_functions.c
	gcc -o multi_join ./pthread/thread_multijoin.c ./lib/print_wait_status.c ./lib/error_functions.c ./lib/get_num.c -lpthread
	