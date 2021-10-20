#include "rand_story.h"

//read the template file and return a char*
FILE * open_file(const char * file_name) {
  FILE * f = fopen(file_name, "r");
  if (f == NULL) {
    fprintf(stderr, "The input template file is empty.\n");
    exit(EXIT_FAILURE);
  }
  return f;
}

char * read_temp(FILE * f) {
  char * temp = NULL;
  int num_temp = 0;
  int c = 0;

  while ((c = fgetc(f)) != EOF) {
    temp = realloc(temp, (num_temp + 1) * sizeof(*temp));
    temp[num_temp] = c;
    num_temp++;
  }
  temp = realloc(temp, (num_temp + 1) * sizeof(*temp));
  temp[num_temp] = '\0';

  if (fclose(f) != 0) {
    fprintf(stderr, "The input template file cannot close.\n");
    exit(EXIT_FAILURE);
  }
  return temp;
}

/*check if the template file following the rule of each
  '_' have a matching '_' on the same line; */
int check_underscore(char * p) {
  char * symbol = strchr(p + 1, '_');
  char * next_line = strchr(p, '\n');
  if (symbol == NULL) {
    return 0;
  }
  else if (next_line - symbol < 0) {
    return 0;
  }
  return 1;
}

//check if the name is integer
int check_int(char * name) {
  char * p = name;
  while (*p != '\0') {
    if (*p > 48 && *p <= 57) {
      return 0;
    }
    if (*p == 48) {
      fprintf(stderr, "The number on blank should not be zero.\n");
      return EXIT_FAILURE;
    }
    p++;
  }
  return atoi(name);
}

char * get_used(category_t * used, int n_back) {
  if (used->n_words == 0) {
    fprintf(stderr, "There is no used work.\n");
    exit(EXIT_FAILURE);
  }

  char * replace_w = used->words[(used->n_words) - n_back];
  return replace_w;
}

void add_to_used(category_t * used, char * word) {
  used->n_words++;
  size_t len_w = strlen(word);
  used->words = realloc(used->words, used->n_words * sizeof(*used->words));
  used->words[used->n_words - 1] =
      malloc((len_w + 1) * sizeof(*used->words[used->n_words - 1]));
  strcpy(used->words[used->n_words - 1], word);
}

//check if the name in the c.
/*void check_name_cats(char * name, catarray_t * c) {
  if (c->n == 0) {
    fprintf(stderr, "There is no more valid category in cat.\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < c->n; i++) {
    if (strcmp(name, c->arr[i].name) == 0) {
      return;
    }
  }
  fprintf(stderr, "There is an invalet name.\n");
  exit(EXIT_FAILURE);
  }*/

