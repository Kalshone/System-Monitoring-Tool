CC = gcc
CFLAGS = -Wall -Werror

mysystemstat: mySystemStats.o stats_functions.o
	${CC} ${CFLAGS} $^ -o $@

%.o: %.c
	${CC} ${CFLAGS} -c $<

	
.PHONY: clean 
clean:
	rm -f *.o mysystemstat

.PHONY: help
help: Makefile
	@sed -n 's/^##//p' $<
