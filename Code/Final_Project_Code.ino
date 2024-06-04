/*
 * Include all the libraries
 */
#include <DHT.h>
#include <LiquidCrystal.h>
#include <Servo.h>

/*
 * Declare all the variables
 */

#define Type DHT11
int TempHumSensorPin = 24;
DHT HumidityTemp(TempHumSensorPin, Type);

/*
 * Pin numbers for the sensors, LCD, servo motor and LEDs
 */
int trigPin = 26;
int echoPin = 28;
int redLED = 2;
int greenLED = 3;
int yellowLED = 22;
int servoMotorPin = 4;
int RS = 8;
int E = 9;
int D4 =10;
int D5 =11;
int D6 =12;
int D7 =13;

/*
 * To store the values from the sensors
 */
int soundSensor = A0;
int waterSensor = A1;
int sensorValueMm = 0;
String sensorValueDisplay; 
String stringWaterLevel = "Water level: ";
String stringMillimeter = "mm";

/*
 * To determine the current status of the box
 */
int loudSound = 50;
int hotTemp = 30;
int coldTemp = 10;
int wetBox = 1;

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
Servo myServoMotor;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  myServoMotor.attach(servoMotorPin);
  lcd.begin(16, 2);
  HumidityTemp.begin(); 
  pinMode(soundSensor, INPUT);
  pinMode(waterSensor, INPUT);

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  
  pinMode(echoPin, INPUT);
  
  long x = pulseIn(echoPin, HIGH);
  long cm = ((x/2)/29.1);

  /*
   * Print the new readings every 0.5 second
   */ 
  Serial.print("The distance is: ");
  Serial.println(cm);

  float humidity = HumidityTemp.readHumidity();
  float temperature = HumidityTemp.readTemperature();
  int soundValue = analogRead(soundSensor);

  Serial.print("The temperature is: ");
  Serial.println(temperature);

  Serial.print("The humidity is: ");
  Serial.println(humidity);  

  Serial.print("The sound is: ");
  Serial.println(soundValue);

  /*
   * Display the converted value in millimeters
   */
  int waterValue = analogRead(waterSensor);
  sensorValueMm = map(waterValue, 0, 360, 0, 41);
  sensorValueDisplay = stringWaterLevel + sensorValueMm + stringMillimeter; 
  Serial.println(sensorValueDisplay);

  delay(500);

  /*
   * Check if a child is going to play then
   * turned ON the green LED
   */ 
  if (cm < 10 && cm != 0)
  {
    Serial.println("Fivs is here.");

    lcd.setCursor(0, 0);
    lcd.print("Hello Fivs!");
    delay(500);

    /*
     * Rotate the servo motor to 90 degrees
     * to display the 3d printed logo
     */ 
    for(int a = 0; a <= 90; a+=2) {
      myServoMotor.write(a);
      
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);     
    }

    /*
     * Check the current state of the box if it is 
     * either be hot, cold, noisy or wet then turned
     * ON the red LED
     */ 
    if (temperature > hotTemp) { 
      Serial.print("The box is hot: ");
      Serial.println(temperature);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Box is hot!");
      delay(500);
      lcd.clear();

      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);  

    } else if (temperature < coldTemp) {
      Serial.print("The box is cold: ");
      Serial.println(temperature);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Box is cold!");
      delay(500);
      lcd.clear();

      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW); 

    } else if (soundValue > loudSound) {
      Serial.print("The box is noisy: ");
      Serial.println(soundValue);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Box is noisy!");
      delay(500);
      lcd.clear();

      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW); 

    } else if (sensorValueMm > 0) {
      Serial.print("The box is wet: ");
      Serial.println(sensorValueMm);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Box is wet!");
      delay(500);
      lcd.clear();

      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW); 
    }
  } else {

    Serial.println("The box is ok.");

    lcd.clear();

    for(int a = 90; a >= 0; a-=2) {
      myServoMotor.write(a);
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, HIGH);
      delay(500); 
    }
  } 
}
