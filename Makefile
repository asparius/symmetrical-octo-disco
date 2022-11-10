obj-m += mymodule2.o

mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

cmod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


run : clean a.out
	./a.out
a.out:
	gcc -I/usr/include/python3.10 shellax-skeleton.c -L/usr/lib/Python3.10/config-3.10-x86_64-linux-gnu -lpython3.10
	
clean:
	rm ./a.out
	
write:
	vim shellax-skeleton.c

make a:
	touch ./a.out
