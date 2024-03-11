#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINES 1000

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

  // Comprobar si los archivos de entrada y salida son idénticos (hardlinked)
  if (argc == 3) {
    struct stat input_stat, output_stat;
    if (stat(argv[1], &input_stat) == 0 && stat(argv[2], &output_stat) == 0) {
      if (input_stat.st_ino == output_stat.st_ino) {
        print_error("reverse: input and output file must differ");
        return 1;
      }
    }
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
    fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
    return 1;
  }

  // Leer líneas y revertirlas
  char *lines[MAX_LINES];
  int line_count = 0;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, input)) != -1) {
    lines[line_count] = strdup(line); // Almacenar la línea en un array
    if (lines[line_count] == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    line_count++;
  }

  // Escribir líneas invertidas en la salida en orden inverso
  for (int i = line_count - 1; i >= 0; i--) {
    if (fprintf(output, "%s", lines[i]) < 0) {
        perror("fprintf");
        exit(EXIT_FAILURE);
    }
    free(lines[i]); // Liberar la memoria asignada
  }

  // Liberar memoria y cerrar archivos
  free(line);
  fclose(input);
  if (output != stdout) {
    fclose(output);
  }

  return 0;
}
