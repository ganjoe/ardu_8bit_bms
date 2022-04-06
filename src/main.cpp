
#include <functions.h>
#include <CmdMessenger.h>
#include <EEPROM.h>
#include <utils_buffer.h>


//#define ARDU10BITADC
//#define ARDU12BITADC
//#define ADS111x
#define CONFBUFFER buffer
#define FLOAT_SCALE 100.0
#define PRINTPREC 6
#define TEST
 
CmdMessenger cmdMessenger = CmdMessenger(Serial);

void setup() {
 

  //analogReference(INTERNAL2V56);  // a built-in 2.56V reference (Arduino Mega only) 
  Serial.begin(115200);
  cmdMessenger.printLfCr();   // Adds newline to every command
  attachCommandCallbacks();
  dlog.flag_PeriodicReportEnable = 1;
  livedata.samplecount =2048;
  
  
  akku.cell_max_temperature = (55.123456  );    //in milli-Grad
  akku.cell_min_temperature = (5.123456  );
  akku.cell_min_voltage = 3000;        //in milli-Volts
  akku.cell_max_voltage = 3500;
  akku.cell_max_diff = 5;
  akku.cellcount = 8;

  akku.hallFaktor[CURRENT_IN] =  1.0001;
  akku.hallFaktor[CURRENT_OUT] = 1.0002;

  akku.resFaktor[CELL01] = (1.01  );
  akku.resFaktor[CELL02] =  (1.02  );
  akku.resFaktor[CELL03] =  (1.03  );
  akku.resFaktor[CELL04] =  (1.04  );
  akku.resFaktor[CELL05] =  (1.05  );
  akku.resFaktor[CELL06] =  (1.06  );
  akku.resFaktor[CELL07] =  (1.07  );
  akku.resFaktor[CELL08] =  (1.08 );
  Serial.print("setup done");

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
  Serial.println("---REPORT: LOGOBJEKT---");
  Serial.println("!!! Type '0;' for help !!!");
  Serial.print("safegame bytes:");
  Serial.println(sizeof(akku));
  Serial.print("Samplecount:");
  Serial.println(log->samplecount,DEC);

 
  for (size_t i = 0; i < params->cellcount; i++)
    {
      Serial.print(i,DEC);
      Serial.print(":\t");
      Serial.println(log->raw_CellVolts[i]); 
      //Serial.println(log->raw_stringVolts[i]); 
    }
    BATTSTAUS status = {0};
     status =updateStatus(log, params);
     Serial.print("lowest Cell:");  Serial.println(status.lowest_cell );
     Serial.print("highest Cell:"); Serial.println(status.highest_cell);
     Serial.print("diff cell:");    Serial.println(status.diff_cell);
     Serial.print("enable:");       Serial.println(status.status);
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

for (size_t i = 0; i < log->samplecount; i++)
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
    log->raw_stringVolts[i] /= log->samplecount +1;
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
  cmdMessenger.attach(kSaveGame, SaveGame);
  cmdMessenger.attach(kShowConfig,ShowConfig);
}

void help()
{  
  dlog.flag_PeriodicReportEnable = false;
  Serial.println("#######################################################");
  Serial.println("# Argument 0/-1  zeigt Wert ohne Aenderung an        #"); 
  Serial.println("# Temperaturen in Kelvin angeben 273 == 0°           #");
  Serial.println("#######################################################");
  Serial.println(" 0;                      - diese Ausgabe"); 
  Serial.println(" 1,<setPeriodicReport>;  - Live-Daten Spam, 0..10: updates/sec");
  Serial.println(" 2,<setAvgSamples>;      - samples für mittelwerte pro kanal, 1..4096"); 
  Serial.println(" 3,<setCellLowVolts>;    - UVP Einzelzelle[mV] "); 
  Serial.println(" 4,<setCellHighVolts>;   - OVP Einzelzelle[mV]"); 
  Serial.println(" 5,<setCellmaxDiff>;     - max. Diff. Batt[mV]");
  Serial.println("#######################################################");
  Serial.println("# Der letzte Savegameaufruf setzt die Reset-Defaults  #");
  Serial.println("#######################################################");
  Serial.println(" 6,<Save Config>          - Slot 1-3, Parameter speichern"); 
  Serial.println(" 7,<Load Config>          - Slot 1-3, Parameter laden"); 
  Serial.println(" 8,<Restore Config>       - Progmem defaults laden");
  Serial.println(" 9,<Show Config>          - aktive Konfig listen");
 
}
void setCellLowVolts()  {  Serial.println("setCellLowVolts:"); }
void setCellHighVolts() {  Serial.println("setCellHighVolts:"); }
void setCellmaxDiff()   {  Serial.println("setCellmaxDiff:"); }

void setAvgSamples()
{
  int temp = cmdMessenger.readInt16Arg();
  if (temp==0)
  {
    Serial.print("setAvgSamples:");
    Serial.println(livedata.samplecount, DEC);
  }
  else
  {
    livedata.samplecount = temp;
    Serial.print("setAvgSamples:");
    Serial.println(livedata.samplecount, DEC);
  }
};
void setPeriodicReport()
{ 
dlog.flag_PeriodicReportEnable = cmdMessenger.readInt16Arg();
Serial.print("flag_PeriodicReportEnable:");
Serial.println(dlog.flag_PeriodicReportEnable, DEC);
};

