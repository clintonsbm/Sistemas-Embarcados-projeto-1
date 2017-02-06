/* 
 * File:   writeMsg.c
 * Author: LiveG
 *
 * Created on 25 de Janeiro de 2017, 17:08
 */
#include <stdlib.h>
#include <stdio.h>
//#include "stego.h"

#include <xc.h>
#include <string.h>
//#include <plib/usart.h>

#pragma config FOSC     = HS    /// EXTERN CLOCK 8MHZ
#pragma config WDT      = OFF   /// DISABLE WATCHDOG TIMER
#pragma config WDTPS    = 32768 /// WATCHDOG TIMER 32768s
#pragma config PBADEN   = OFF   /// PORTB.RB0,1,2,3,4 AS I/O DIGITAL
#pragma config MCLRE = OFF
#define _XTAL_FREQ 8000000

void encode_message(char *, int, char *);
void decode_message(unsigned char fileChars[256], int length);

void putch(unsigned char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}

void UART_Init(const long int baudrate){
    unsigned int x;
    x = (_XTAL_FREQ/baudrate/64) - 1;            //SPBRG for Low Baud Rate

    SPBRG = x;                                    //Writing SPBRG Register
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    TRISC7 = 1;                                   //As Prescribed in Datasheet
    TRISC6 = 1;                                   //As Prescribed in Datasheet
    CREN = 1;                                     //Enables Continuous Reception
    TXEN = 1;                                     //Enables Transmission
}

void UART_Stop(){
    SPBRG = 0;                                    //Writing SPBRG Register
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 0;                                     //Enables Serial Port
    TRISC7 = 0;                                   //As Prescribed in Datasheet
    TRISC6 = 0;                                   //As Prescribed in Datasheet
    CREN = 0;                                     //Enables Continuous Reception
    TXEN = 0;                                     //Enables Transmission
}

int main() {
    UART_Init(9600);
    
    //printf("Start.\n");
    
    //Entrada messagen
    //static unsigned char fileChars[1992] = "џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ   ВВГВВГВВГ   ВВГ   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ         ???      ВВГВВГ   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ         ???         ВВГВВГџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ         ???               џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ         ???               џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ      ???      ??????      ???   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ            OOOOOOOOO   OOOOOO      џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ      ???   jlljlljll   jlljll   ???   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ         jll         ВВГ      jll???   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ                  ???            jll      џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ         OOOOOOjlljll         ???jll      џџџџџџ         џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ   jlljll???????????????jlljllџџџџџџ      jlljllџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ                                 џџџџџџ   jlljlljllџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ               ???ВВГВВГВВГOOO            џџџџџџjlljll   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ      ?????????OOO???OOOOOOOOOOOOOOOOOO??????      jlljllџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ   ??????      OOO???OOOOOOOOO???OOOOOO            jlljllџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ   OOOOOOOOO                           OOOOOO   OOO      џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ   OOOOOOOOOjll   OOO                  jllOOOOOOOOOOOOџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ      OOOOOOOOOjll   OOOџ#               jll   OOOOOO   џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ      jll            OOO                  ";
    
    //Entrada codificada
    static unsigned char fileCharsEncoded[1992] = "ўџџџўџўўўџџўўџўџўџџџўўџџўџџџўџўўўџџўўџўџўўўўўўўўўўџўўџўџўџџўўўџџўўўўўўўўўўџўўџўџўџџџўўџџўўўўўўўўўўџўўџўџўџџўўџўўўўўўўўўўўўўўўўўўџџџџџџџџџџџџџџџџ!!!ГГГГГГГГГ!!!ГГГ!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!???!!!!!!ГГГГГГ!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!???!!!!!!!!!ГГГГГГџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!???!!!!!!!!!!!!!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!???!!!!!!!!!!!!!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!???!!!!!!??????!!!!!!???!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!!!!OOOOOOOOO!!!OOOOOO!!!!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!???!!!kmmkmmkmm!!!kmmkmm!!!???!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!kmm!!!!!!!!!ГГГ!!!!!!kmm???!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!!!!!!!!!!???!!!!!!!!!!!!kmm!!!!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!!!!OOOOOOkmmkmm!!!!!!!!!???kmm!!!!!!џџџџџџ!!!!!!!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!kmmkmm???????????????kmmkmmџџџџџџ!!!!!!kmmkmmџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџўўџўўўўўўў!       !     ! !!  ! ! !!  ! ! !џўўџџџ !!jlmjmlkmlўџўџўџџўўџџџўџџўўџўџўџџўўџўџўџџўўџџўўџў      !       !>>>ВВГВГГВВГNON!!  ! ! !!  џџўўџўjlljll!  ўўўўўџўўўўўџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!?????????OOO???OOOOOOOOOOOOOOOOOO??????!!!!!!kmmkmmџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!??????!!!!!!OOO???OOOOOOOOO???OOOOOO!!!!!!!!!!!!kmmkmmџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!OOOOOOOOO!!!!!!!!!!!!!!!!!!!!!!!!!!!OOOOOO!!!OOO!!!!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!OOOOOOOOOkmm!!!OOO!!!!!!!!!!!!!!!!!!kmmOOOOOOOOOOOOџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!OOOOOOOOOkmm!!!OOOџ#!!!!!!!!!!!!!!!kmm!!!OOOOOO!!!џџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџџ!!!!!!kmm!!!!!!!!!!!!OOO!!!!!!!!!!!!!!!!!!";
    
    //Mensagem
    //char *myMessage = "teste";
    
    //Funчуo encode
    //encode_message(fileChars, 5, myMessage);

    //printf("Encoding Process Complete");
    
    printf("Decode message");
    
    //Funчуo decode
    decode_message(fileCharsEncoded, 5);
    
    UART_Stop();
    return 0;
}

void encode_message(unsigned char fileChars[1992], int num_to_encode, char* my_message) {
    int encoded = 0;
    unsigned char temp;
    int idx = 0, num_coppied = 0;
    char current;
    
    int fileSize = strlen(fileChars);
    int i;
    
    printf("%d", fileSize);
    printf("COMECOU");

    for (i = 0; i < fileSize; i++) {
        temp = fileChars[i];
        current = my_message[idx];

        current >>= 7 - num_coppied;
        num_coppied++;

        if (encoded <= num_to_encode) {
            encoded++;
            if ((current & 1) == 1) {
                if ((temp & 1) == 0) {
                    temp++;
                }
            } else {
                if ((temp & 1) == 1) {
                    temp--;
                }
            }
            if (num_coppied == 8) {
                idx++;
                num_coppied = 0;
            }
        }
             
        //printf("%c", temp); 
    }
    
    printf("ACABOU");
    return;
}

void decode_message(unsigned char fileChars[1992], int length) {
  int readTo = length * 8, numRead = 0, i;
  unsigned char charBuffer = 0;
  char temp;
  int idx = 0;
  char secret_message[5];
  
  int fileSize = strlen(fileChars);
  
  printf("%d", fileSize);
  printf("COMECOU");
  
  while(numRead <= readTo) {
    temp = fileChars[numRead];
    if(numRead % 8 == 0) {
      //secret_message[idx] = charBuffer;
      printf("%c", charBuffer);
      idx++;
      charBuffer = 0;
    } else {
      charBuffer <<= 1;
    }
    charBuffer |= (temp & 1);
    numRead++;
    
    printf("%c", secret_message[i]);
  }

  for(i=0;i<length;i++) {
      //printf(" qtaconstesendo ");
     
  }
  
  printf("ACABOU");
}

