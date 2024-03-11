#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función para imprimir un mensaje de error
void print_error(const char *message) {
  fprintf(stderr, "%s\n", message);
  exit(1);
}

// Función para manejar el fallo de malloc
void malloc_failed() {
  print_error("malloc failed");
}

int main(int argc, char *argv[]) {
  // Comprobar si hay demasiados argumentos
  if (argc > 3) {
    print_error("usage: reverse <input> <output>");
    return 1;
  }

  // Comprobar si los archivos de entrada y salida son idénticos
  if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
    print_error("reverse: input and output file must differ");
    return 1;
  }

  // Abrir archivo de entrada (stdin si no hay argumentos, argv[1] en otro caso)
  FILE *input = NULL;
  if (argc == 1) {
    input = stdin;
  } else {
    input = fopen(argv[1], "r");
    if (input == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      return 1;
    }
  }

  // Abrir archivo de salida (stdout si no hay o un argumento, argv[2] si hay dos argumentos)
  FILE *output = (argc <= 1) ? stdout : fopen(argv[2], "w");
  if (output == NULL && argc > 1) {
    fclose(input); // Cerrar el archivo de entrada si la apertura falló
    fprintf(stderr, "error: cannot open file 'input.txt''%s'\n", argv[2]);
    return 1;
  }

  // Leer líneas y revertirlas
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, input)) != -1) {
    // Revertir la línea
    size_t i = 0, j = read - 2; // Excluir el carácter de nueva línea
    while (i < j) {
      char temp = line[i];
      line[i] = line[j];
      line[j] = temp;
      i++;
      j--;
    }

    // Escribir la línea revertida en la salida
    if (fprintf(output, "%s", line) < 0) {
      free(line); // Liberar la memoria asignada
      fclose(input);
      if (output != stdout) {
        fclose(output);
      }
      print_error("Error al escribir en la salida");
      return 1;
    }
  }

  // Liberar memoria y cerrar archivos
  free(line);
  fclose(input);
  if (output != stdout) {
    fclose(output);
  }

  return 0;
}
