#include <datatypes.h>

extern BATTPARAMS battery;

RTNCODE measureVoltages (LOGOBJEKT log)
{
    for (size_t i = 0; i < battery.cellcount; i++)
    {
     log.cellvoltage[1] = analogRead(i); 
    }
    

return OK;
}