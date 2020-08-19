make: main.c Image.c
	gcc -o exec -Wall -O2 main.c Image.c GenerateMovie.c Color.c ConvertImage.c