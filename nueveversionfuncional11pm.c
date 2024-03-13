#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

// Función para imprimir un mensaje de error
void print_error(const char *message) {
  fprintf(stderr, "%s\n", message);
  exit(1);
}

// Función para invertir una cadena
void reverse_string(char *str) {
  int length = strlen(str);
  for (int i = 0; i < length / 2; i++) {
    char temp = str[i];
    str[i] = str[length - i - 1];
    str[length - i - 1] = temp;
  }
}

int main(int argc, char *argv[]) {
  // Si no se proporcionan suficientes argumentos, mostrar mensaje de uso
  if (argc < 1 || argc > 3) {
    print_error("usage: reverse <input> <output>");
  }

  // Abrir archivo de entrada
  FILE *input;
  if (argc == 1) {
    input = stdin; // Usar la entrada estándar
  } else {
    if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
      input = fopen(argv[1], "r+");
    } else {
      input = fopen(argv[1], "r");
    }
    if (input == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      return 1;
    }
  }

  // Abrir archivo de salida
  FILE *output;
  if (argc == 1 || argc == 2) {
    output = stdout; // Usar la salida estándar
  } else {
    output = fopen(argv[2], "w");
    if (output == NULL) {
      if (input != stdin) {
        fclose(input);
      }
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
      return 1;
    }
  }

  // Leer y escribir el contenido del archivo de entrada invertido en el archivo de salida
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), input) != NULL) {
    // Invertir la línea
    reverse_string(line);
    // Escribir la línea invertida en el archivo de salida
    fprintf(output, "%s", line);
  }

  // Cerrar archivos si no son la entrada y la salida estándar
  if (input != stdin) {
    fclose(input);
  }
  if (output != stdout) {
    fclose(output);
  }

  return 0;
}
