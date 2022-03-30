
#include <functions.h>

void setup() {

  analogReference(INTERNAL2V56);  // a built-in 2.56V reference (Arduino Mega only) 
  Serial.begin(9600);

  
  battery.cell_max_temperature = 5500;
  battery.cell_min_temperature = -0500;
  battery.cell_min_voltage = 3000;
  battery.cell_min_voltage = 3600;
  battery.cellcount = 8;

  RESISTOR_CALIBRATION divider;
  divider.hallFaktor[CURRENT_IN] =  234354;
  divider.hallFaktor[CURRENT_OUT] = 321431;
 // RESISTOR_CALIBRATION calib;
  
}



void loop() 
{
  LOGOBJEKT newdata ={0};
  measureVoltages(&newdata, 3);
  printLog(&newdata);
  _delay_ms(500);
  // put your main code here, to run repeatedly:
}

void printLog(LOGOBJEKT *log)
{
Serial.println("---REPORT: LOGOBJEKT---");
for (size_t i = 0; i < battery.cellcount; i++)
{
  Serial.print(i,DEC);
  Serial.print(":\t");
  Serial.println(log->cellvoltage[i]); 
}

 
}

RTNCODE measureVoltages (LOGOBJEKT *log, unsigned int averaging_Samples)
{

for (size_t i = 0; i < averaging_Samples; i++)
{
    for (size_t i = 0; i < battery.cellcount; i++)
    {
     log->cellvoltage[1] += analogRead(i); 
    }   
}

for (size_t i = 0; i < battery.cellcount; i++)
{
    log->cellvoltage[i] /= averaging_Samples;
}

return OK;
}