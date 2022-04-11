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


#endif