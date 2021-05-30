CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = list_files.h command.h screentest.h gotoxy.h static_variables.h
OBJ = command.o static_variables.o list_files.o screentest.o gotoxy.o file_explorer.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

file_explorer: $(OBJ)
	$(CC) $(CFLAGS) -o  $@ $^

