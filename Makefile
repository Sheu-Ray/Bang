all:main
main:main.c init.c print_status.c
	gcc main.c init.c print_status.c -o main
