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
  cmdMessenger.attach(khelp,            help);
  cmdMessenger.attach(kPeriodicReport,  cmdSetPeriodicReport);
  cmdMessenger.attach(ksetAvgSamples,   cmdSetAvgSamples);
  cmdMessenger.attach(ksetLowHighDiff,  cmdSetLowHighDiff);
  cmdMessenger.attach(ksetTempLowHigh,  cmdSetTempLowHigh);
  cmdMessenger.attach(ksetAdcScaler,    cmdSetAdcScaler);
  cmdMessenger.attach(kSaveGame,        cmdSaveGame);
  cmdMessenger.attach(kLoadGame,        cmdLoadGame);
  cmdMessenger.attach(kLoadDefaults,    cmdLoadDefaults);
  cmdMessenger.attach(kShowConfig,      cmdShowConfig);

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
  Serial.println(F(" 3,<setLowHighDiff>;     - Limits für jede Zelle[mV].. ")); 
  Serial.println(F("    ...Diff ist die Maximale Abweichung im Akku")); 
  Serial.println(F(" 4,<setTempLowHigh>;     - Limits für Zelltemperatur "));
  Serial.println(F(" 5,<setAdcScaler>;       - Faktor für ADC[bits]->mV... "));
  Serial.println(F("    ...<Zelle 0-7><(float)Faktor>... "));  
  Serial.println(F("#######################################################"));
  Serial.println(F("# Savegame 0 wird stets nach Reset geladen            #"));
  Serial.println(F("#######################################################"));
  Serial.println(F(" 6,<Save Config>          - Slot 0-4 Parameter speichern")); 
  Serial.println(F(" 7,<Load Config>          - Slo1t 0-4, Parameter laden")); 
  Serial.println(F(" 8,<Load Defaults>        - Factory defaults laden"));
  Serial.println(F(" 9,<Show Config>          - Slot 0-4"));
 
}
void cmdSetLowHighDiff()   
{  
  akku.cell_min_voltage = cmdMessenger.readInt16Arg();;
  akku.cell_max_voltage =cmdMessenger.readInt16Arg();
  akku.cell_max_diff = cmdMessenger.readInt16Arg();
  Serial.println(F("setLowHighDiff:")); 
  Serial.print(F("Lower Limit:"));  Serial.println(akku.cell_min_voltage);
  Serial.print(F("Max Diff:"));     Serial.println(akku.cell_max_diff);
  Serial.print(F("Upper Limit:"));  Serial.println(akku.cell_max_voltage);
}
void cmdSetTempLowHigh()   
{  
  akku.cell_min_temperature = cmdMessenger.readInt16Arg();
  akku.cell_max_temperature =cmdMessenger.readInt16Arg();

  Serial.println(F("SetTempLowHigh:")); 
  Serial.print(F("Lower Temperature Limit:"));  Serial.println(akku.cell_min_temperature);
  Serial.print(F("Upper Temperature Limit:"));  Serial.println(akku.cell_max_temperature);
}
void cmdSetAdcScaler()     
{
  int pos = cmdMessenger.readInt16Arg();
  float faktor = cmdMessenger.readFloatArg();
  akku.resFaktor[pos] = faktor;
  for (size_t i = 0; i < akku.cellcount; i++)
  {
    Serial.println(akku.resFaktor[i],6);    
  }
  Serial.println(F("...done")); 
  
}

void cmdSetAvgSamples()
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
void cmdSetPeriodicReport()
{ 
dlog.flag_PeriodicReportEnable = cmdMessenger.readInt16Arg();
Serial.print(F("flag_PeriodicReportEnable:"));
Serial.println(dlog.flag_PeriodicReportEnable, DEC);
};

/*------Storage------------*/
void cmdSaveGame()
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
void cmdLoadDefaults()
{
  confgenLoadDefaults(&akku);
  Serial.print(F("Load Defaults from Progmem:"));
  cmdShowConfig();
}
void cmdShowConfig()
{
  confgenReport(&akku);
}

#endif