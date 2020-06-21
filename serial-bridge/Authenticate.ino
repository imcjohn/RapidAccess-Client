#include <Crypto.h>
#include <Ed25519.h>
#include <RNG.h>
#include <string.h>

const uint8_t PRIVATE_KEY[32] = {157, 69, 177, 157, 239, 253, 90, 96, 186, 132, 74, 244, 146, 236, 44, 196, 68, 73, 197, 105, 123, 50, 105, 25, 112, 59, 172, 3, 28, 174, 127, 96};
const uint8_t PUBLIC_KEY[32] = {201, 131, 169, 176, 3, 49, 193, 225, 136, 10, 42, 218, 79, 190, 13, 232, 121, 226, 116, 42, 177, 150, 33, 147, 38, 179, 164, 200, 171, 16, 81, 100};
uint8_t signature[64];
char message[16];
const uint8_t messageLen = 16;

/**
   These methods execute an authentication handshake as follows
   Client - Send 'S', repeats until getting 'G'
   ESP - Sends back 'G', waits for string
   Client - Sends 16-byte string to be signed
   ESP - Sends string back as JSON, with brackets on each side
   Client - Sends 'C' if string is correct, otherwise anything else
   This will repeat until it works or something crashes etc
*/

void clearSerial() {
  while (Serial.available()) {
    Serial.read();
  }
}

Status handleAuthentication() {
  // Handle initial authentication with the computer, based on private key
  // Return true once authenticated
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch != 'S') {
      Status ret = Error;
      return ret; // expect s as start sequence
    }
    Serial.print("G");
    // Start sequence recieved, pull string to sign
    for (int i = 0; i < messageLen; i++) {
      while (!Serial.available()) {
        ;
      }
      message[i] = Serial.read();
    }
    Ed25519::sign(signature, PRIVATE_KEY, PUBLIC_KEY, (void*) &message, messageLen);
    Serial.print("[");
    for (int i = 0; i < 64; i++) {
      Serial.print(signature[i]);
      if (i != 63) Serial.print(",");
    }
    Serial.print("]");
    while (!Serial.available()) {
      ;
    }
    ch = Serial.read();
    if (ch == 'C') {
      Status ret = Authenticated;
      return ret; // successful auth
    }
    else if (ch == 'R'){
      Status ret = Reset;
      return ret; // successful, but time to restore to factory
    }
    else {
      clearSerial();
      Status ret = Incorrect;
      return ret; // something went wrong, clear input and try again
    }
  }
  else {
    Status ret = Disconnected;
    return ret;
  }
}
