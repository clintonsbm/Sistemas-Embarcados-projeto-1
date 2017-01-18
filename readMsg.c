#include <stdio.h>
#include "stego.h"
#include <stdlib.h>

int get_msg_length(FILE *);
void decode_message(int, FILE *);

int main(int argc, char **argv) {
  FILE *fp;

  //Print an error if no file found
  if((fp = fopen(argv[1], "rb")) == NULL) {
    printf("\nError: Please provide a file to scan.\n\n");
    return 1;

  }

  if(read_ppm_type(fp)) {
    printf("\nStart aplication!\n");

    //Explanation on WriteMsg.c
    skip_comments(fp);
    get_width(fp);
    get_height(fp);
    if(read_color_depth(fp)) {

    //int length = get_msg_length(fp);

    //Read the message length from the function parameters and convert to int
    char *len = (char *)argv[2];
    int length = atoi(len);
	  printf("%d", length);

    //printf("\nHoly secret message batman! We cracked the code: \n");
    printf("\nDone! The message decoded was this: \n");
    decode_message(length, fp);

    fclose(fp);

    } else {
        printf("Error: Invalid Color Depth.");
        return 1;
    }
  } else {
      printf("Error: Wrong PPM File Format. Terminating.");
      return 1;
  }

  return 0;
}

/*
//Gets the length of the secret message
int get_msg_length(FILE *fp) {
  char temp = 0;
  int length = 0;
  int i;

  for(i=0; i < 8; i++) {
    temp = fgetc(fp);
    //Delay the shift by one to get the 29, otherwise I'd get 58
    if(i > 0) length <<= 1;
    length |= (temp & 1);
  }
	printf("%d", length);
    return length;
}
*/

void decode_message(int length, FILE *fp) {
  int readTo = length * 8, numRead = 0, i;
  unsigned char charBuffer = 0;
  char temp;
  char secret_message[length + 1];
  int idx = 0;

  //Do the inverse function of WriteMsg encode_message
  while(numRead <= readTo) {
    temp = fgetc(fp);
    if(numRead % 8 == 0) {
      secret_message[idx] = charBuffer;
      idx++;
      charBuffer = 0;
    } else {
      charBuffer <<= 1;
    }
    charBuffer |= (temp & 1);
    numRead++;
  }

  //Starts printing from character 2 because the first char is junk
  for(i = 2; i < idx; i++) {
    printf("%c", secret_message[i]);
  }

  printf("\n\n");

  return;
}
