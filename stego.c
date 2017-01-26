#include <stdio.h>
#include <stdlib.h>

#include "stego.h"

//Program killer
void die(char* reason){
  fprintf(stderr, "ERROR: %s\n", reason);
  exit(1);
}

//Returns 1 if P6, else returns 0
int read_ppm_type(char *fp) {
  char temp = ReadUSART(fp);
  if(temp == 'P' && ReadUSART(fp) == '6') {
    ReadUSART(fp);
    return 1;
  }

  return 0;
}

void skip_comments(char *fp) {
  char temp;
  while((temp = ReadUSART(fp)) == '#') {
	//printf("First while skipping\n");
    while(ReadUSART(fp) != '\n') {
		//printf("Second while skipping\n");
	}
  }

  //Give back the first character of width that we took
  ungetc(temp, fp);

  return;
}

//Returns the width of the image
int get_width(char *fp) {
  int w;
  fscanf(fp, "%d", &w);
  return w;
}

//Returns the height of the image
int get_height(char *fp) {
  int h;
  fscanf(fp, "%d", &h);
  return h;
}

//Reads the color depth, returns 1 if 255, else returns 0
int read_color_depth(char *fp) {
  int c;
  fscanf(fp, "%d", &c);
  if(c == 255) {
    ReadUSART(fp);    //Get rid of the trailing whitespace
    return 1;
  }
  return 0;
}
