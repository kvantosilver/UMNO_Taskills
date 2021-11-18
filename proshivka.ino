#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Пины подключаемых устройств
#define PIN_TEMP1 2
#define PIN_TEMP2 3
#define PIN_TEMP3 4
#define PIN_TR1 10
#define PIN_TR2 11
#define PIN_RX 9
#define PIN_TX 8
#define PIN_SIGNAL1 A1
#define PIN_SIGNAL2 A2
#define PIN_SIGNAL3 A3
// переменные 
float mint = 20;
float maxt = 50;
float delta = 0.2; 
float t_now = 0;
float t_hot_now = 0;
float t_cold_now = 0;
float t_in = 0;
int status = 0;
boolean t_on = false;
boolean status_con = false;
String input_message;
String output_message;
uint32_t myTimer;
// объекты температурных датчиков
OneWire oneWire(2);
DallasTemperature ds1(&oneWire);
OneWire oneWire1(3);
DallasTemperature ds2(&oneWire1);
OneWire oneWire2(4);
DallasTemperature ds3(&oneWire2);
SoftwareSerial mySerial(PIN_RX, PIN_TX); 
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  ds1.begin();   
  ds2.begin();   
  ds3.begin();   
  pinMode(PIN_TR1, OUTPUT); // объявляем пин 6 как выход
  pinMode(PIN_TR2, OUTPUT); // объявляем пин 8 как выход
  pinMode(PIN_SIGNAL1, OUTPUT); // объявляем пин 6 как выход
  pinMode(PIN_SIGNAL2, OUTPUT); // объявляем пин 8 как выход
  pinMode(PIN_SIGNAL3, OUTPUT); // объявляем пин 7 как выход
  temp();      
  // инициализация
}
void temp()
{ 
  ds1.requestTemperatures();                               
  t_now = ds1.getTempCByIndex(0);
  Serial.print("Temp now:");  
  Serial.println(t_now);
  ds2.requestTemperatures();                               
  t_hot_now = ds2.getTempCByIndex(0);
  Serial.print("Temp hot now:");  
  Serial.println(t_hot_now);
  ds3.requestTemperatures();                               
  t_cold_now = ds3.getTempCByIndex(0);
  Serial.print("Temp cold now:");  
  Serial.println(t_cold_now);
  output_message = output_message + String(t_now, 2) + ":" + String(t_hot_now, 2) + ":" + String(t_cold_now, 2) + ":" + String(status) + ":" + String(t_on) + "#";
}
void loop() {
  digitalWrite(PIN_SIGNAL1, status_con);
  if (mySerial.available() && !status_con)
  {
    input_message = mySerial.readStringUntil('#');
    if (input_message == "h")
    {
      status_con = true;
    }
  }
  if (millis() - myTimer >= 500 && status_con) {   // ищем разницу (500 мс)
    myTimer = millis();              // сброс таймера
    if (mySerial.available())
    {
      input_message = mySerial.readStringUntil('#');
      Serial.println(input_message);    
      if (input_message[0] == 'y')
      {
        t_in = ((String(input_message[2]) + String(input_message[3]) + String(input_message[4]) + String(input_message[5]))).toFloat();
        Serial.println(t_in);            
        t_on = true;
      }
    }
      output_message = "y:";
      temp();      
      Serial.println(output_message);
      mySerial.println(output_message);      
}
if (t_on)
{  
  if (t_now > t_in + delta){
    digitalWrite(PIN_TR1, HIGH);
    digitalWrite(PIN_TR2, LOW);
    digitalWrite(PIN_SIGNAL2, HIGH);
  digitalWrite(PIN_SIGNAL1, LOW);
}
  else if (t_now < t_in - delta){
digitalWrite(PIN_TR1, LOW);
    digitalWrite(PIN_TR2, HIGH);
    digitalWrite(PIN_SIGNAL2, LOW);
  digitalWrite(PIN_SIGNAL3, HIGH);
  }
  else
  {
    t_on = !t_on;
    digitalWrite(PIN_SIGNAL2, LOW);
  digitalWrite(PIN_SIGNAL3, LOW);
    }
}
}
