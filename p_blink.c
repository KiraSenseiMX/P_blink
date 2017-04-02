/*
 *  readMcp3008.c:
 *  read value from ADC MCP3008
 *
 * Requires: wiringPi (http://wiringpi.com)
 * Copyright (c) 2015 http://shaunsbennett.com/piblog
 ***********************************************************************
 */
#define _GNU_SOURCE
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <ncurses.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
 
#define CHAN_CONFIG_SINGLE  8

 
static int myFd ;

void spiSetup (int spiChannel)
{
    if ((myFd = wiringPiSPISetup (spiChannel, 1000000)) < 0)
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}

int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}


int main ()
{
    int loadSpi       = 0; // 0:false; 1:true
    int channelConfig = CHAN_CONFIG_SINGLE;
    int analogChannel = 0;
    int spiChannel    = 0;
    int analogRead    = 0;

    wiringPiSetup () ;
    spiSetup(spiChannel);
    
    int i=0;
 
    printf("\nProgram START.\n");
    initscr();     //in ncurses
    timeout(0);
    
/********************/
/*** Main Program ***/
    
    while(!i)
    {
        analogRead = myAnalogRead(spiChannel,channelConfig,0);
        printf("analogRead = %d\n\r",analogRead);
        i=getch();
        printw("%d ",i);
        if (i>0) {i=1;} else {i=0;}
    }
    
/*** Main Program ***/
/********************/
    
    endwin();
    printf("\nProgram END.\n");

    close (myFd);
    return 0;
}
