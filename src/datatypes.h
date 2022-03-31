/*
Wiederstandswerte für spannungsteiler. Reihe E12
zweigstrom 100uA normiert auf 1024mV ADC und 4V zelle
2k7+27k         /   10k
1k8+68k         /   10k
10k+100k        /   10k
150k            /   10k
10k+180k        /   10k
560k||390k      /   10k
270k            /   10k
1500k||390k     /   10k
*/

#include <Arduino.h>

typedef struct 
{
    unsigned int cellcount;
    unsigned int cell_min_voltage;
    unsigned int cell_max_voltage;
    unsigned int cell_min_temperature;
    unsigned int cell_max_temperature;
}BATTPARAMS;

typedef struct 
{
    unsigned long cellvoltage[8];
    unsigned long hallvoltage[2];
    unsigned long temperature[2];
    unsigned long unixtime;
    unsigned long samplecount;
    /* data */
}LOGOBJEKT;

typedef struct 
{
    long resFaktor[8];
    long hallFaktor[2];
}RESISTOR_CALIBRATION;

typedef enum 
{
    CURRENT_IN,
    CURRENT_OUT,
}CURRENTSENSOR;

typedef enum 
{
    CELL01,    CELL02,    CELL03,    CELL04,    CELL05,    CELL06,    CELL07,    CELL08,
}CELLNAMES;

typedef enum 
{
    NOT_OK,
    OK,
    TRUNC,  //wertebereich wurde durch truncate eingeschränkt
    TIME,   // timeout der peripherie
}RTNCODE;

