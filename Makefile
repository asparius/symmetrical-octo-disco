run : clean a.out
	./a.out
a.out:
	gcc shellax-skeleton.c
	
clean:
	rm ./a.out
	
write:
	vim shellax-skeleton.c

make a:
	touch ./a.out
