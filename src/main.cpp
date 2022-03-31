
#include <functions.h>

//#define ARDU10BITADC
//#define ARDU12BITADC
//#define ADS111x
#define TEST


BATTPARAMS akku;

void setup() {

  // initialize GDB stub


  //analogReference(INTERNAL2V56);  // a built-in 2.56V reference (Arduino Mega only) 
  Serial.begin(9600);

  
  akku.cell_max_temperature = 5500;    //in milli-Grad
  akku.cell_min_temperature = -0500;
  akku.cell_min_voltage = 3000;        //in milli-Volts
  akku.cell_max_voltage = 3500;
  akku.cell_max_diff = 5;
  akku.cellcount = 8;

  akku.hallFaktor[CURRENT_IN] =  1;
  akku.hallFaktor[CURRENT_OUT] = 1;

  akku.resFaktor[CELL01] = 1L;
  akku.resFaktor[CELL02] = 1L;
  akku.resFaktor[CELL03] = 1L;
  akku.resFaktor[CELL04] = 1L;
  akku.resFaktor[CELL05] = 1L;
  akku.resFaktor[CELL06] = 1L;
  akku.resFaktor[CELL07] = 1L;
  akku.resFaktor[CELL08] = 1L;
  
}

void loop() 
{
  BATTDATA livedata = {0};  // die null ist wichtig
  livedata.samplecount =2048;

  avgVoltages     (&livedata, &akku);
  calcCellVoltage (&livedata, &akku);
  scaleCellVoltage(&livedata, &akku);
  printLog        (&livedata, &akku);
  //_delay_ms(500);
  // put your main code here, to run repeatedly:
}

void printLog(BATTDATA *log, BATTPARAMS *params)
{
  Serial.print("---REPORT: LOGOBJEKT(");
  Serial.print(log->samplecount,DEC);
  Serial.println(")---");
 
  for (size_t i = 0; i < params->cellcount; i++)
    {
      Serial.print(i,DEC);
      Serial.print(":\t");
      Serial.println(log->raw_CellVolts[i]); 
      // Serial.println(log->raw_stringVolts[i]); 
    }
    BATTSTAUS status = {0};
     status =updateStatus(log, params);
     Serial.print("lowest Cell:");  Serial.println(status.lowest_cell, DEC);
     Serial.print("highest Cell:"); Serial.println(status.highest_cell, DEC);
     Serial.print("diff cell:");    Serial.println(status.diff_cell, DEC);
     Serial.print("enable:");       Serial.println(status.status, DEC);
 
}

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

RTNCODE avgVoltages (BATTDATA *log, BATTPARAMS *params)
{

for (size_t i = 0; i < log->samplecount; i++)
{
    for (size_t i = 0; i < params->cellcount; i++)
    {
     log->raw_stringVolts[i] += getVoltage(i); 
    }   
}

for (size_t i = 0; i < params->cellcount; i++)
{
    log->raw_stringVolts[i] /= log->samplecount;
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
  for (size_t i = 0; i < batt->samplecount; i++)
  {
    batt->raw_CellVolts[i] *= params->resFaktor[i];
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