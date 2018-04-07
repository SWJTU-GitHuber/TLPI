file : ./lib/print_wait_status.c ./lib/error_functions.c
	gcc -o proc_monitor ./monitor_subproc/child_status.c ./lib/print_wait_status.c ./lib/error_functions.c ./lib/get_num.c
	