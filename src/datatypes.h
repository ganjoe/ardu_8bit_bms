/*Wiederstandswerte für spannungsteiler.
 Reihe E12
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


#ifndef _datatypes_h
#define _datatypes_h
#include <Arduino.h>


#define KELVIN  273150  //milli-Kelvin
#define FLOAT_SCALE 100.0
#define PRINTPREC 6
#define BTUART Serial
#define CONFBUFFER buffer


enum COMMANDS
{
  khelp,                // Command to request list of available commands
  kPeriodicReport, 
  ksetAvgSamples,
  ksetCellLowVolts,      // Command to request led to be set in specific state  
  ksetCellHighVolts,     // Command to request led to be set in to specific brightness  
  ksetCellmaxDiff,       // Command to request led status
  kSaveGame,
  kLoadGame,
  kLoadDefaults,
  kShowConfig,

  
};

typedef struct 
{
 unsigned long lowest_cell;
 unsigned long highest_cell;
 unsigned long diff_cell;
 unsigned long status;
}BATTSTAUS;

typedef struct 
{
    unsigned long   cellcount;
    unsigned long   cell_min_voltage;
    unsigned long   cell_max_voltage;
    unsigned long   cell_max_diff;
    float           cell_min_temperature;
    float           cell_max_temperature;
    float           resFaktor[8];
    unsigned long hallFaktor[2];
    unsigned long samplecount; 
}BATTPARAMS;

typedef struct 
{
    unsigned long unixtime;
    unsigned long samplecount;    
    unsigned long raw_stringVolts[8];   //gemittelte adc-werte
    unsigned long raw_CellVolts[8];   //
    unsigned long hallvoltage[2];
    unsigned long temperature[2];

    
    /* data */
}BATTDATA;

typedef struct 
{
    int flag_PeriodicReportEnable;
}DATALOGGA;

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



BATTPARAMS akku;
DATALOGGA dlog; 
BATTDATA livedata;  // die null ist wichtig
uint8_t buffer[sizeof(BATTPARAMS)];

#endif