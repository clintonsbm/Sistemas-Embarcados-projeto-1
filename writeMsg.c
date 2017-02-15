#include <stdio.h>
#include "stego.h"
#include <sys/timeb.h>  /* ftime, timeb (for timestamp in millisecond) */
#include <sys/time.h>   /* gettimeofday, timeval (for timestamp in microsecond) */
#include <time.h>
#include <sys/resource.h>

void copy_header(FILE *, int, FILE *);
int get_message_length(char[]);
int message_fits(int, int, int);
int count_new_lines(FILE *);
void encode_length(FILE *, FILE *, int);
void encode_message(FILE *, FILE *, int, char *, int, int);

int main(int argc, char **argv) {

  //If the program has less than two parameters returns 1 and stop
  if(argc < 3) {
    printf("Program requires 2 parameters to work properly; the message within \" \" and the PPM file you want to encode the message in.\nAborting\n.");
    return 1;
  }

  FILE *fp;
  //If the program couldn't open the file returns 1 and stop
  if((fp = fopen(argv[2], "r+")) == NULL) {
    printf("Could not open file %s.\nAborting\n", argv[2]);
    return 1;
  }

  struct rusage r_usage;
  struct timeval timer_usec2;
  struct timeval timer_usec;
  long long int timestamp_usec; 

  if(read_ppm_type(fp)) {

    /* Example of timestamp in microsecond. */

         /* timestamp in microsecond */
         if (!gettimeofday(&timer_usec, NULL)) {
           timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll + (long long int) timer_usec.tv_usec;
         }
         else {
           timestamp_usec = -1;
         }



    printf("\nStart aplication!\n");

    //Skip comments inside the ppm file
    skip_comments(fp);

    //Load message to be encrypted
    char *myMessage = (char *)argv[1];
	  //myMessage = " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    //Get message length
	  int message_length = get_message_length(myMessage);

    //Get file Width and Height
    int w = get_width(fp);
    int h = get_height(fp);

	  printf("\nImage sized defined\n");

    //Based on the image size defines if is possible to hide the message
    if(message_fits(message_length, w, h)) {
	    printf("\nImage capable of hidding message\n");

      //Read color depth of the image. If less than 255, returns 0
      //The algorithm is prepared to handle with Direct Colors (8 bits images)
      if(read_color_depth(fp)) {
        //Creates output file
      	FILE *fp_t = fopen("out.ppm","w");

        //In function
        int i = count_new_lines(fp);

        //In function
      	copy_header(fp, i, fp_t);
        encode_length(fp, fp_t, (message_length - 8) / 8);


      	encode_message(fp, fp_t, (message_length - 8), myMessage, w, h);


      	printf("Encoding Process Complete. Take a look at out.ppm\n");

        //Close files
      	fclose(fp);
      	fclose(fp_t);
      } else {
	        printf("\nError: Image color depth invalid. Must be 255.\n");
	        return 1;
      }
    } else {
      printf("\nError: Image file is not large enough to hold the message.\n\nAborting.\n");
      return 1;
    }
  } else {
    printf("Error: Wrong file format.\n\nAborting\n");
    return 1;
  }

     long long int timestamp_usec2; /* timestamp in microsecond */
     if (!gettimeofday(&timer_usec2, NULL)) {
       timestamp_usec2 = ((long long int) timer_usec2.tv_sec) * 1000000ll + (long long int) timer_usec2.tv_usec;
     }
     else {
       timestamp_usec2 = -1;
     }
     printf("\n\n%lld microssegundos.\n", timestamp_usec2 - timestamp_usec);


     getrusage(RUSAGE_SELF,&r_usage);
     printf("\n\nMemory usage: %ld bytes\n",r_usage.ru_isrss);

  return 0;
}

void copy_header(FILE *fp1, int numLines, FILE *fp2) {
  int i;
  char temp;

  //Goes back to the beginning of the file
  rewind(fp1);

  printf("\nCopying header\n");

  //Copies the header of the file
  for(i = 0; i < numLines; i++) {
    while((temp = fgetc(fp1)) != EOF && temp != '\n') {
      fputc(temp, fp2);
    }
    fputc('\n', fp2);
  }

  //printf("\nEnded copy_header\n\n");
  return;
}

//Get message length
int get_message_length(char my_msg[]) {
  int i = 0;
  while(my_msg[i] != '\0') {
    i++;
  }

  //Returns the number of bites necessary to encode the message
  //8 per char and jumps the first char (junk)
  return i * 8 + 8;
}

//Check if the message it's smaller than the file size
int message_fits(int length, int w, int h) {
  return length < w * h * 3;
}

//Count new lines on the header******
int count_new_lines(FILE *fp) {
  char temp;
  int count = 0;

  //Goes back to the beginning of the file
  rewind(fp);

  while((temp = fgetc(fp)) != EOF) {
	//printf("%d", count);
	if (count == 4) {
      return count;
    }
    if(temp == '\n') {
      count++;
	}
  }

  return count;
}

//
void encode_length(FILE *in, FILE *out, int length) {
  char temp;
  int i, l;

  printf("\nEncoding length\n");

  //Encodes the message length on the first byte ******
  //(l & 1) is the operator AND in binari
  for(i = 0; i < 8; i++) {
      temp = fgetc(in);
      l = length;
      l >>= 7 - i;
      if((l & 1) == 1) {
      	if((temp & 1) == 0) {
      	  temp++;
	       }
      } else {
      	if((temp & 1) == 1) {
      	  temp--;
      	}
      }
    fputc(temp, out);
  }
  printf("%d", length);

  return;
}

void encode_message(FILE *in, FILE *out, int num_to_encode, char* my_message, int w, int h) {
  int encoded = 0;
  unsigned char temp;
  int idx = 0, num_coppied = 0;
  char current;

  int fileSize = (w * h * 3) - 8; //Number of bytes after first 8
  int i;

  printf("\nEncoding message\n");

  //Starts encoding the message by going char by char of the file and changing
  //the less significant bit
  for(i = 0; i < fileSize; i++) {
    temp = fgetc(in);
    current = my_message[idx];

    current >>= 7 - num_coppied;
    num_coppied++;

    if(encoded <= num_to_encode) {
      encoded++;
      if((current & 1) == 1) {
      	if((temp & 1) == 0) {
      	  temp++;
      	}
      } else {
      	if((temp & 1) == 1) {
      	  temp--;
      	}
      }
      if(num_coppied == 8) {
      	idx++;
      	num_coppied = 0;
      }
    }

    fputc(temp, out);
  }

  return;
}