void no_reused_cat(catarray_t * cat, char * word) {
  if (cat->n == 0) {
    fprintf(stderr, "There is no more category avaliable.\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < cat->n; i++) {
    for (size_t j = 0; j < cat->arr[i].n_words; j++) {
      if (strcmp(word, cat->arr[i].words[j]) == 0) {
        free(cat->arr[i].words[j]);
        cat->arr[i].n_words--;
        return;
      }
    }
  }
}
char * parse_name(char * start) {
  char * end = strchr(start + 1, '_');
  size_t len_blank = end - start - 1;
  char * cate_name = malloc((len_blank + 1) * sizeof(*cate_name));
  strncpy(cate_name, start + 1, len_blank);
  cate_name[len_blank] = '\0';
  return cate_name;
}
char * find_replace(char * name, catarray_t * cat, category_t * used) {
  const char * replace_word = NULL;

  //if cat is null
  if (cat == NULL) {
    replace_word = chooseWord(name, cat);
  }

  //if name is an integer
  int i = check_int(name);
  if (i != 0) {
    replace_word = used->words[used->n_words - i];
  }

  //if name is char
  if (i == 0) {
    replace_word = chooseWord(name, cat);
  }
  return (char *)replace_word;
}
char * replace_blank(char * p_temp,
                     size_t n_temp,
                     char * start,
                     catarray_t * cats,
                     category_t * used_word,
                     int flag) {
  if (check_underscore(start) == 0) {
    fprintf(stderr, "The blank format does not meet the requirement.");
    exit(EXIT_FAILURE);
  }

  //parse the name
  char * cat_name = parse_name(start);
  char * replace_word = find_replace(cat_name, cats, used_word);

  //add the replace_word into the used category
  used_word->n_words++;
  used_word->words =
      realloc(used_word->words, (used_word->n_words) * sizeof(*used_word->words));
  used_word->words[used_word->n_words - 1] = strdup(replace_word);

  if (flag == 1) {
    no_reused_cat(cats, replace_word);
  }

  free(cat_name);

  //replace the word in the string
  n_temp = n_temp + strlen(used_word->words[used_word->n_words - 1]);
  p_temp = realloc(p_temp, (n_temp + 1) * sizeof(*p_temp));
  strcat(p_temp, used_word->words[used_word->n_words - 1]);
  p_temp[n_temp] = '\0';

  //add the replace word to the used_word

  return p_temp;
}

char * parse_temp(char * temp, catarray_t * cats, int flag) {
  char * ptr = temp;
  char * parsed_t = NULL;
  size_t num_temp = 0;

  //define the a category_t to record the used words
  category_t * used_word = malloc(sizeof(*used_word));
  used_word->name = NULL;
  used_word->words = NULL;
  used_word->n_words = 0;

  while (*ptr != '\0') {
    if (*ptr != '_') {
      parsed_t = realloc(parsed_t, (num_temp + 2) * sizeof(*parsed_t));
      parsed_t[num_temp] = *ptr;
      parsed_t[num_temp + 1] = '\0';
      num_temp++;
    }
    if (*ptr == '_') {
      parsed_t = replace_blank(parsed_t, num_temp, ptr, cats, used_word, flag);
      num_temp = strlen(parsed_t);
      ptr = strchr(ptr + 1, '_');
    }
    ptr++;
  }

  ptr = NULL;
  parsed_t = realloc(parsed_t, (num_temp + 1) * sizeof(*parsed_t));
  parsed_t[num_temp] = '\0';

  //free used_word category
  for (size_t i = 0; i < used_word->n_words; i++) {
    free(used_word->words[i]);
  }
  free(used_word->words);
  free(used_word);

  return parsed_t;
}

int check_name(char * name, catarray_t * cat) {
  if (cat->n == 0) {
    return 0;  //there is no category of this name add yet
  }
  else {
    for (size_t i = 0; i < cat->n; i++) {
      if (strcmp(cat->arr[i].name, name) == 0) {
        return 1;  //there is already the category of this name
      }
    }
  }
  return 0;
}

void add_name(char * name, char * word, catarray_t * cat) {
  cat->arr = realloc(cat->arr, (cat->n + 1) * sizeof(*cat->arr));
  cat->arr[cat->n].name = name;
  cat->arr[cat->n].n_words = 1;
  cat->arr[cat->n].words = NULL;
  cat->arr[cat->n].words =
      realloc(cat->arr[cat->n].words,
              cat->arr[cat->n].n_words * sizeof(*(cat->arr[cat->n].words)));
  cat->arr[cat->n].words[cat->arr[cat->n].n_words - 1] = word;
  cat->n++;
}

void add_word(char * name, char * word, catarray_t * cat) {
  for (size_t i = 0; i < cat->n; i++) {
    if (strcmp(name, cat->arr[i].name) == 0) {
      for (size_t j = 0; j < cat->arr[i].n_words; j++) {
        if (strcmp(word, cat->arr[i].words[j]) == 0) {
          free(word);
          return;
        }
      }
      cat->arr[i].n_words++;
      cat->arr[i].words =
          realloc(cat->arr[i].words, cat->arr[i].n_words * sizeof(*cat->arr[i].words));
      cat->arr[i].words[cat->arr[i].n_words - 1] = word;
    }
  }
}
char * find_name(char * line) {
  char * col = strchr(line, ':');
  if (col == NULL) {
    fprintf(stderr, "The format of list file is incorrect.\n");
    exit(EXIT_FAILURE);
  }
  int len_n = col - line;
  char * name = malloc((len_n + 1) * sizeof(*name));
  strncpy(name, line, len_n);
  name[len_n] = '\0';
  return name;
}

char * find_word(char * line) {
  char * col = strchr(line, ':');
  col++;
  char * newline = strchr(line, '\n');
  int len_w = newline - col;
  char * word = malloc((len_w + 1) * sizeof(*word));
  strncpy(word, col, len_w);
  word[len_w] = '\0';
  return word;
}

//read the list of name and word, return a catarray type.
catarray_t * read_list(FILE * f) {
  size_t sz = 0;
  char * line = NULL;
  catarray_t * array_cat = malloc(sizeof(*array_cat));
  array_cat->n = 0;
  array_cat->arr = NULL;

  while (getline(&line, &sz, f) >= 0) {
    char * name = find_name(line);
    char * word = find_word(line);
    int name_exist = check_name(name, array_cat);

    //if the name is not exist in category
    if (name_exist == 0) {
      add_name(name, word, array_cat);
    }
    if (name_exist == 1) {
      add_word(name, word, array_cat);
      free(name);
    }
    free(line);
    line = NULL;
    word = NULL;
  }
  free(line);

  if (fclose(f) != 0) {
    fprintf(stderr, "The input list file cannot close.\n");
    exit(EXIT_FAILURE);
  }
  return array_cat;
}

void free_cat(catarray_t * cat) {
  for (size_t i = 0; i < cat->n; i++) {
    for (size_t j = 0; j < cat->arr[i].n_words; j++) {
      free(cat->arr[i].words[j]);
    }
    free(cat->arr[i].words);
    free(cat->arr[i].name);
  }
  free(cat->arr);
  free(cat);
}
