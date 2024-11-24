CC = clang
CFLAGS = -Wall -g \
         -I/opt/homebrew/opt/sdl2/include \
         -I/opt/homebrew/opt/sdl2/include/SDL2 \
         -I/opt/homebrew/opt/sdl2_image/include \
         -I/opt/homebrew/opt/sdl2_ttf/include \
         -I/opt/homebrew/opt/sdl2_mixer/include

LDFLAGS = -L/opt/homebrew/lib \
          -L/opt/homebrew/opt/sdl2_image/lib \
          -L/opt/homebrew/opt/sdl2_ttf/lib \
          -L/opt/homebrew/opt/sdl2_mixer/lib \
          -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRC = src/main.c
OBJ = src/main.o
EXEC = fighting-game

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

clean:
	rm -f $(OBJ) $(EXEC)
