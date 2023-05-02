gcc -I/usr/include/lua5.1 -O2 -c -o reglua.o reglua.c
gcc -shared -o reglua.so reglua.o