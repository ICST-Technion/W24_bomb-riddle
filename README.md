## "Deuse The Bomb" puzzle Project by : Daniel Belenky.
In this proect i have built an IoT-based game. Using an ESP32 microcontroller as its core, the game presents players with a puzzle involving a set of wires that must be unplugged in the correct sequence within a ten-minute time frame. As players manipulate the wires, a buzzer sounds a sequence corresponding to the number of wires unplugged in the correct order, helping the players to solve the puzzle (similar to the famous mastermind board game). Node-RED dashboard offers real-time monitoring and control capabilities. Monitoring the game time, and each wires state (cut/ un-cut) and an MQTT connection status indicating whether the esp32 is connected through MQTT. It also offers control over the game including the ability to adjust the timer, restart the game, and even automatically solve the puzzle if needed.

## The Project in detail :

#### Hardware Components :

* **ESP32 Microcontroller:** This serves as the brain of the operation, controlling the game logic (code) and interacting with other hardware components.

* **Timer:** A countdown timer implemented using the ESP32 to limit the player's time to solve the puzzle.

* **RGB LED:** Used to indicate the state of the game by providing visual feedback.

* **Buzzer:** Provides auditory feedback to the player, indicating whether they correctly unplugged the wires in the right order (or how much were in the right order).

* **Wires:** Physical wires connected to the ESP32, representing the elements of the puzzle that the player needs to manipulate, configured to "pull-up" so their state changes when connected to the ground or unplugged.

#### Gameplay :

Players are presented with a set of wires that need to be unplugged in the correct order.
They have 10 minutes to experiment with different combinations of unplugging and replugging the wires.
The the buzzer beeps for the wires that were unplugged in the correct order helping players solve the puzzle.
The RGB LED indicates the state of the game.
When the player successfully unplugs all wires in the correct order, the led turnes green and the timer stopes.
#### Node-RED Dashboard :

Provides a user-friendly interface for the game master to monitor and control the game.
Displays the countdown timer, allowing the game master to track the remaining time.
Shows the status of each wire (plugged or unplugged) in real-time, helping the game master oversee the progress of the players and shows status indicating ESP's connection to MQTT.
Features controls to start the game over, add time to the timer, and automatically solve the puzzle if needed, enhancing the flexibility and customization of the game experience.
Also shows status of control action, including: "sending to device", "success" and "failed".

## Folder description :
* **ESP32:** source code for the esp side (firmware).
* **Documentation:** wiring diagram + basic operating instructions including raspberry-pi scripts for node-red and MQTT broker.
* **Unit Tests:** tests for individual hardware components (Timer, RGB led, buzzer)
* **NODE RED:** json file of the node-red dashboard- including all features andconnection to MQTT server.
* **Parameters:** contains description of configurable parameters. 
* **Assets:** Audio files used in this project (in the node-red dashboard), 

## Arduino/ESP32 libraries used in this project :
* PubSubClient (by Nick O'Leary) - version 2.8
* LedController (by Noa Sakurajin) - version 2.0.2

## Wire Diagram :

  

