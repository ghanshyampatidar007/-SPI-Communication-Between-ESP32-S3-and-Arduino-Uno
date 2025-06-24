#include <SPI.h>

volatile bool dataReceived = false;
volatile byte receivedByte = 0;
volatile byte sendByte = 0x41; // Byte to send back (example: 0x55)

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);        // MISO must be output in slave mode
  SPCR |= _BV(SPE);             // Enable SPI
  SPI.attachInterrupt();        // Enable SPI interrupt
}

ISR(SPI_STC_vect) {
  receivedByte = SPDR;          // Read the byte sent by the master
  SPDR = sendByte;              // Load the next byte to send (immediate response)
  dataReceived = true;
}

void loop() {
  if (dataReceived) {
    Serial.print("Received byte: ");
    //Serial.print(receivedByte, HEX);
    Serial.print((char)receivedByte);
    Serial.print(" | Sent byte: ");
    //Serial.println(sendByte, HEX);
    Serial.println((char)sendByte);

    // Optionally change sendByte for next transfer (echo or counter)
    // sendByte = receivedByte; // Echo back
    // sendByte++;             // Increment on each transfer

    dataReceived = false;
  }
}

