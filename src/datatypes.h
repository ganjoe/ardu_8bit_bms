#include <Arduino.h>


typedef struct 
{
    long cellvoltage[8];
    int temperature[2];
    long unixtime;
    /* data */
}LOGOBJEKT;

typedef struct 
{
    long faktor[8];
}RESISTOR_CALIBRATION;

