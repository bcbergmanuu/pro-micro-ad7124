#include <Arduino.h>

#include <NHB_AD7124.h>

#define csPin 10

Ad7124 adc(csPin, 4000000);

void calibrate()
{
  delay(100);
  adc.setAdcControl(AD7124_OpMode_Idle, AD7124_FullPower, false);
  delay(100);
  adc.enableChannel(0);
  delay(100);
  adc.setAdcControl(AD7124_OpMode_SystemOffsetCalibration, AD7124_FullPower, false);
  delay(100);
  adc.enableChannel(0);
  delay(100);
  adc.setAdcControl(AD7124_OpMode_Continuous, AD7124_FullPower, false);
  delay(100);
}

void init_adc() {
  adc.setup[0].setConfig(AD7124_Ref_Avdd, AD7124_Gain_128, true);

// The filter select bits determine the filtering and ouput data rate
  //     1 = Minimum filter, Maximum sample rate
  //  2047 = Maximum filter, Minumum sample rate
  //
  adc.setup[0].setFilter(AD7124_Filter_SINC4, 2047, AD7124_PostFilter_dB92);
  
  adc.setChannel(0, 0, AD7124_Input_AIN6, AD7124_Input_AIN7, true);


  adc.setAdcControl(AD7124_OpMode_Continuous, AD7124_FullPower, false);
}

void setup()
{
  pinMode(A0, INPUT_PULLUP);
  Serial.begin(500000);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("AD7124");

  // Initializes the AD7124 device
  adc.begin();

  
  init_adc();
}

void loop()
{     
  Serial.println(adc.readRaw(0)-0x800000);

  if (Serial.available())
  {
    int inByte = Serial.read();
    if (inByte == 'c')
    {
      calibrate();
    }
  }
  
}