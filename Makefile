BIN_DIR := bin

all: tradutor

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/tradutor.tab.c $(BIN_DIR)/tradutor.tab.h: tradutor.y | $(BIN_DIR)
	bison -t -v -d -o $(BIN_DIR)/tradutor.tab.c tradutor.y

$(BIN_DIR)/lex.yy.c: tradutor.l $(BIN_DIR)/tradutor.tab.h | $(BIN_DIR)
	flex -o$(BIN_DIR)/lex.yy.c tradutor.l

tradutor: tradutor.c $(BIN_DIR)/lex.yy.c $(BIN_DIR)/tradutor.tab.c $(BIN_DIR)/tradutor.tab.h
	gcc -o tradutor $(BIN_DIR)/tradutor.tab.c $(BIN_DIR)/lex.yy.c tradutor.c -I. -lfl

clean:
	rm -rf tradutor $(BIN_DIR)
