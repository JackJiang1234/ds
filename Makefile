all:
	gcc -g -fno-builtin -shared dlist.c darray.c -o libds.so
	gcc -g -fno-builtin dlist.c -DDLIST_TEST -o dlist_test
	gcc -g -fno-builtin darray.c -DDARRAY_TEST -o darray_test 

clean:
	rm -f *test *.exe *.so *.o
