#include <datatypes.h>

RTNCODE calcCellVoltage     (BATTDATA *batt, BATTPARAMS *params);
RTNCODE avgVoltages         (BATTDATA *log, BATTPARAMS *params);
RTNCODE scaleCellVoltage    (BATTDATA *batt, BATTPARAMS *params);
BATTSTAUS updateStatus      (BATTDATA *batt, BATTPARAMS *params);
void    printLog            (BATTDATA *batt, BATTPARAMS *params);
unsigned long getVoltage    (int channel);

/*-------Command Messenger----------*/
void attachCommandCallbacks();

void help();
void setPeriodicReport();
void setAvgSamples();
void setCellLowVolts();
void setCellHighVolts();
void setCellmaxDiff();




