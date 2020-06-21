WiFiClient serverClient;
boolean clientConnected = false;

void handleSerial(){
  // Process bytes going to and from the TCP Client, via the serial stream
  
  if (server.hasClient()){ // handle new client add
    serverClient = server.available(); // only one client at a time
    clientConnected = true;
  }
  if (clientConnected){
    if (serverClient.connected()){
      while (serverClient.available() && Serial.availableForWrite() > 0){ // write to serial from client
        Serial.write(serverClient.read());
      }
      // write to client from serial
      size_t tcp_buffer = serverClient.availableForWrite();
      size_t len = std::min((size_t)Serial.available(), tcp_buffer);
      len = std::min(len, (size_t)512); // don't want overrun
      if (len) {
        uint8_t sbuf[len];
        size_t serial_got = Serial.readBytes(sbuf, len);
        // push UART data to all connected telnet clients
        if (serverClient.availableForWrite() >= serial_got) {
          size_t tcp_sent = serverClient.write(sbuf, serial_got);
        }
      }
    }
    else{
      clientConnected = false;
      Serial.write("END_SEQUENCE");
    }
  }
}
