#include "Dispenser_lib.h"
#include "Parameters.h"
#include "pinout.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pthread.h>

/*
    * C Program to Get IP Address
    */

// char array[] = "wlan0";
char array[] = "eth0";
char * get_IP(){
            int n;
    struct ifreq ifr;
    n = socket(AF_INET, SOCK_DGRAM, 0);

    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name, array , IFNAMSIZ - 1);
    ioctl(n, SIOCGIFADDR, &ifr);
    close(n);

    //display result
    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

void display_IP(char address[]){
    // Extract the first token
    char * token = strtok(address, ".");
    // printf(meme);
    pthread_t t_id;
    // SS_print(meme);
    // // pthread_create(&t_id, NULL, SS_print, token);
    // sleep(1);
    // SSDon = 0;
    // pthread_join(t_id, NULL);
    
    // loop through the string to extract all other tokens
    while( token != NULL ) {
        // copy to memory
        char *meme = malloc(strlen(token));
        strcpy(meme, token);

        printf("%s\n", token); //printing each token
        pthread_create(&t_id, NULL, SS_print, meme);
        sleep(2);
        SSDon = 0;
        pthread_join(t_id, NULL);
        token = strtok(NULL, ".");

        free(meme);
    }
    
}

void setup(){
    // num[0] = "0000";
    gpioInitialise();
    SSDon = 0;
    pthread_mutex_init(&lock, NULL);
}

int main(){
    setup();
    char* IP = get_IP();
    printf("%s IP Address: %s\n", array, IP);

    display_IP(IP);
    gpioTerminate();
    


}