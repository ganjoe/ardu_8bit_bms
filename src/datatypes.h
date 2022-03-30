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
    uint16_t cellvoltage[8];
    uint16_t hallvoltage[2];
    int temperature[2];
    long unixtime;
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

