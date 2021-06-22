//https://www.youtube.com/c/VTMVlogVoThongMinh
#include <Wire.h>
#include <Sim800l.h>                      //https://github.com/vittorioexp/Sim800L-Arduino-Library-revised.git
#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);           
#include <LiquidCrystal_I2C.h>           //https://drive.google.com/file/d/1lpJyuODmZXfi7wwBM2B79BqBIBmBfanS/view?usp=sharing
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ldr 2
#define role1 3
#define role2 4
#define role3 5
#define role4 6
#define ss1 12
#define ss2 9
#define wire 7
int _timeout;
String _buffer;
Sim800l Sim800l;
String textSms, text;
uint8_t index1;
byte Enable = 1;
String number = "+84*********"; //số điện thoại
//............................................................................
void setup() {
  _buffer.reserve(50);
  delay(7000);
  Serial.begin(9600);
  sim.begin(9600);
  Sim800l.begin();
  Serial.println("Delete SMS");
  Sim800l.delAllSms();
  Serial.println("<<Connected>>");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RELAY:      0000");
  lcd.setCursor(0, 1);
  lcd.print("Enable ");
  pinMode(ldr, INPUT);
  pinMode(ss1, INPUT_PULLUP);
  pinMode(ss2, INPUT_PULLUP);
  pinMode(wire, INPUT_PULLUP);
  pinMode(role1, OUTPUT);
  pinMode(role2, OUTPUT);
  pinMode(role3, OUTPUT);
  pinMode(role4, OUTPUT);
  digitalWrite(role1, HIGH);
  digitalWrite(role2, HIGH);
  digitalWrite(role3, HIGH);
  digitalWrite(role4, HIGH);
}
//............................................................................
void loop()
{
  ReadSMS();
  ReadSensor();
}
//............................................................................
void ReadSensor()
{
  if ((Enable == 1) && (digitalRead(ldr) == 1)) {
    if ((digitalRead(ss1) == 1) || (digitalRead(ss2) == 1)) {
      Serial.println("Phát hiện trộm");
      text = String("Phat hien trom");
      lcd.setCursor(11, 1);
      lcd.print("THIEF");
      SendMessage();
      delay(7000);
      callNumberr();
      while ((digitalRead(ss1) == 1) || (digitalRead(ss2) == 1));
      lcd.setCursor(11, 1);
      lcd.print("     ");
    }
  }
  Serial.println("Step4");
  if (digitalRead(wire) == 1) {
    Serial.println("Dây đã bị cắt");
    lcd.setCursor(11, 1);
    lcd.print(" WIRE");
    callNumberr();
    while (digitalRead(wire) == 1);
    lcd.setCursor(11, 1);
    lcd.print("     ");
  }
  Serial.println("Step5");
}
//............................................................................
void ReadSMS()
{
  Serial.println("Step1");
  textSms = Sim800l.readSms(1);
  Serial.println("Step2");
  if (textSms.length() > 8)
  {
    if (textSms.indexOf("ON 1") != -1)
    {
      digitalWrite(role1, LOW);
      Serial.println("Role1 ON");
      lcd.setCursor(12, 0);
      lcd.print('1');
    }
    else if (textSms.indexOf("OFF 1") != -1)
    {
      digitalWrite(role1, HIGH);
      Serial.println("Role1 OFF");
      lcd.setCursor(12, 0);
      lcd.print('0');
    }
    else if (textSms.indexOf("ON 2") != -1)
    {
      digitalWrite(role2, LOW);
      Serial.println("Role2 ON");
      lcd.setCursor(13, 0);
      lcd.print('1');
    }
    else if (textSms.indexOf("OFF 2") != -1)
    {
      digitalWrite(role2, HIGH);
      Serial.println("Role2 OFF");
      lcd.setCursor(13, 0);
      lcd.print('0');
    }
    else if (textSms.indexOf("ON 3") != -1)
    {
      digitalWrite(role3, LOW);
      Serial.println("Role3 ON");
      lcd.setCursor(14, 0);
      lcd.print('1');
    }
    else if (textSms.indexOf("OFF 3") != -1)
    {
      digitalWrite(role3, HIGH);
      Serial.println("Role3 OFF");
      lcd.setCursor(14, 0);
      lcd.print('0');
    }
    else if (textSms.indexOf("ON 4") != -1)
    {
      digitalWrite(role4, LOW);
      Serial.println("Role4 ON");
      lcd.setCursor(15, 0);
      lcd.print('1');
    }
    else if (textSms.indexOf("OFF 4") != -1)
    {
      digitalWrite(role4, HIGH);
      Serial.println("Role4 OFF");
      lcd.setCursor(15, 0);
      lcd.print('0');
    }
    else if (textSms.indexOf("ENABLE") != -1)
    {
      Enable = 1;
      Serial.println("ENABLE sensor");
      lcd.setCursor(0, 1);
      lcd.print("Enable ");
    }
    else if (textSms.indexOf("DISABLE") != -1)
    {
      Enable = 0;
      Serial.println("DISABLE sensor");
      lcd.setCursor(0, 1);
      lcd.print("Disable");
    }
    Serial.println("ok SMS");
    Sim800l.delAllSms();
  }
  Serial.println("step3");
}
//............................................................................
void SendMessage()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = text;
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(5000);
}
//............................................................................
void callNumberr() {
  sim.println("AT");
  updateSerial();
  sim.println("ATD+ " + number + ';');
  updateSerial();
  Serial.println("calling");
  delay(20000); // wait for 20 seconds...
  sim.println("ATH");
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    sim.write(Serial.read());
  }
  while (sim.available())
  {
    Serial.write(sim.read());
  }
}
