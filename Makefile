all:
	gcc -g -fno-builtin -Wall -shared dlist.c darray.c sort.c -o libds.so
	gcc -g -fno-builtin dlist.c -DDLIST_TEST -o dlist_test.exe
	gcc -g -fno-builtin darray.c -DDARRAY_TEST -o darray_test.exe 
	gcc -g -fno-builtin sort.c -DSORT_TEST -o sort_test.exe 

clean:
	rm -f *test *.exe *.so *.o
