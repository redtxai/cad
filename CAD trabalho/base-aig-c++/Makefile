CC = g++
CFLAGS = -Wall -g

EXECUTABLE = main
LOG = aagComentado.txt

HEADER_DIR = headers/

all: main.cpp
	$(CC) $(CFLAGS) main.cpp aagReader.cpp aig.cpp -o $(EXECUTABLE) -I$(HEADER_DIR)

run:
	./$(EXECUTABLE)
	@echo "\nExecução encerrada. Dump do log.\n"
	@tput setaf 1 			 # Alterar cor do output para vermelho
	@cat $(LOG)
	@echo "\n"
	@tput setaf default # Retornar para cor padrão.
