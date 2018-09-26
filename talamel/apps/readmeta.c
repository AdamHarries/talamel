#include <stdio.h>
#include <stdlib.h>

#define TAGLIB_STATIC

#include <talamel.h>

void print_metadata(const char* filename) { 
  tml_TalamelFile* f = tml_open_file(filename); 

  char* title = tml_read_title(f); 
  printf("Title: %s\n", title); 
  free(title); 

  char* artist = tml_read_artist(f); 
  printf("Artist: %s\n", artist); 
  free(artist); 

  int bpm = tml_read_bpm(f); 
  printf("Bpm: %d\n", bpm);

  int comments = tml_count_comments(f); 
  if(comments == 0)
    printf("No comments found in file.\n");
  for(int i = 0; i< comments; i++) { 
    char * comm = tml_get_comment(f, i); 
    printf("Comment [%d]: %s\n", i, comm); 
    free(comm);
  }

  tml_free_file(f); 
}

int main(int argc, char *argv[]) {
  for(int i = 1; i< argc;i++) { 
    print_metadata(argv[i]);
  }
  return 0;
}
