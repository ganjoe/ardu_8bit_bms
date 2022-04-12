#ifndef confgen_h
#define confgen_h

#include <datatypes.h>
#include <utils_buffer.h>



void confgenReport(BATTPARAMS *thisbatt)
{
  Serial.print(F("batt->cellcount:"));             Serial.println(thisbatt->cellcount,DEC);
  Serial.print(F("batt->cell_min_voltage:"));      Serial.println(thisbatt->cell_min_voltage,DEC);
  Serial.print(F("batt->cell_max_voltage:"));      Serial.println(thisbatt->cell_max_voltage,DEC);
  Serial.print(F("batt->cell_min_temperature:"));  Serial.println(thisbatt->cell_min_temperature,PRINTPREC);
  Serial.print(F("batt->cell_max_temperature:"));  Serial.println(thisbatt->cell_max_temperature,PRINTPREC);
  Serial.print(F("batt->cell_max_diff:"));         Serial.println(thisbatt->cell_max_diff,DEC);
  Serial.print(F("batt->resFaktor[CELL01]:"));     Serial.println(thisbatt->resFaktor[CELL01],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL02]:"));     Serial.println(thisbatt->resFaktor[CELL02],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL03]:"));     Serial.println(thisbatt->resFaktor[CELL03],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL04]:"));     Serial.println(thisbatt->resFaktor[CELL04],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL05]:"));     Serial.println(thisbatt->resFaktor[CELL05],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL06]:"));     Serial.println(thisbatt->resFaktor[CELL06],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL07]:"));     Serial.println(thisbatt->resFaktor[CELL07],PRINTPREC);
  Serial.print(F("batt->resFaktor[CELL08]:"));     Serial.println(thisbatt->resFaktor[CELL08],PRINTPREC);
  Serial.print(F("batt->resFaktor[CURRENT_IN]:")); Serial.println(thisbatt->hallFaktor[CURRENT_IN],DEC);
  Serial.print(F("batt->resFaktor[CURRENT_OUT]:"));Serial.println(thisbatt->hallFaktor[CURRENT_OUT],DEC);
  Serial.print(F("batt->samplecount"));            Serial.println(thisbatt->samplecount, DEC);
  Serial.println();
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
  buffer_append_int16     (buffer,thisbatt->samplecount,&ind);
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
  thisbatt->samplecount =           buffer_get_int16(buffer, &ind);

 
  return sizeof(buffer);
  //5.4.22
}



void confgenLoadDefaults(BATTPARAMS *thisbatt)
{
 thisbatt->cell_max_temperature = (55.123456  );    //in milli-Grad
  thisbatt->cell_min_temperature = (5.123456  );
  thisbatt->cell_min_voltage = 3000;        //in milli-Volts
  thisbatt->cell_max_voltage = 3500;
  thisbatt->cell_max_diff = 5;
  thisbatt->cellcount = 8;

  thisbatt->hallFaktor[CURRENT_IN] =  1.0001;
  thisbatt->hallFaktor[CURRENT_OUT] = 1.0002;

  thisbatt->resFaktor[CELL01] = (1.00001  );
  thisbatt->resFaktor[CELL02] =  (1.02  );
  thisbatt->resFaktor[CELL03] =  (1.03  );
  thisbatt->resFaktor[CELL04] =  (1.04  );
  thisbatt->resFaktor[CELL05] =  (1.05  );
  thisbatt->resFaktor[CELL06] =  (1.06  );
  thisbatt->resFaktor[CELL07] =  (1.07  );
  thisbatt->resFaktor[CELL08] =  (1.08 );
  thisbatt->samplecount = 2048;
}

void LoadGame(int pos)
{
 
size_t size = sizeof(CONFBUFFER);
int offset = size * pos;
Serial.print(F("Load Config from Offset:"));
Serial.println(offset);

//BATTPARAMS testakku = {0};

   for (size_t i = 0; i < size; i++)  
  {    
    CONFBUFFER[i]=EEPROM.read(i+offset);     
  }
  confgenDemultiplaxParams(&akku, CONFBUFFER);
  for (size_t i = 0; i < size; i++)  
  {    Serial.print(CONFBUFFER[i],HEX);}
  Serial.println();
  cmdShowConfig();
  Serial.println(F("..done"));
}
#endif