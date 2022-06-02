#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//RF24 Object
RF24 radio(3,4);//CE(Chip Enable), CSN( Chip Select Not)

//Communication address:
const uint8_t address[6] = {'0','0','0','0','1'}; //Address of the pipe through which the two radio transmitters communicate



void setup() {
  printf_begin();
  
  Serial.begin(9600);
  
  radio.begin(); //Initializing 'radio' object that is the nRF24L01
  
  radio.setChannel(76);

  bool chipConnected = radio.isChipConnected();
  if (chipConnected) {
    Serial.println("Chip is connected to SPI");
  }

  
  //char channel[] = radio.getChannel();
  Serial.print("This transmitter is connected to channel :");
  Serial.println(radio.getChannel());

  radio.setPALevel(RF24_PA_MIN, 1);
  



  //Opening a pipe and setting the adress where the transmitter will write to.
  radio.openWritingPipe(address);
  //radio.setChannel(channel); // Setting the channel to 2400 + channel Mhz.
  //This method stops listening for incoming messages and switches the module to transmit mode.
  radio.stopListening(); 

  radio.printPrettyDetails();
  
}


void loop(){
  const char text[] = "Dawson College";


  /* This method needs as arguments
   *  1. A pointer to the data to be sent
   *  2. The number of bytes that have to be sent
   */
  bool RF24_ACK;
  RF24_ACK = radio.write(&text, sizeof(text));

  if (RF24_ACK) {
    
    Serial.println("Acknowledgement received");
  }
  else {
    bool tx_ok, tx_fail, rx_ready; // Debug variables to find out what happened to the module
    radio.whatHappened(tx_ok, tx_fail, rx_ready);
    
    Serial.print("tx_ok :");
    Serial.println(tx_ok);
    Serial.print("tx_fail :");
    Serial.println(tx_fail);
    Serial.print("rx_ready :");
    Serial.println(rx_ready);
    Serial.println("Not transmitting!");
   }
   
  if (radio.failureDetected) {
    Serial.println("Failure Detected");
    radio.failureDetected = 0; //Resetting failure count
  }
   delay(1000);
}
