#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//RF24 Object
RF24 radio(3,4);//CE(Chip Enable), CSN( Chip Select Not)

//Communication address:
const uint8_t address[6] = {'0','0','0','0','1'};




void setup() {
  printf_begin();
  
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin(); //Initializing 'radio' object that is the nRF24L01

    bool chipConnected = radio.isChipConnected();
  if (chipConnected) {
    Serial.println("Chip is connected to SPI");
  }


  
  Serial.print("This transmitter is connected to channel :");
  Serial.println(radio.getChannel());

  
  radio.setAutoAck(true);
  //radio.stopListening(); // Putting it into transmitter mode temporarily.
  
  radio.closeReadingPipe(0);
  radio.closeReadingPipe(1);
  
  radio.openReadingPipe(1, address); //Takes in as arguments the number of the pipe (0-5) and the address.


  radio.startListening(); // Puts the radio module in receiver mode.

  //radio.printPrettyDetails(); //Debugging information
}


void loop(){

  if (radio.available()) { //Checks whether there is information available to be read.

    char text[32] = {0};
    radio.read(&text, sizeof(text));
    //Serial.println("Received Message");
    Serial.println(text);
    //radio.printPrettyDetails();
  }

  else {
    //Serial.println("NOT RECEIVING!");
    delay(1000);
  }
  

}
