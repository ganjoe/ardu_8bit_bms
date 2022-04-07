#ifndef commands_h
#define commands_h


#include <CmdMessenger.h>
#include <datatypes.h>
#include <functions.h>
#include <EEPROM.h>

CmdMessenger cmdMessenger = CmdMessenger(BTUART);

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

#endif