#include <Servo.h>

#include <LiquidCrystal.h>

Servo myServo;

int const potPin = A0;

int potVal;

int angle;

int tempSensorVal;

int lightSensorVal;

int sun;

int gain;

int sp;

int fan;

const int lightSensorPin = A1;

const int redLed = A2;

const int yellowLed = A3;

const int greenLed = A4;

const int tempSensorPin = A5;

const int motorPin = 6;

const float lowerTemp = 20.0;

const float upperTemp = 30.0;

// LCD dislay parameters

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{

    myServo.attach(9);

    pinMode(potPin, INPUT);

    pinMode(lightSensorPin, INPUT);

    pinMode(tempSensorPin, INPUT);

    pinMode(motorPin, OUTPUT);

    pinMode(greenLed, OUTPUT);

    pinMode(yellowLed, OUTPUT);

    pinMode(redLed, OUTPUT);

    lcd.begin(16, 2);

    lcd.print("Temp Sun Ang Fan");

    Serial.begin(9600);
}

void loop()
{

    // Temperature Calculation

    tempSensorVal = analogRead(tempSensorPin);

    float voltage = (tempSensorVal / 1024.0) * 5.0;

    float temperature = (voltage - .5) * 100;

    // Luminocity calculation

    lightSensorVal = analogRead(lightSensorPin);

    sun = map(lightSensorVal, 0, 1023, 0, 99);

    // Window opening calculation

    potVal = analogRead(potPin);

    angle = map(potVal, 0, 1023, 0, 90);

    myServo.write(angle);

    if (temperature < lowerTemp)
    {

        digitalWrite(greenLed, HIGH);

        digitalWrite(yellowLed, LOW);

        digitalWrite(redLed, LOW);

        sp = 0;

        fan = 0;
    }

    if (temperature > lowerTemp & temperature < upperTemp)
    {

        digitalWrite(greenLed, HIGH);

        digitalWrite(yellowLed, HIGH);

        digitalWrite(redLed, LOW);

        gain = temperature + 0.01 * lightSensorVal + 0.1 * angle;

        sp = map(gain, 20, 50, 100, 255);

        fan = map(sp, 100, 255, 0, 99);
    }

    if (temperature > upperTemp)
    {

        digitalWrite(greenLed, HIGH);

        digitalWrite(yellowLed, HIGH);

        digitalWrite(redLed, HIGH);

        sp = 255;

        fan = 99;
    }

    Serial.print("{'Temp': ");

    Serial.print(temperature);

    Serial.print(", 'Sun': ");

    Serial.print(sun);

    Serial.print(", 'Ang': ");

    Serial.print(angle);

    Serial.print(", 'Fan': ");

    Serial.print(fan);

    Serial.println(" },");

    analogWrite(motorPin, sp);

    // LCD Display

    lcd.setCursor(0, 1);

    lcd.print(temperature, 1);

    lcd.setCursor(5, 1);

    lcd.print(sun);

    lcd.setCursor(7, 1);

    lcd.print("%");

    lcd.setCursor(9, 1);

    lcd.print(angle);

    lcd.setCursor(11, 1);

    lcd.print("o");

    lcd.setCursor(13, 1);

    lcd.print(fan);

    lcd.setCursor(15, 1);

    lcd.print("%");

    delay(100);
}
