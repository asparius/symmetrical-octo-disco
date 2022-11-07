obj-m += mymodule2.o

mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

cmod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


run : clean a.out
	./a.out
a.out:
	gcc -g shellax-skeleton.c
	
clean:
	rm ./a.out
	
write:
	vim shellax-skeleton.c

make a:
	touch ./a.out
