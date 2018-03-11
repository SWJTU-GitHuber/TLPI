file : ./lib/error_functions.c ./fileio/seekio.c
	gcc -o demo_inotify ./inotify/demo_inotify.c ./lib/error_functions.c ./lib/get_num.c
	