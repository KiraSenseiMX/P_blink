/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define _GNU_SOURCE
#define CHAN_CONFIG_SINGLE  8
#define HIGH 1
#define LOW 0

#define PIN18 1 // wiringPi pin 1 is BCM_GPIO 18.

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <ncurses.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
 
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int myFd ;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void spiSetup (int spiChannel)
{
    if ((myFd = wiringPiSPISetup (spiChannel, 1000000)) < 0)
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int main ()
{
    int loadSpi       = 0; // 0:false; 1:true
    int channelConfig = CHAN_CONFIG_SINGLE;
    int analogChannel = 0;
    int spiChannel    = 0;
    int analogRead    = 0;
    int iterator      = 0;
    double period     = 0.0;
    double freq       = 0.0;

    wiringPiSetup () ;
    spiSetup(spiChannel);
    
    pinMode (PIN18, OUTPUT);
 
    printf("\nProgram START. Press any key to exit.\n");
    initscr();     //in ncurses
    timeout(0);
    
    while(!iterator)
    {
		
/********************/
/*** Main Program ***/
		
        analogRead = myAnalogRead(spiChannel,channelConfig,0) + 1;
        period = (double)(1.0/analogRead); // in ms
        freq   = 1.0/(2*period);
        
        printf("freq = %03.2f\r",freq);
        
        digitalWrite (PIN18, HIGH);
        delay (period*1000);
        digitalWrite (PIN18, LOW);
        delay (period*1000);
        
/*** Main Program ***/
/********************/
        
        iterator = getch();
        if (iterator>0) {iterator=1;} else {iterator=0;}
    }
    
    endwin();
    printf("\nProgram END.\n");

    close (myFd);
    return 0;
}
