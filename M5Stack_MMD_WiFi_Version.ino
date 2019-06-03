#include <M5Stack.h>
#include "TCPReceiverWithIMU.h"

TCPReceiverWithIMU* tcpReceiverWithIMU;

void setup() {
  M5.begin();
  tcpReceiverWithIMU = new TCPReceiverWithIMU();
  tcpReceiverWithIMU->setup();
}

void loop() { tcpReceiverWithIMU->loop(); }
