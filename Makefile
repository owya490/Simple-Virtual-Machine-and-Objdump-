CC=gcc
CFLAGS= -fsanitize=address -Wvla -Wall -Werror -g -std=gnu11 -lasan

# fill in all your make rules

vm_x2017: vm_x2017.c
	$(CC) $(CFLAGS) $^ -o $@

objdump_x2017: objdump_x2017.c
	$(CC) $(CFLAGS) $^ -o $@

tests: 
	echo "tests"

run_tests: vm_x2017
	bash test.sh

clean:
	echo "clean"
	rm objdump_x2017
	rm vm_x2017
