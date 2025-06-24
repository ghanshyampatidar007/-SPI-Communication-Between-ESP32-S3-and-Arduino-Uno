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

/*
#include <SPI.h>

#define MAX_LEN 64
//volatile 
char rxBuffer[MAX_LEN];      // Buffer to hold incoming string
volatile byte rxIndex = 0;
volatile bool stringReceived = false;

const char response[] = "11111";  // Response string
volatile byte txIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(MISO, OUTPUT);        // Required for SPI slave
  SPCR |= _BV(SPE);             // Enable SPI
  SPI.attachInterrupt();        // Enable SPI interrupt
  SPDR = response[txIndex];     // Load first byte of response
}

ISR(SPI_STC_vect) {
  char received = SPDR;

  // Prepare next byte to send from response string
  txIndex++;
  if (txIndex >= strlen(response)) {
    txIndex = 0;
  }
  SPDR = response[txIndex];

  // Accumulate received characters
  if (rxIndex < MAX_LEN - 1) {
    rxBuffer[rxIndex++] = received;
    if (received == '\n') {
      rxBuffer[rxIndex] = '\0';   // Null-terminate the string
      stringReceived = true;
      rxIndex = 0;                // Reset for next string
    }
  } else {
    // Overflow protection
    rxIndex = 0;
  }
}

void loop() {
  if (stringReceived) {
    Serial.print("sent string: ");
    Serial.println(response);
    Serial.print("Received string: ");
    Serial.println(rxBuffer);
    stringReceived = false;
  }
}
*/