C=tcc
CFLAGS=-o main.out

all: main.c
	@echo "Build..."
	@$(C) $(CFLAGS) main.c

install:
	@echo "Build..."
	@echo "Install..."
	@$(C) $(CFLAGS) main.c
	@sudo cp main.out /usr/local/bin/holyfetch

uninstall:
	@echo "Uninstalling..."
	@sudo rm /usr/local/bin/holyfetch

clean:
	@echo "Clean..."
	@rm main.out
