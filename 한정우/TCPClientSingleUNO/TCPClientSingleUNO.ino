/**
 * @example TCPClientSingleUNO.ino
 * @brief The TCPClientSingleUNO demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.03
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "B631"//공유기 아이디
#define PASSWORD    "park0429"//공유기 비번
#define HOST_NAME   "192.168.0.7"//0.7,3.1
#define HOST_PORT   (23)

SoftwareSerial mySerial(10, 9); /* RX:D10, TX:D9 */
ESP8266 wifi(mySerial);


void printUsage()
{
    uint8_t buf[]="Usage\n1 : Turn On LED\n2 : Turn Off LED\nS : LED status\n\n";
    wifi.send(buf, strlen(buf));
}

bool isConnected = false;

void setup(void)
{
    Serial.begin(9600);
    Serial.print("setup begin\r\n");
    
    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());
      
    if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP:");
        Serial.println( wifi.getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }
    
    Serial.print("setup end\r\n");



    pinMode(LED_BUILTIN, OUTPUT);

}



void loop(void)
{
    if ( isConnected == false){
      
        while(1){
            if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
                Serial.print("create tcp ok\r\n");
                isConnected = true;
                printUsage();
                break;
            } else {
                Serial.print("create tcp err\r\n");
            }
        }
    }
   
    uint8_t buffer[128] = {0};
    
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");


        char command = buffer[0];
        int ledStatus = digitalRead(LED_BUILTIN);
        

        switch (command){
        
            case '1':
              
              if (ledStatus == LOW){
                digitalWrite(LED_BUILTIN, HIGH);
                sprintf(buffer, "LED is On!!!\n");
                wifi.send(buffer, strlen(buffer));
              }
              else{
                sprintf(buffer, "LED is already on!!!!\n");
                wifi.send(buffer, strlen(buffer));
              }
              break;
        
            case '2':
            
              if (ledStatus == HIGH){
                digitalWrite(LED_BUILTIN, LOW);
                sprintf(buffer, "LED is off.\n");
                wifi.send(buffer, strlen(buffer));
              }
              else{
                sprintf(buffer, "LED is already off.\n");
                wifi.send(buffer, strlen(buffer));
              }
              break;
        
            case 'S':
            case 's':
                
              if (ledStatus == LOW){
                sprintf(buffer, "LED status: off\n");
                wifi.send(buffer, strlen(buffer));
              }
              else {
                sprintf(buffer, "LED status: On\n");
                wifi.send(buffer, strlen(buffer));
              }
              break;
              
            default:

              break;
              
        }      
    }
}
