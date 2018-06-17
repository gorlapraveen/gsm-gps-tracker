#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial gprsSerial(4, 5);//Tx = Pin 4, Rx = Pin 5 . 
SoftwareSerial Serial_Connection(2, 3); //Tx = Pin 2, Rx = Pin 3 .
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data


int currentStatus=0,PreviousStatus,i=0,a=0,b=0,c=0,d=0,e=0,f=1;
int unit_id = 1;
long currentDay,currentTime;
float currentSpeed;
char inData[100];
char inData1[20];
char inchar;
byte index=0;
byte index1=0;
int j=0;
int num;
String currentIMEI="868004025881163";

const int StartPin=7,LoadPin=8,UnLoadPin=9,StopPin=10,IdlePin=11;BreakDownPin=12;
const int LedPin0=A0,LedPin1=A1,LedPin2=A2,LedPin3=A3,LedPin4=A4,LedPin5=A5;
boolean ButtonState1 = 0,ButtonState2 = 0,ButtonState3 = 0,ButtonState4 = 0,ButtonState5 = 0,ButtonState0=0;

float Lat,Lon,flat, flon;
String addDay;

void setup()
{  

 
//PushButtonMode();
 pinMode(LedPin0,OUTPUT);
 pinMode(LedPin1,OUTPUT);
 pinMode(LedPin2,OUTPUT);
 pinMode(LedPin3,OUTPUT);
 pinMode(LedPin4,OUTPUT);
 pinMode(LedPin5,OUTPUT);
 analogWrite(LedPin0,255);
 analogWrite(LedPin1,255);
 analogWrite(LedPin2,255);
  analogWrite(LedPin3,255);
  analogWrite(LedPin4,255);
 analogWrite(LedPin5,255);
 delay(2000);
 analogWrite(LedPin0,0);
 analogWrite(LedPin1,0);
 analogWrite(LedPin2,0);
 analogWrite(LedPin3,0);
 analogWrite(LedPin4,0);
 analogWrite(LedPin5,0);
PushButtonMode();

gprsSerial.begin(9600);
  Serial.begin(9600);
  Serial_Connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started
  Serial.println("Config SIM900...");
  delay(2000);
  Serial.println("Done!...");
  gprsSerial.flush();
  Serial.flush();

 Gsm_Gprs_Setup(); // attach or detach from GPRS service 
    
}

void loop()
{
  PushButton();
  Gps();
Serial.println("Values");
Serial.println(Lat,6);

  //if(a==1)
  //{
  Gsm_Server_Loop();
  //}
  
}


void Gsm_Gprs_Setup()
{

  gprsSerial.listen();
    gprsSerial.println("AT+CGATT?");
  delay(100);
  toSerial();

  gprsSerial.println("AT+CIFSR?");
  delay(100);
  toSerial();
  


  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\" ");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"http:/TATA.DOCOMO.INTERNET\" ");
  delay(200);
  toSerial();
  gprsSerial.println("AT+SAPBR=2,1");
  delay(1000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=1,1");
  delay(1000);
  toSerial();

}
void Gsm_Server_Loop()
{
    gprsSerial.listen();
    
    Serial.println("unique IMEI code");
    
    
    Serial.println(currentIMEI);
    Serial.println(Lat);
    Serial.println(Lon);
    Serial.println(currentDay);
//    Serial.println(current)
    toSerial();
  
  // attach or detach from GPRS service 
  gprsSerial.println("AT+CGATT?");
  delay(100);
  toSerial();

  gprsSerial.println("AT+CIFSR?");
  delay(100);
  toSerial();
  


  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\" ");
  delay(1000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"http:/TATA.DOCOMO.INTERNET\" ");
  delay(1000);
  toSerial();
  gprsSerial.println("AT+SAPBR=2,1");
  delay(1000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=1,1");
  delay(1000);
  toSerial();
  
  
   gprsSerial.println("AT+HTTPINIT");
   delay(500); 
   toSerial();
// set http param value
   gprsSerial.println("AT+HTTPPARA=\"CID\",1");
   delay(500);
   toSerial();
   //gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://www.tarunpulluri/gps/adddata.php?unit=1&addDay=");//Dynamic
   //gprsSerial.print("addDay"),gprsSerial.print(currentDay),gprsSerial.print("\" ");
   gprsSerial.print( F("AT+HTTPPARA=\"URL\",\"http://tarunpulluri.com/gps/adddata1.php?Pin=") );
  //delay(100);
          //toSerial();
          gprsSerial.print(currentIMEI);

  gprsSerial.print(F("&Date="));
   
  gprsSerial.print( currentDay );

  //gprsSerial.print( F("-") );
  
    //gprsSerial.print( currentMonth );

    //gprsSerial.print( F("-") );
  
    //gprsSerial.print( currentYear );

          gprsSerial.print( F("&Time=") );
          gprsSerial.print( currentTime );
        //gprsSerial.print( F(":") );
        //gprsSerial.print( F("&addMinute=") );
        //gprsSerial.print( currentMinute );
          
          gprsSerial.print(F("&Lat="));
          //toSerial();
          gprsSerial.print(Lat,6);
          //toSerial();
          gprsSerial.print(F("&Lon="));
          //toSerial();
          gprsSerial.print(Lon,6);
          //toSerial();
          gprsSerial.print(F("&Speed="));
          //toSerial();
          gprsSerial.print( currentSpeed);

          gprsSerial.print(F("&Status="));
          //toSerial();
          gprsSerial.print(currentStatus);
          
          
  gprsSerial.println( '\"' );
  toSerial();
  delay(1000);
       
   gprsSerial.println("AT+HTTPACTION=0");
   PushButton();
   delay(2000);
   PushButton();
   delay(2000);
   PushButton();
   delay(1500);
   PushButton();
   delay(1200);
   PushButton();
   toSerial();
   // read server response
  // gprsSerial.println("AT+HTTPREAD"); 
  // delay(2000);
  // toSerial();

   //gprsSerial.println("");
   gprsSerial.println("AT+HTTPTERM");
   toSerial();
   delay(300);
   gprsSerial.println("AT+SAPBR=0,1");
   delay(500);
   toSerial();

   gprsSerial.println("");
   delay(1000);
   PushButton();
}


