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
  cmdMessenger.attach(kLoadGame, cmdLoadGame);
  cmdMessenger.attach(kShowConfig,ShowConfig);
  cmdMessenger.attach(kLoadDefaults,LoadDefaults);
}

void help()
{  
  dlog.flag_PeriodicReportEnable = false;
  Serial.println(F("#######################################################"));
  Serial.println(F("# Argument 0/-1  zeigt Wert ohne Aenderung an        #")); 
  Serial.println(F("# Temperaturen in Grad mit Vorzeichen                #"));
  Serial.println(F("#######################################################"));
  Serial.println(F(" 0;                      - diese Ausgabe")); 
  Serial.println(F(" 1,<setPeriodicReport>;  - Live-Daten Spam, 0..10: updates/sec"));
  Serial.println(F(" 2,<setAvgSamples>;      - samples für mittelwerte pro kanal, 1..4096")); 
  Serial.println(F(" 3,<setCellLowVolts>;    - UVP Einzelzelle[mV] ")); 
  Serial.println(F(" 4,<setCellHighVolts>;   - OVP Einzelzelle[mV]")); 
  Serial.println(F(" 5,<setCellmaxDiff>;     - max. Diff. Batt[mV]"));
  Serial.println(F("#######################################################"));
  Serial.println(F("# Savegame 0 sind die Reset-Defaults !!               #"));
  Serial.println(F("#######################################################"));
  Serial.println(F(" 6,<Save Config>          - Slot 0-4 Parameter speichern")); 
  Serial.println(F(" 7,<Load Config>          - Slot 0-4, Parameter laden")); 
  Serial.println(F(" 8,<Load Defaults>       - Factory defaults laden"));
  Serial.println(F(" 9,<Show Config>          - Slot 0-4"));
 
}
void setCellLowVolts()  {  Serial.println(F("setCellLowVolts:")); }
void setCellHighVolts() {  Serial.println(F("setCellHighVolts:")); }
void setCellmaxDiff()   {  Serial.println(F("setCellmaxDiff:")); }

void setAvgSamples()
{
  int temp = cmdMessenger.readInt16Arg();
  if (temp==0)
  {
    Serial.print(F("setAvgSamples:"));
    Serial.println(livedata.samplecount, DEC);
  }
  else
  {
    livedata.samplecount = temp;
    Serial.print(F("setAvgSamples:"));
    Serial.println(livedata.samplecount, DEC);
  }
};
void setPeriodicReport()
{ 
dlog.flag_PeriodicReportEnable = cmdMessenger.readInt16Arg();
Serial.print(F("flag_PeriodicReportEnable:"));
Serial.println(dlog.flag_PeriodicReportEnable, DEC);
};

/*------Storage------------*/
void SaveGame()
{
  
  int pos = 0;
  pos = cmdMessenger.readInt16Arg();
  size_t size = sizeof(CONFBUFFER);
  int offset = size * pos;
  Serial.print(F("Save Config ("));Serial.print(size,DEC);Serial.print(F("bytes) "));
  Serial.print(F("byte offset: ")); Serial.println(offset, DEC);
  Serial.println();

 // confgenReport(&akku);
  confgenMultiplaxParams(&akku, CONFBUFFER);

  for (size_t i = 0; i < size; i++)  
  {    
    EEPROM.write(i+offset,CONFBUFFER[i]);     
  }

   for (size_t i = 0; i < size; i++)  
  {    
    CONFBUFFER[i]=EEPROM.read(i+offset);     
  }

 // confgenDemultiplaxParams(&akku, CONFBUFFER);
  for (size_t i = 0; i < size; i++)  {    Serial.print(CONFBUFFER[i],HEX);      }
  Serial.println(F("..done"));
  //confgenReport(&akku);
  
}
void cmdLoadGame(){LoadGame(cmdMessenger.readInt16Arg());}
void LoadDefaults()
{
  confgenLoadDefaults(&akku);
  Serial.print(F("Load Defaults from Progmem:"));
  ShowConfig();
}
void ShowConfig()
{
  confgenReport(&akku);
}

#endif