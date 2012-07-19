all:
	gcc -g -fno-builtin -shared dlist.c -o libdlist.so
	gcc -g -fno-builtin dlist.c -DDLIST_TEST -o dlist_test

clean:
	rm -f *test *.exe *.so