void Gps()
{
  for(i=0 ; i<100 ; i++)
  {

    PushButton();
    
  Serial_Connection.listen();
    Serial.println("Latitude:");
    Lat = Serial_Connection.println(gps.location.lat(),6);
    Lat = Serial.println(gps.location.lat(),6);
   delay(100);
   PushButton();
   Lat = gps.location.lat();
   
    Serial.println("Longitude:");
 Lon=Serial_Connection.println(gps.location.lng(),6);
 Lon=Serial.println(gps.location.lng(),6);
 
    Lon = gps.location.lng();
    Serial.println("time HHMMSSCC:");
    currentTime=Serial.println(gps.time.value());
   Serial.println("Date DDMMYEAR:");
    currentDay=Serial.println(gps.date.value());
    Serial.println("Speed KMPH:");
    currentSpeed=Serial.println(gps.speed.kmph(),6);
  while(Serial_Connection.available())//While there are characters to come from the GPS
  {
    gps.encode(Serial_Connection.read());//This feeds the serial NMEA data into the library one char at a time
    toSerial();
  }
  if(i>50)
  {
   PushButton();
  
    Serial.println("time HHMMSSCC:");
    currentTime=Serial.println(gps.time.value());
    currentTime=Serial_Connection.println(gps.time.value());
    currentTime = gps.time.value();
    PushButton();
   Serial.println("Date DDMMYEAR:");
    currentDay=Serial.println(gps.date.value());
    currentDay=Serial_Connection.println(gps.date.value());
    currentDay = gps.date.value();
    PushButton();
    Serial.println("Speed KMPH:");
    currentSpeed=Serial.println(gps.speed.kmph(),6);
    currentSpeed=Serial_Connection.println(gps.speed.kmph(),6);
    currentSpeed = gps.speed.kmph();
    PushButton();
    if(i>75)
  {
   PushButton();
   Serial.println("Date DDMMYEAR:");
    currentDay=Serial.println(gps.date.value());
    currentDay=Serial_Connection.println(gps.date.value());
    currentDay = gps.date.value();
    PushButton();
    Serial.println("Speed KMPH:");
    currentSpeed=Serial.println(gps.speed.kmph(),6);
    currentSpeed=Serial_Connection.println(gps.speed.kmph(),6);
    currentSpeed = gps.speed.kmph();
   
    }
if(i>85)
  {
   PushButton();
    Serial.println("Speed KMPH:");
    currentSpeed=Serial.println(gps.speed.kmph(),6);
    currentSpeed=Serial_Connection.println(gps.speed.kmph(),6);
    currentSpeed = gps.speed.kmph();
    PushButton();
   
  }

    }
  }
  
  
}


