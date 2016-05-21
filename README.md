# ArduinoSerialProtocols
Arduino serials signals handler

# Protocols
LEDS
  Prototype : [LED DOOR STATUS]
  Example : LED 1 0
  Params :
    String "LED"
    Int DOOR : 1, 2, 3
    Int Status : 1, 2, 3, ...
  
BUZZER
  Prototype : [BUZZER]
  Example : BUZZER
  Params :
    String "BUZZER"


MESSAGE
  Prototype : [MESSAGE STR]
  Example : MESSAGE here comes the message
  Params :
    String "MESSAGE"
    String STR : String of the message [MAX 16 Digits]
