# NODE-RED installation
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

# NODE-RED libraries
npm install node-red-contrib-mqtt-broker
npm install node-red-dashboard

# MOSQUITTO installation
sudo apt install -y mosquitto mosquitto-clients

#edit MOSQUITTO configuration
sudo nano /etc/mosquitto/mosquitto.conf
    # and add the lines: 
    #     listener 1883
    #     allow_anonymous true

# auto-start node-red and nosquitto
sudo systemsctl enable nodered.service
sudo chmod 777 /etc/mosquitto/mosquitto.conf
sudo systemsctl enable mosquitto.service