void PushButtonMode()
{
    pinMode(LedPin0, OUTPUT);
  pinMode(LedPin1, OUTPUT);
  pinMode(LedPin2, OUTPUT);
  pinMode(LedPin3, OUTPUT);
  pinMode(LedPin4, OUTPUT);
  pinMode(LedPin5, OUTPUT);
  pinMode(StartPin, INPUT_PULLUP); // Connects internal pullup resistor
  
  pinMode(LoadPin, INPUT_PULLUP);
  pinMode(UnLoadPin, INPUT_PULLUP);
  pinMode(StopPin, INPUT_PULLUP);
  pinMode(IdlePin, INPUT_PULLUP); 
  pinMode(BreakDownPin, INPUT_PULLUP);
   
}


void PushButton()
{
  ButtonState0 = !digitalRead(StartPin);
  ButtonState1 = !digitalRead(LoadPin);
  ButtonState2 = !digitalRead(UnLoadPin);
  ButtonState3 = !digitalRead(StopPin);
  ButtonState4 = !digitalRead(IdlePin);
  ButtonState5 = !digitalRead(BreakDownPin); //The inverts the digitalRead() so an active low becomes HIGH
 Serial.print("a,b,c,d,f,status=");
 Serial.println(a);
 Serial.println(b);
 Serial.println(c);
 Serial.println(d);
 Serial.println(f);
 Serial.println(currentStatus);
 Start();
 Load();
 UnLoad();
 Stop();
 Idle();
 BreakDown();
}

void Start()
 {
  
  if (ButtonState0 == HIGH)
  { 
    if(((e==0)&&(a==0))&&((d==0)&&(f==1)))
    {
     a=1;
     b=0;
     c=0;
     d=0;
     f=0;
     currentStatus=1;
    digitalWrite(LedPin0, HIGH);
    digitalWrite(LedPin1, LOW);
    digitalWrite(LedPin2, LOW);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, LOW);
    digitalWrite(LedPin5, LOW);
    delay(100);
    }
  } 
 }
 
 void Load()
 {
   if (ButtonState1 == HIGH) 
   {
     if(((a==1)&&(b==0))&&((e==0)&&(f==0)))
     {
       b=1;
       c=0;
       d=0;
       f=0;
       
       currentStatus=2;
       digitalWrite(LedPin0, LOW);
    digitalWrite(LedPin1, HIGH);
    digitalWrite(LedPin2, LOW);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, LOW);
    digitalWrite(LedPin5, LOW);
    delay(100);
     }
    
  }
 }
 
 void UnLoad()
 {
   if (ButtonState2 == HIGH) 
   {
     if(((b==1)&&(c==0))&&(e==0))
     {
       a=1;
       b=1;
       c=1;
       d=0;
       f=0;
       currentStatus=3;
    digitalWrite(LedPin0, LOW);
    digitalWrite(LedPin1, LOW);
    digitalWrite(LedPin2, HIGH);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, LOW);
    digitalWrite(LedPin5, LOW);
    delay(100);
     }
  }
 }
 
 void Stop()
{
  if (ButtonState3 == HIGH)
  { 
    if(((c==1)&&(d==0))&&(e==0))
   { a=1;
     b=1;
     c=1;
     d=1;
     f=0;
     currentStatus=4;
     digitalWrite(LedPin0, LOW);
    digitalWrite(LedPin1, LOW);
    digitalWrite(LedPin2, LOW);
    digitalWrite(LedPin3, HIGH);
    digitalWrite(LedPin4, LOW);
    digitalWrite(LedPin5, LOW);
    delay(100);
   }
  }
}

void Idle()
 {
  
  if (ButtonState4 == HIGH)
  { 
    if(((e==0)&&(a==1))&&((d==1)&&(f==0)))
    {
     a=0;
     b=0;
     c=0;
     d=0;
     f=1;
     currentStatus=0;
    digitalWrite(LedPin0, LOW);
    digitalWrite(LedPin1, LOW);
    digitalWrite(LedPin2, LOW);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, HIGH);
    digitalWrite(LedPin5, LOW);
    delay(100);
    }
  } 
 }


void BreakDown()
{
  
  if (ButtonState5 == HIGH) 
  { 
    if(e==0)
    {
     PreviousStatus=currentStatus;
     currentStatus=5;
    digitalWrite(LedPin5, HIGH);
    delay(300);
    e=1;
    }
    else
    {
      currentStatus=PreviousStatus;
      digitalWrite(LedPin5,LOW);
      delay(300);
      e=0;
    }
  }
  
}



void toSerial()
{
    while(gprsSerial.available()!=0)

   {
      Serial.write(gprsSerial.read());
    }
 }

void toSerial1()
{
  while(Serial_Connection.available()!=0)
    {
    Serial.write(Serial_Connection.read());
    }
 }





