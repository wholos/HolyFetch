CXX = tcc

all: main.c
	@echo "Build..."
	@$(CXX) main.c -o main.out

clean:
	@echo "Clean..."
	@rm main.out
