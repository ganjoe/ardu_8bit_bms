
#include <functions.h>

void setup() {

  analogReference(INTERNAL2V56);  // a built-in 2.56V reference (Arduino Mega only) 

  BATTPARAMS battery;
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
  // put your main code here, to run repeatedly:
}