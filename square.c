#include <stdlib.h>
#include "tok.h"

const char* example = "def (concat \"thing_\" $var1) [print \"Hello, world!\" (+ 2 $1 $\"3\")];";

int main(int argc, char** argv) {
  token_cursor cur;
  cur.p = example;
  cur.index = 0;

  token_list tokenlist;
  tokenlist.first = NULL;
  tokenlist.last = NULL;

  tokenise(&tokenlist, &cur);

  print_token_list(&tokenlist);

  return 0;
}
