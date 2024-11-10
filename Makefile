CC = clang
CFLAGS = -Wall -g \
         -I/opt/homebrew/opt/sdl2/include \
         -I/opt/homebrew/opt/sdl2/include/SDL2 \
         -I/opt/homebrew/opt/sdl2_image/include  # Tab indentation here
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib \
          -L/opt/homebrew/opt/sdl2_image/lib \
          -lSDL2 -lSDL2_image

SRC = src/main.c
OBJ = src/main.o
EXEC = fighting-game

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)  # This line must start with a tab

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o  # Tab indentation here

clean:
	rm -f $(OBJ) $(EXEC)
