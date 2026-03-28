CFLAGS =
EXEC = exc
ARGS =
OBJ = main.o

$(EXEC): $(OBJ)
	gcc $(OBJ) -o $(EXEC) $(CFLAGS)

val: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(EXEC) $(ARGS)

# reader: $(READEROBJ) 
# 	gcc $(READEROBJ) -o reader $(CFLAGS)

# write: $(WRITEOBJ) 
# 	gcc $(WRITEOBJ) -o write $(CFLAGS)

# all: $(EXEC) reader write

# clean:
# 	rm -f $(OBJ) $(READEROBJ) $(WRITEOBJ) $(EXEC) reader write

# run:
# 	make all --no-print-directory
# 	./$(EXEC) $(ARGS)
