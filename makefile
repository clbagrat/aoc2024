run:
	gcc -fsanitize=address -o solution ./day$(d)/solution.c && time ./solution && rm ./solution
clean:
	rm -f ./solution
