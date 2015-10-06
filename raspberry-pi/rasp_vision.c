/*
 *  rasp_vision.c
 *
 *  Copyright (C) 2015  Cgroup
 */

/*
 * This program makes the exchange of information between the Android smartphone
 * and the STM32 control processor. UDP packets with a fixed length of 3 bytes
 * containing control commands arrive and are retransmited to the processor via 
 * I2C if the value has changed.
 */

#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#define UDP_PORT       2016
#define SLAVE_ADDRESS  0x20

/* Pauses for i milliseconds */
void delay(int i)    
{
        clock_t start, end;
        start = clock();
        while ( (end = clock() - start ) <= i * CLOCKS_PER_SEC / 1000);
}

int main(void)
{
        /** UDP initialization */
        int s;
        char paquet_snd[3];
        char paquet_rcv[3];
        int err_send, err_recv,er;
        struct sockaddr_in adServeur, adClient;
        unsigned int ladd = sizeof(adClient);
        int port = UDP_PORT;
        
        s = socket(AF_INET, SOCK_DGRAM, 17);
        perror("socket");

        adServeur.sin_family = AF_INET;
        adServeur.sin_port = htons (port);
        adServeur.sin_addr.s_addr = INADDR_ANY;

        er = bind (s, (struct sockaddr *) &adServeur, sizeof(adServeur));
        perror("bind");
        
        /** I2C initialization */
        int file;
        int adapter_nr = 1;
        char filename[20];
        char buff_rcv[3];
        char buff_snd[3];       
        char buff_snd_prev[3] = { '0', '0', '0'};
        
        /** Option */
        //paquet_rcv[0] = 'r';
        //paquet_rcv[1] = 'c';
        //paquet_rcv[2] = 'v';
        
        clock_t start, end;
        
        snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
        file = open(filename, O_RDWR);
        
        if (file < 0) {
                printf("file error");
                exit(1);
        }

        int addr = SLAVE_ADDRESS;

        if ( ioctl(file, I2C_SLAVE, addr) < 0) {
                printf("error io");
                exit(1);
        }
        
        start = clock();

        do
        {
                buff_snd[0] = '0';
                buff_snd[1] = '0';
                buff_snd[2] = '0';
                buff_rcv[0] = '0';
                buff_rcv[1] = '0';
                buff_rcv[2] = '0';
                paquet_rcv[0] = '0';
                paquet_rcv[1] = '0';
                paquet_rcv[2] = '0';
                paquet_snd[0] = '0';
                paquet_snd[1] = '0';
                paquet_snd[2] = '0';
                
               /** UDP reception */
                err_recv = recvfrom( s, paquet_rcv, 3, 0, 
                       (struct sockaddr *) &adClient,&ladd);
                printf ("UDP rcv: %c %c %c\n", paquet_rcv[0], 
                        paquet_rcv[1], paquet_rcv[2]);
        
               /** I2C emission*/
                buff_snd[0] = paquet_rcv[0];
                buff_snd[1] = paquet_rcv[1];
                buff_snd[2] = paquet_rcv[2];
                
                if ( (int) buff_snd[1] - (int) buff_snd_prev[1] > 20
                        || (int) buff_snd[1] - (int) buff_snd_prev[1] < - 20) {
                        if (write(file, buff_snd, 3) != 3) {
                                printf("I2C write error");
                        }

                        buff_snd_prev[0] = buff_snd[0];
                        buff_snd_prev[1] = buff_snd[1];
                        buff_snd_prev[2] = buff_snd[2];
                }
                
                end = clock();

                /* 
                 * Sends motor information to the Android device every 50ms and 
                 * sends OK message every 5ms.
                 */
                if( (end - start) > 50 * CLOCKS_PER_SEC / 1000) {

                        // if (read(file,buff_rcv, 3) != 3) {
                        //      printf("error read");
                        // } else {
                        //      printf("buff_rcv = %c %c %c\n", buff_rcv[0], 
                        //              buff_rcv[1],buff_rcv[2]);
                        // }
                        
                        buff_rcv[0] = (unsigned char) 0x04;
                        buff_rcv[1] = (unsigned char) 0x00;
                        buff_rcv[2] = (unsigned char) 0x00; 
                        
                        paquet_snd[0] = buff_rcv[0];
                        paquet_snd[1] = buff_rcv[1];
                        paquet_snd[2] = buff_rcv[2];

                        err_send = sendto(s, paquet_snd, 3, 0,
                                (struct sockaddr *) &adClient, ladd);

                        start = clock();

                } else {

                        paquet_snd[0] = (char) 0x04;
                        paquet_snd[1] = 'K';
                        paquet_snd[2] = 'Y';

                        err_send = sendto(s, paquet_snd, 3, 0,
                                (struct sockaddr *) &adClient, ladd);
                }
                delay(10);
        }
        while ( (strncmp( (char *) paquet_rcv, "bye", 3)));
        printf("Disconnected\n"); 
        close(s);
        return 0 ;
}
