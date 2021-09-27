# alerte-choc
A simple program to work with knock sensor.

The idea behind this project is to make an SOS equipment. In the case of an road accident, the module has to inform an trusted person when he hasn't be able to stop the distress signal.

The protoype is composed of :
  *Arduino (whatever you choose)
  *Jumpers
  *Sim808 module
  *Push button
  *Buzzer
  *Red and Green led

The connections are made like follow:
  Equipments                            Arduino
  ******Sim808(Tx-Rx)    -------------    (5-4)
  ******Red led       -----------------      12 
  ******Green led       -----------------      8
  ******Push button       -----------------      7
  

The logic flow of the code follows this guide:




![alerte-choc drawio](https://user-images.githubusercontent.com/71699176/134894837-36dcc533-1e90-4e8e-95df-2bc93e471ead.png)
