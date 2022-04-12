#ifndef functions_h
#define functions_h

#include <datatypes.h>

RTNCODE calcCellVoltage     (BATTDATA *batt, BATTPARAMS *params);
RTNCODE avgVoltages         (BATTDATA *log, BATTPARAMS *params);
RTNCODE scaleCellVoltage    (BATTDATA *batt, BATTPARAMS *params);
BATTSTAUS updateStatus      (BATTDATA *batt, BATTPARAMS *params);
void    printLog            (BATTDATA *batt, BATTPARAMS *params);

unsigned long getVoltage (int channel);

/*-------Command Messenger----------*/
void attachCommandCallbacks();
void help();
void cmdSetPeriodicReport();
void cmdSetAvgSamples();
void cmdSetLowHighDiff();
void cmdSetTempLowHigh();
void cmdSetAdcScaler();
void cmdSaveGame();
void cmdLoadGame();
void cmdShowConfig();
void cmdLoadDefaults();

/*------Storage------------*/
int confgenSaveGame(uint8_t* buffer, int pos);
int confgenMultiplaxParams(BATTPARAMS *batt, uint8_t* buffer);
int confgenDemultiplaxParams(BATTPARAMS *batt, uint8_t* buffer);
void confgenLoadDefaults(BATTPARAMS *batt);
void confgenReport(BATTPARAMS *batt);

#endif



