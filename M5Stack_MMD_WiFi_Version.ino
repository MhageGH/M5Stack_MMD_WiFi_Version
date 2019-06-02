#include <M5Stack.h>
#include <esp_sleep.h>
#include <vector>
#include "TCPReceiver.h"

TCPReceiver* tcpReceiver;

void setup() {
  M5.begin();
  tcpReceiver = new TCPReceiver();
  tcpReceiver->setup();
}

void loop() { tcpReceiver->loop(); }
