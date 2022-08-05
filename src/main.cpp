
#include <datatypes.h>
#include <functions.h>
#include <EEPROM.h>
#include <utils_buffer.h>
#include <confgen.h>
#include <commands.h>
//lala


//#define ARDU10BITADC
//#define ARDU12BITADC
//#define ADS111x


#define TEST
 


void setup() {
 

  //analogReference(INTERNAL2V56);  // a built-in 2.56V reference (Arduino Mega only) 
  Serial.begin(115200);
  cmdMessenger.printLfCr();   // Adds newline to every command
  attachCommandCallbacks();
  dlog.flag_PeriodicReportEnable = 1;
  livedata.samplecount =2048;
  LoadGame(0);
  Serial.println("setup done");

}

void loop() 
{
  avgVoltages     (&livedata, &akku);
  calcCellVoltage (&livedata, &akku);
  scaleCellVoltage(&livedata, &akku);
  printLog        (&livedata, &akku);

  cmdMessenger.feedinSerialData();
  //_delay_ms(1500);
  // put your main code here, to run repeatedly:
}

void printLog(BATTDATA *log, BATTPARAMS *params)
{
  if(dlog.flag_PeriodicReportEnable){
  Serial.println(F("---REPORT: LOGOBJEKT---"));
  Serial.println(F("!!! Type '0;' for help !!!"));
  Serial.print(F("safegame bytes:"));
  Serial.println(sizeof(akku));
  Serial.print(F("Samplecount:"));
  Serial.println(params->samplecount,DEC);

 
  for (size_t i = 0; i < params->cellcount; i++)
    {
      Serial.print(i,DEC);
      Serial.print(":\t");
      Serial.println(log->raw_CellVolts[i]); 
      //Serial.println(log->raw_stringVolts[i]); 
    }
    BATTSTAUS status = {0};
     status =updateStatus(log, params);
     Serial.print(F("lowest Cell:"));  Serial.println(status.lowest_cell );
     Serial.print(F("highest Cell:")); Serial.println(status.highest_cell);
     Serial.print(F("diff cell:"));    Serial.println(status.diff_cell);
     Serial.print(F("enable:"));       Serial.println(status.status);
  }
 
}

/*-------Sensors----------*/

unsigned long getVoltage (int channel)
{
  #ifdef ARDU10BITADC
    return analogRead(channel); 
  #endif
  #ifdef ARDU12BITADC
    return analogRead(channel); 
  #endif
  #ifdef ADS111x
    return analogRead(channel); 
  #endif
  #ifdef TEST
    return random(3450,3520)*((unsigned int)channel+1);
  #endif
}

/*-------Calculations----------*/


RTNCODE avgVoltages (BATTDATA *log, BATTPARAMS *params)
{

for (size_t i = 0; i < params->samplecount; i++)
{ 
    for (size_t i = 0; i < params->cellcount; i++)
    {
     log->raw_stringVolts[i] += getVoltage(i); 
    }   
}

for (size_t i = 0; i < params->cellcount; i++)
{
  /*
  stringVolts hat noch den alten wert, der muss durch "+1" in der mittelwertbildung berücksichtig werden
  wird stringVolts vorher auf 0 gesetzt entfällt +1
  ohne "+1" muss auch die 0 alls illegaler wert abgefangen werden
  */
    log->raw_stringVolts[i] /= params->samplecount +1;
}

return OK;
}

RTNCODE calcCellVoltage (BATTDATA *batt, BATTPARAMS *params)
{
for (size_t i = params->cellcount; i > 0; i--)
{
   batt->raw_CellVolts[i] =  batt->raw_stringVolts[i] - batt->raw_stringVolts[i-1];
}  
batt->raw_CellVolts[0] =  batt->raw_stringVolts[0];
return OK;
}

RTNCODE scaleCellVoltage(BATTDATA *batt, BATTPARAMS *params)
{
  for (size_t i = 0; i < params->cellcount; i++)
  {
    //batt->raw_CellVolts[i] *= params->resFaktor[i];
  }
return OK;
}

BATTSTAUS updateStatus(BATTDATA *batt, BATTPARAMS *params)
{
  BATTSTAUS status = {0};
  status.status = 1;
  unsigned long temp = 0;

  for (size_t i = 0; i < params->cellcount; i++)
  {
    if (temp < batt->raw_CellVolts[i])
    {
      temp = batt->raw_CellVolts[i];
    }
  }
  status.highest_cell = temp;

  for (size_t i = 0; i < params->cellcount; i++)
  {
    if (temp > batt->raw_CellVolts[i])
    {
      temp = batt->raw_CellVolts[i];
    }
  }
  status.lowest_cell = temp;

  status.diff_cell = abs(status.highest_cell - status.lowest_cell);

  if (status.highest_cell >= params->cell_max_voltage)
    status.status = 0;
  if (status.lowest_cell <= params->cell_min_voltage)
    status.status = 0;
  if (status.diff_cell >= params->cell_max_diff)
    status.status = 0;

  return status;
}



/*-------Command Messenger----------*/







