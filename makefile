run:
	gcc -fsanitize=address -o solution ./day$(d)/solution.c && ./solution && rm ./solution

clean:
	rm -f ./solution
