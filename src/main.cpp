
#include <functions.h>
#include <CmdMessenger.h>

//#define ARDU10BITADC
//#define ARDU12BITADC
//#define ADS111x
#define TEST


BATTPARAMS akku;
DATALOGGA dlog; 
BATTDATA livedata;  // die null ist wichtig

 
CmdMessenger cmdMessenger = CmdMessenger(Serial);

void setup() {
 

  //analogReference(INTERNAL2V56);  // a built-in 2.56V reference (Arduino Mega only) 
  Serial.begin(9600);
  cmdMessenger.printLfCr();   // Adds newline to every command
  attachCommandCallbacks();
  dlog.flag_PeriodicReportEnable = 1;
  livedata.samplecount =1024;
  
  
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
  Serial.print("setup done");

}

void loop() 
{
 
 
  avgVoltages     (&livedata, &akku);
  calcCellVoltage (&livedata, &akku);
  scaleCellVoltage(&livedata, &akku);
  printLog        (&livedata, &akku);

  cmdMessenger.feedinSerialData();
  //_delay_ms(500);
  // put your main code here, to run repeatedly:
}

void printLog(BATTDATA *log, BATTPARAMS *params)
{
  if(dlog.flag_PeriodicReportEnable){
  Serial.println("---REPORT: LOGOBJEKT---");
  Serial.println("!!! Type '0;' for help !!!");

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
  for (size_t i = 0; i < params->cellcount; i++)
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
/*-------Command Messenger----------*/
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(khelp, help);
  cmdMessenger.attach(kPeriodicReport, setPeriodicReport);
  cmdMessenger.attach(ksetAvgSamples, setAvgSamples);
  cmdMessenger.attach(kPeriodicReport, setPeriodicReport);
  cmdMessenger.attach(ksetCellLowVolts, setCellLowVolts);
  cmdMessenger.attach(ksetCellHighVolts, setCellHighVolts);
  cmdMessenger.attach(ksetCellmaxDiff, setCellmaxDiff);
}

void help()
{  
  dlog.flag_PeriodicReportEnable = false;
  Serial.println(" 0;                     - diese Ausgabe"); 
  Serial.println(" 1,<setPeriodicReport>  - Live-Daten Spam, 0..10: updates/sec");
  Serial.println(" 2,<setAvgSamples>      - samples für mittelwerte pro kanal, 0..4096"); 
  Serial.println(" 3,<setCellLowVolts>    - UVP Einzelzelle[mV], 0: aktueller Wert "); 
  Serial.println(" 4,<setCellHighVolts>   - OVP Einzelzelle[mV], 0: aktueller Wert "); 
  Serial.println(" 5,<setCellmaxDiff>     - max. Diff. Batt[mV], 0: aktueller Wert ");
  Serial.println(" 6,<setAvgSamples>     - max. Diff. Batt[mV], 0: aktueller Wert "); 
 
}
void setCellLowVolts(){  Serial.println("Help:"); }
void setCellHighVolts(){  Serial.println("Help:"); }
void setCellmaxDiff(){  Serial.println("Help:"); }
void setAvgSamples()
{ 
livedata.samplecount = cmdMessenger.readInt16Arg();
Serial.print("setAvgSamples:");
Serial.println(livedata.samplecount, DEC);
};

void setPeriodicReport()
{ 
dlog.flag_PeriodicReportEnable = cmdMessenger.readInt16Arg();
Serial.print("flag_PeriodicReportEnable:");
Serial.println(dlog.flag_PeriodicReportEnable, DEC);
};





