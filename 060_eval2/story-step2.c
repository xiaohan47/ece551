#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "The arguments format is incorrect.\n");
    return EXIT_FAILURE;
  }

  catarray_t * cat = read_list(argv[1]);

  printWords(cat);

  return EXIT_SUCCESS;
}
