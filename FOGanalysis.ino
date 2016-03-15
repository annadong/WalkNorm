#include <Wire.h>
int mpu = 0x68; //I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float prevresult = 0;
float result = 0;
float nextresult = 0;
float isMin[2] = {0,0};
float isMax[2] = {0,0};
float peakmeantime = 0;
float troughmeantime = 0;
float prevpeak = 0;
float prevtrough = 0;
float a = 0;
float b = 0;
float peakfreezing = 0;
float troughfreezing = 0;  
const float pi = 3.1415;
const int MOTOR = 9;
int speed = 0;
int i = 1;
float maximumValue = 0;
float minimumValue = 0;
float topThreshold = 0;
float lowThreshold = 0;
float curpeak = 0;
float curtrough = 0;
void setup() {
  Wire.begin();
  Wire.beginTransmission(mpu);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up the MPU-6050
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(MOTOR,OUTPUT);
}
void loop() {
  // Starting register of acceleration data
  Wire.beginTransmission(mpu);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(mpu,14,true);
  // << Bit shift left | Bitwise OR
  AcX = Wire.read() << 8|Wire.read();
  AcY = Wire.read() << 8|Wire.read();
  AcZ = Wire.read() << 8|Wire.read();
  Tmp = Wire.read() << 8|Wire.read();
  GyX = Wire.read() << 8|Wire.read();
  GyY = Wire.read() << 8|Wire.read();
  GyZ = Wire.read() << 8|Wire.read();
  
    //more variable declartions:
  
    float angle = atan2(AcX, AcZ)*180/pi;
    prevresult = result;
    result = nextresult;
    nextresult = angle+27;
    
    isMin[0] = isMin[1];
    if((result-prevresult < 0) && (result-nextresult < 0)){
      isMin[1] = 1;
    }  
    else{
      isMin[1] = 0;  
    } 
    isMax[0] = isMax[1];
    if((result-prevresult > 0) &&(result-nextresult > 0)){
      isMax[1] = 1;
    }  
    else{
      isMax[1] = 0;  
    }  
    if(isMin[1]==1 &&isMin[0]==0){
      troughmeantime = (i-prevtrough+troughmeantime*a)/(a+1);
      minimumValue = (result+minimumValue*a)/(a+1);
      if (i-prevtrough < troughmeantime*0.7)
        troughfreezing = troughfreezing + 1;
        //digitalWrite(13,HIGH);
        //delay(1000);
        //digitalWrite(13,LOW);
      prevtrough = i;
      a = a+1;  
    }
    if(isMax[0]==1 && isMax[1]==0){
      peakmeantime = (i-prevpeak+peakmeantime*b)/(b+1);
      maximumValue = (result + maximumValue*b)/(b+1);
      topThreshold = (maximumValue - minimumValue)*0.75;
      lowThreshold = (maximumValue - minimumValue)*0.25;
      if (i-prevpeak < peakmeantime*0.7)
        peakfreezing = peakfreezing + 1;
        //digitalWrite(13,HIGH);
        //delay(1000);
        //digitalWrite(13,LOW);
      if ((peakfreezing == troughfreezing)&&(i>50))  {
        speed = 250;
        analogWrite(MOTOR,speed);
        delay(500);
        speed = 0;
        analogWrite(MOTOR,speed);
      }
      if ((lowThreshold < (curpeak - curtrough)) && ((curpeak - curtrough) <topThreshold) && i >50){
        speed = 250;
        analogWrite(MOTOR,speed);
        delay(500);
        speed = 0;
        analogWrite(MOTOR,speed);
      }
      prevpeak = i; 
      b = b+1;  
    }
    if (result > (maximumValue-minimumValue)*(1.5) && i > 50){
      speed = 250; 
        delay(500);
        analogWrite(MOTOR,speed);
        speed = 0;
        analogWrite(MOTOR,speed);
    }
    analogWrite(MOTOR,speed);
    i++;
    Serial.print(prevresult); Serial.print(" ");
    Serial.print(result); Serial.print(" ");
    Serial.print(nextresult); Serial.print(" ");
    Serial.print(i); Serial.print(" ");
    Serial.print(isMax[0]); Serial.print(" ");
    Serial.print(isMax[1]); Serial.println(" ");
    Serial.print(peakfreezing); Serial.print(" ");
    Serial.print(troughfreezing); Serial.print(" ");
    Serial.print(troughmeantime); Serial.print(" ");
    Serial.print(peakmeantime); Serial.println(" ");
    
    delay(33);
}
