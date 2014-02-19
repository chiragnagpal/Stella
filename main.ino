
#include <Wire.h> //I2C Arduino Library

#define HMC5883L_ADDR 0x1E //0011110b, I2C 7bit address of HMC5883

boolean flag = LOW;
char azi[6];

void setup()
{
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  Serial.println("GY271 TEST");
  Wire.begin();
  // lower I2C clock http://www.gammon.com.au/forum/?id=10896
  //TWBR = 78;  // 25 kHz 
 // TWSR |= _BV (TWPS0);  // change prescaler  

pinMode(PUSH2, INPUT_PULLUP);
attachInterrupt(PUSH2, star_val, FALLING);
pinMode(2,OUTPUT);
}

void loop()
{
   if (flag){
     Serial.println("Enter Desired Azimuth:") ;
   int i = 0;
   while((Serial.available() <6));
   
   azi[0]=Serial.read();
   azi[1]=Serial.read();
   azi[2]=Serial.read();
   azi[3]=Serial.read();
   azi[4]=Serial.read();
   azi[5]=Serial.read();


 
 Serial.print(azi[0]);
 Serial.print(azi[1]);
 Serial.print(azi[2]);
 Serial.print(azi[3]);
 Serial.print(azi[4]);
 Serial.println(azi[5]);

 
 Serial.flush();
 flag = LOW;
 }

      Wire.beginTransmission(HMC5883L_ADDR); //open communication with HMC5883
      Wire.write(0x02); //select mode register
      Wire.write(0x00); //continuous measurement mode
      Wire.endTransmission();
  
int x,y,z; //triple axis data
float heading;
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

 //Read data from each axis, 2 registers per axis
 
  Wire.requestFrom(HMC5883L_ADDR, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.print(z);
 Serial.print('\t');
 float angle= atan2((float)y,(float)x) * (180 / 3.14159265) + 180;
  Serial.println(angle);

//if (angle<azi+1&&angle>azi-1)
//digitalWrite(2,HIGH);
//else
digitalWrite(2,LOW);


  delay(250);
  
}

void star_val(){
  
 flag = HIGH;
  
}
