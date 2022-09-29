#include<LiquidCrystal_I2C.h>
#include<Servo.h>

//LCD Variable Object
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Servo Motor Object
Servo servoMotor;

//UltraSonic Pins
int echo = A0;
int trig = 2;

//Servo Motor Pin
int servoPin = 3;

//Calibration Varibales
long maximum = 0;
long minimum = 99999999;

void setup() {
  //LCD Initialization
  lcd.init();

  //LCD Functions
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("JLEGACY-AI...");

  //UltraSonic Pin Setup
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);

  //Servo Motor Pin Setup
  servoMotor.attach(servoPin);

  //caliberation
  caliberationSensor();

  //Printing Calibeartion Values
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mini: ");
  lcd.print(minimum);
  lcd.setCursor(0,1);
  lcd.print("Maxi: ");
  lcd.print(maximum);

  servoMotor.write(0);
}

void loop() {
  delay(1000);

  long distance = getDistance(10);

  //Printing Distance
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.print(distance);

  //Mapping Angle
  int angle = map(distance, minimum, maximum, 0, 180);
  lcd.setCursor(0,1);
  
  if(angle >=0 && angle <=180){
    lcd.print("Angle: ");
    lcd.print(angle);
    servoMotor.write(angle);
  }else{
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Angle Out :(");  
  }
    
}

long getDistance(int microSecond){

  //Triggering Ultrasonic
  digitalWrite(trig, HIGH);
  delayMicroseconds(microSecond);
  digitalWrite(trig, LOW);

  //Reading Ultrasonic Back
  long duration = pulseIn(echo,HIGH);

  //Calculating Distance
  long distance =  (duration*0.034)/2;

  //Returning Distance Back
  return distance;  
}

void caliberationSensor(){
  //Calibaration Setup
  while(millis() < 5000){

    //getting distance
    long distance = getDistance(10);

    //fr maximum Distance
    if(distance > maximum)
      maximum = distance;

    //for minimum Distance
    if(distance < minimum)
      minimum = distance;
    
  }  
}