/*------Storage------------*/
void SaveGame()
{
  
  int pos = 0;
  pos = cmdMessenger.readInt16Arg();
  size_t size = sizeof(CONFBUFFER);
  int offset = size * pos;
  Serial.print("Save Config (");Serial.print(size,DEC);Serial.print("bytes) ");
  Serial.print("byte offset: "); Serial.println(offset, DEC);

  confgenReport(&akku);
  confgenMultiplaxParams(&akku, CONFBUFFER);

  for (size_t i = 0; i < size; i++)  
  {    
    EEPROM.write(i+offset,CONFBUFFER[i]);     
  }
   for (size_t i = 0; i < size; i++)  
  {    
    CONFBUFFER[i]=EEPROM.read(i+offset);     
  }


  confgenDemultiplaxParams(&akku, CONFBUFFER);
 // for (size_t i = 0; i < size; i++)  {    Serial.print(CONFBUFFER[i],HEX);      }
  Serial.println();
  confgenReport(&akku);
  

}
void ShowConfig()
{
  confgenReport(&akku);
}

void confgenReport(BATTPARAMS *thisbatt)
{
  Serial.print("batt->cellcount:");             Serial.println(thisbatt->cellcount,DEC);
  Serial.print("batt->cell_min_voltage:");      Serial.println(thisbatt->cell_min_voltage,DEC);
  Serial.print("batt->cell_max_voltage:");      Serial.println(thisbatt->cell_max_voltage,DEC);
  Serial.print("batt->cell_min_temperature:");  Serial.println(thisbatt->cell_min_temperature,PRINTPREC);
  Serial.print("batt->cell_max_temperature:");  Serial.println(thisbatt->cell_max_temperature,PRINTPREC);
  Serial.print("batt->cell_max_diff:");         Serial.println(thisbatt->cell_max_diff,DEC);
  Serial.print("batt->resFaktor[CELL01]:");     Serial.println(thisbatt->resFaktor[CELL01],PRINTPREC);
  Serial.print("batt->resFaktor[CELL02]:");     Serial.println(thisbatt->resFaktor[CELL02],PRINTPREC);
  Serial.print("batt->resFaktor[CELL03]:");     Serial.println(thisbatt->resFaktor[CELL03],PRINTPREC);
  Serial.print("batt->resFaktor[CELL04]:");     Serial.println(thisbatt->resFaktor[CELL04],PRINTPREC);
  Serial.print("batt->resFaktor[CELL05]:");     Serial.println(thisbatt->resFaktor[CELL05],PRINTPREC);
  Serial.print("batt->resFaktor[CELL06]:");     Serial.println(thisbatt->resFaktor[CELL06],PRINTPREC);
  Serial.print("batt->resFaktor[CELL07]:");     Serial.println(thisbatt->resFaktor[CELL07],PRINTPREC);
  Serial.print("batt->resFaktor[CELL08]:");     Serial.println(thisbatt->resFaktor[CELL08],PRINTPREC);
  Serial.print("batt->resFaktor[CURRENT_IN]:"); Serial.println(thisbatt->hallFaktor[CURRENT_IN],DEC);
  Serial.print("batt->resFaktor[CURRENT_OUT]:");Serial.println(thisbatt->hallFaktor[CURRENT_OUT],DEC);
}

int confgenMultiplaxParams(BATTPARAMS *thisbatt, uint8_t* buffer)
{
  int32_t ind = 0;
  buffer_append_int32     (buffer,thisbatt->cellcount ,&ind);
  buffer_append_int32     (buffer,thisbatt->cell_min_voltage ,&ind);
  buffer_append_int32     (buffer,thisbatt->cell_max_voltage ,&ind);
  buffer_append_int32     (buffer,thisbatt->cell_max_diff ,&ind);
  buffer_append_float16   (buffer,thisbatt->cell_min_temperature,FLOAT_SCALE ,&ind);
  buffer_append_float16   (buffer,thisbatt->cell_max_temperature,FLOAT_SCALE ,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL01] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL02] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL03] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL04] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL05] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL06] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL07] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->resFaktor[CELL08] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->hallFaktor[CURRENT_IN] ,FLOAT_SCALE,&ind);
  buffer_append_float16   (buffer,thisbatt->hallFaktor[CURRENT_OUT] ,FLOAT_SCALE,&ind);
  return sizeof(buffer);
  //5.4.22
}

int confgenDemultiplaxParams(BATTPARAMS *thisbatt, uint8_t* buffer)
{
  int32_t ind = 0;
  thisbatt->cellcount =             buffer_get_int32(buffer, &ind);
  thisbatt->cell_min_voltage =      buffer_get_int32(buffer, &ind);
  thisbatt->cell_max_voltage =      buffer_get_int32(buffer, &ind);
  thisbatt->cell_max_diff =         buffer_get_int32(buffer, &ind);
  thisbatt->cell_min_temperature =  buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->cell_max_temperature =  buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[0] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[1] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[2] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[3] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[4] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[5] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[6] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->resFaktor[7] =          buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->hallFaktor[0] =         buffer_get_float16(buffer,FLOAT_SCALE, &ind);
  thisbatt->hallFaktor[1] =         buffer_get_float16(buffer,FLOAT_SCALE, &ind);

 
  return sizeof(buffer);
  //5.4.22
}



