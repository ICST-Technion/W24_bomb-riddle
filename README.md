## "Deuse The Bomb" puzzle Project by : Daniel Belenky.
In this proect i have built an IoT-based game. Using an ESP32 microcontroller as its core, the game presents players with a puzzle involving a set of wires that must be unplugged in the correct sequence within a ten-minute time frame. As players manipulate the wires, a buzzer sounds a sequence corresponding to the number of wires unplugged in the correct order, helping the players to solve the puzzle (similar to the famous mastermind board game). Node-RED dashboard offers real-time monitoring and control capabilities. Monitoring the game time, and each wires state (cut/ un-cut) and an MQTT connection status indicating whether the esp32 is connected through MQTT. It also offers control over the game including the ability to adjust the timer, restart the game, and even automatically solve the puzzle if needed.

## Our Project in details :
1) Hardware Components:

ESP32 Microcontroller: This serves as the brain of the operation, controlling the game logic and interacting with other hardware components.
Timer: A countdown timer implemented using the ESP32 to limit the player's time to solve the puzzle.
RGB LED: Used to indicate the status of each wire (plugged or unplugged) and provide visual feedback during gameplay.
Buzzer: Provides auditory feedback to the player, indicating whether they correctly unplugged the wires in the right order.
Wires: Physical wires connected to the ESP32, representing the elements of the puzzle that the player needs to manipulate.

2) Gameplay:

Players are presented with a set of wires that need to be unplugged in the correct order.
They have 5 minutes to experiment with different combinations of unplugging and replugging the wires.
The RGB LED indicates the status of each wire, helping players keep track of their progress.
When the player successfully unplugs all wires in the correct order, the buzzer sounds a sequence corresponding to the number of wires unplugged.
3) Node-RED Dashboard:

Provides a user-friendly interface for the game master to monitor and control the game.
Displays the countdown timer, allowing the game master to track the remaining time.
Shows the status of each wire (plugged or unplugged) in real-time, helping the game master oversee the progress of the players.
Features controls to start the game over, add time to the timer, and automatically solve the puzzle if needed, enhancing the flexibility and customization of the game experience.
4) Features:

Engaging Gameplay: Offers a challenging and interactive puzzle-solving experience that encourages players to strategize and think critically.
Real-time Monitoring: Enables the game master to monitor the game's progress and intervene if necessary using the Node-RED dashboard.
Customizable Options: Allows the game master to adjust the game settings, such as adding extra time or automatically solving the puzzle, to accommodate different play styles and preferences.
Educational Value: Provides an opportunity for players to develop problem-solving skills and logical thinking while having fun with the game.

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
  
