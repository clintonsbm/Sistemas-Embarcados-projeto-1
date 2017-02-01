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
#include <plib/usart.h>

void copy_header(char *, int, char *);
int get_message_length(char[]);
int message_fits(int, int, int);
int count_new_lines(char *);
void encode_length(char *, char *, int);
void encode_message(char *, int, char *);

void putch(unsigned char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}

void UART_Init(const long int baudrate)
{
    unsigned int x;
    x =  (_XTAL_FREQ/baudrate/64) - 1;            //SPBRG for Low Baud Rate

    SPBRG = x;                                    //Writing SPBRG Register
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    TRISC7 = 1;                                   //As Prescribed in Datasheet
    TRISC6 = 1;                                   //As Prescribed in Datasheet
    CREN = 1;                                     //Enables Continuous Reception
    TXEN = 1;                                     //Enables Transmission
}

/*
 void putch(char dataByte) {
    while (!PIR1bits.TX1IF);
    TXREG1 = dataByte;
}
 */
int main(int argc, char **argv) {
    // Configure USART
    /*OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_OFF &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_CONT_RX,
            25);
            
    /*
     if (argc < 3) {
        printf("Program requires 2 parameters to work properly; the message within \" \" and the PPM file you want to encode the message in.\nAborting\n.");
        return 1;
    }
     */
    
    init_uart();
    printf("Start project\n");
    
    /*
     char *myFileChars;
    myFileChars = "������������������������������������������������������������������������������������������������������������������������������������������������   ���������   ���   ������������������������������������������������������������������������         ???      ������   ���������������������������������������������������������������������         ???         ���������������������������������������������������������������������������         ???               ���������������������������������������������������������������������         ???               ������������������������������������������������������������������      ???      ??????      ???   ������������������������������������������������������������            OOOOOOOOO   OOOOOO      ������������������������������������������������������������      ???   jlljlljll   jlljll   ???   ���������������������������������������������������������         jll         ���      jll???   ������������������������������������������������������                  ???            jll      ������������������������������������������������������         OOOOOOjlljll         ???jll      ������         ������������������������������������������������   jlljll???????????????jlljll������      jlljll���������������������������������������������                                 ������   jlljlljll���������������������������������������               ???���������OOO            ������jlljll   ������������������������������������      ?????????OOO???OOOOOOOOOOOOOOOOOO??????      jlljll���������������������������������������   ??????      OOO???OOOOOOOOO???OOOOOO            jlljll���������������������������������������   OOOOOOOOO                           OOOOOO   OOO      ���������������������������������������   OOOOOOOOOjll   OOO                  jllOOOOOOOOOOOO���������������������������������������      OOOOOOOOOjll   OOO�#               jll   OOOOOO   ���������������������������������������      jll            OOO                              ������������������������������������������      jlljll            OOOOOOOOOOOO                  ������������������������������������������      jll         ����#   OOOOOO   k� OOO            ���������������������������������������������   jll      jll                                    �������������������������������������#�#�#   jll      jll      OOOOOOOOO                     ����������������������������������#�#�#               jll   OOO         OOO      jll            �������������������������#����#���                        OOO                  jll            ����������������������#����#������                           OOO   OOO                        ����������������������#���������                  jll         OOO   OOO   OOO                  ����������������#�#������������                  jllOOOOOO   OOO   OOO      OOOOOO            �������������#�#���������������         ������                                       ������������������������������������������������������������������������������������������������������������������";

    //Load lentgth from parameters
    char *len = (char *) argv[3];
    int length = atoi(len);
    
    //Load message from parameters
    char *myMessage = (char *) argv[1];

    encode_message(myFileChars, length, myMessage);

    printf("Encoding Process Complete.");
     */
    

    return 0;
}

void encode_message(char *fileChars, int num_to_encode, char* my_message) {
    int encoded = 0;
    unsigned char temp;
    int idx = 0, num_coppied = 0;
    char current;
    
    int fileSize = sizeof(*fileChars);
    int i;

    printf("\nEncoding message\n");

    //Starts encoding the message by going char by char of the file and changing
    //the less significant bit
    for (i = 0; i < fileSize; i++) {
        temp = fileChars;
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

        WriteUSART(temp);
    }

    return;
}

