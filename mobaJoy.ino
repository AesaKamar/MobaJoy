
#include <Wire.h>
#include <ArduinoNunchuck.h>
#include <math.h>

#define BAUDRATE 19200

ArduinoNunchuck nunchuck = ArduinoNunchuck();

int zeroedX = 130;
int zeroedY = 130;

int change;

int x;
int y;
double r;
double theta;

void setup()
{
  
  Serial.begin(BAUDRATE);
  nunchuck.init();
  Mouse.begin();
  
}

void loop()
{
  
  nunchuck.update();
  
  x = nunchuck.analogX - zeroedX;
  y = nunchuck.analogY - zeroedY;
  r = polarDistance(x,y);
  theta = polarAngle(x, y, r);
  
  
  Serial.print("Rect: " );
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print("\t");
  Serial.print("Polar: ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print(theta);
  Serial.print("\t");
  Serial.print("C: ");
  Serial.print(nunchuck.cButton);
  Serial.print("\t");
  Serial.print("Z: ");
  Serial.print(nunchuck.zButton);
  Serial.println(' ');
  
 if(r < 5)
 {
     x=0;
     y=0;
     r=0;
     theta=0;
     
  }else{
     Mouse.move(scaledPosition(x), -scaledPosition(y));
     change += pow(scaledPosition(x),2) + pow(scaledPosition(y),2);
     
  }
 if(nunchuck.cButton || change > 100)
 {
     Mouse.click(MOUSE_RIGHT);
     change = 0;
 }
}

double polarDistance(int x, int y)
{
    return sqrt(pow(x,2) + pow(y,2));
}

double polarAngle(int x, int y, double r)
{
    double angle;
    if(r < 10){
      return 0;
    }
 
    if(y>0){
      angle = acos(x/r);
    }else{
      angle = abs(asin(y/r)) + M_PI;
    }
    
    return toDegrees(angle);
}

double toDegrees(double radian)
{
    double degree;
    degree = radian * 180 / M_PI;
    return degree;
}

double scaledPosition(int pos)
{
   int scaled;
   scaled = (.00002666 * pow(pos,3)) + (-.016666666 * pos);
   return scaled;
}
  

