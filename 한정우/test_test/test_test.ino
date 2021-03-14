 
#include <string.h>  
#include <Time.h>
#include "ESP8266.h"
#include <SoftwareSerial.h>  
    

#define SSID "B631"  

#define PASS "park0429"  

#define DST_IP "192.168.0.7"//"ip주소"  

SoftwareSerial dbgSerial(10, 9); /* RX:D10, TX:D9 */ 

void setup()
{
 
  // Setup Serial connection
  Serial.begin(9600);
 
  //ESP8266
  /////////////////////////////////////////////////////////////////////////  
  Serial.setTimeout(5000);  
  dbgSerial.begin(9600);   
  Serial.println("ESP8266 connect");  
  
    
   boolean connected=false;  
   for(int i=0;i<10;i++)  
   {  
       if(connectWiFi())  
       {  
         connected = true;  
         break;  
       }  
   }  
     
   if (!connected){while(1);}  
   delay(5000);  
    
   dbgSerial.println("AT+CIPMUX=0");
}

void loop()
{
//이하는 데이터베이스에 넣는 부분
 String cmd = "AT+CIPSTART=\"TCP\",\"";  
 cmd += DST_IP;
 cmd += "\",80";
 Serial.println(cmd);
 dbgSerial.println(cmd);  
 if(dbgSerial.find("Error"))
 {
  Serial.println( "TCP connect error" );
  return;
  }  
     
    String test;
    String test2;  
    test= "\"hos\"";
    test2="\"11112345678\"";
    cmd = "GET/test.php?name="+test+"&phone="+test2+"HTTP/1.1\r\n\r\n";  
    dbgSerial.print("AT+CIPSEND=");  
    dbgSerial.println(cmd.length());  
          
    
    Serial.println(cmd);
       
     if(dbgSerial.find(">"))  
     {  
       Serial.print(">");  
       }else  
       {  
         dbgSerial.println("AT+CIPCLOSE");  
         Serial.println("connect timeout");  
         delay(1000);  
         return;  
       }  
       Serial.print(cmd);
       dbgSerial.print(cmd);  
       delay(2000);  
       //Serial.find("+IPD");  
       while (Serial.available())  
       {  
         char c = Serial.read();  
         dbgSerial.write(c);  
         if(c=='\r') dbgSerial.print('\n');  
       }  
       Serial.println("===="); 
 
  
  // Wait one second before repeating :)
  delay (2000);
}

boolean connectWiFi()  
{  
   //dbgSerial.println("AT+CWMODE=1");  
     
   String cmd="AT+CWJAP=\"";  
   cmd+=SSID;  
   cmd+="\",\"";  
   cmd+=PASS;  
   cmd+="\"";  
   dbgSerial.println(cmd);  
   Serial.println(cmd);  
   delay(3000);  
    
   if(dbgSerial.find("OK"))  
   {  
     Serial.println("OK, Connected to WiFi.");  
     return true;  
   }  
   else  
   {  
     Serial.println("Can not connect to the WiFi.");  
     return false;  
   }  
 } 
