## Pilltrack Project by : Malak Marrid, Mahmoud Massarwi & Zaina Darawsha.
In this project we have built a smart pill box that makes it easy for patients to track their pill doses over a week (7 days), pill doses times, and to get 
notified by a smartphone app when the pill times come and when they are late for pills. In addition, a helper of the patient can use the app to see the patient LOG and get notified when the patient is late for pills.
The smart pill box is responsive, and gives sound and visual notifications to help the patient. 

## Our Project in details :
First, the patient and his helper both signup to the app (each on his phone).
The patient enters the pill times and chooses a "refill day": when the box needs refill after 1 week of use.
1. Settings: Inside the app, the patient can update the pill times: 2 times per day in which the patient will take his pill doses (2 cells in the box per day) .
2. Search Box: If the patient forgot where the pill box is, he can press on "search box" botton in the app the that will cause the pill box to light in different colors 
and make loud continuous sounds to help the patient find the box location.
3. Reminders: 5 minutes before each pill does time, the patient will get a reminder notification to take his pills. and the suitable cell in the pill box will light in blue to help the patient take the right dose.
4. Pills taken in time: If the patient took his pills from the right cell, it will light in purple to confirm him that he's done okay, and he will get a "good job notification". There is a virtual pill box in the app that shows the status of each pill dose through the week and it will be updated in the suitable cell with a v mark.

## Folder description :
* ESP32: source code for the esp side (firmware).
* Documentation: wiring diagram + basic operating instructions
* Unit Tests: tests for individual hardware components (input / output devices)
* flutter_app : dart code for our Flutter app.
* Parameters: contains description of configurable parameters 
* Assets: 3D printed parts, Audio files used in this project, 
* link to app additional material: https://drive.google.com/drive/folders/1LCiqWJl7VVG6eHTiEd6VVtLLuSFPIfDv

## Arduino/ESP32 libraries used in this project:
* ArduinoMqttClient - version 0.1.6
* RTCZero - version 1.6.0
* Adafruit BusIO - version 1.14.1
* Adafruit PCF8574 - version 1.1.0
* FastLED - version 3.5.0
* Firebase Arduino Client Library for ESP8266 and ESP32 - version 4.2.7
  
## Raspberry Pi installetion:
NODE-RED:
  1)     bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
  2) in the node-red install:
      * node-red-contrib-mqtt-broker
      * node-red-dashboard    
  3)     sudo systemsctl enable nodered.service


MOSQUITTO:
  1)     sudo apt install -y mosquitto mosquitto-clients
  2)     sudo nano /etc/mosquitto/mosquitto.conf
     and add the lines:

        * listener 1883

     
       * allow_anonymous true
  4)     sudo chmod 777 /etc/mosquitto/mosquitto.conf
  5)     sudo systemsctl enable mosquitto.service
  
