CFLAGS =
EXEC = exc
ARGS =
OBJ = main.o core_gameplay.o manual_moves.o ai_moves.o print_ui.o utils.o

$(EXEC): $(OBJ)
	gcc $(OBJ) -o $(EXEC) $(CFLAGS)

val: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(EXEC) $(ARGS)

clean:
	rm -f $(OBJ) $(EXEC)

run:
	make --no-print-directory
	./$(EXEC) $(ARGS)


