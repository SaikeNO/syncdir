OBJ = main.o helpers.o syncdir.o list.o scan_directory.o
all: deamon
deamon: $(OBJ)
	gcc $(OBJ) -o deamon -lssl -lcrypto
$(OBJ): types.h helpers.h syncdir.h list.h scan_directory.h
.PHONY: clean
clean:
	rm -f *.o deamon