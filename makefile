OBJ = main.o helpers.o syncdir.o
all: deamon
deamon: $(OBJ)
	gcc $(OBJ) -o deamon
$(OBJ): helpers.h syncdir.h
.PHONY: clean
clean:
	rm -f *.o deamon