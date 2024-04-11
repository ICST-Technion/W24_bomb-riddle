/.# IOT-bomb-riddle

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
  
