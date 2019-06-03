#pragma once
#include <M5Stack.h>
#include <M5TreeView.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <driver/spi_master.h>
#include <esp_heap_alloc_caps.h>
#include "DMADrawer.h"
#include "Madgwick.h"
#include "tjpgdClass.h"
#include "utility/MPU9250.h"

class TCPReceiverWithIMU {
  MPU9250 IMU;
  static Madgwick *madgwick;
  static const uint8_t Clip8[];
  enum {
    DMA_BUF_LEN = 10240,  // 320x32 pixel
    TCP_BUF_LEN = 512
  };
  WiFiServer _tcp;
  WiFiClient _client;
  DMADrawer _dma;
  TJpgD _jdec;
  uint16_t _jpg_x;
  uint16_t _jpg_y;
  uint16_t _recv_remain = 0;
  bool _recv_requested = false;
  uint8_t _jpg_magnify;
  uint8_t _tcpBuf[TCP_BUF_LEN];
  uint32_t _sec = 0;
  uint8_t _drawCount = 0;
  uint16_t _delayCount = 0;
  bool _softap = false;
  void tcpStart(void);
  static uint16_t jpgRead(TJpgD *jdec, uint8_t *buf, uint16_t len);
  static uint16_t jpgWrite(TJpgD *jdec, void *bitmap, JRECT *rect);
  static uint16_t jpgLine(TJpgD *jdec, uint16_t y, uint8_t h);
  uint32_t _ms = 0;
  bool drawJpg();

 public:
  TCPReceiverWithIMU();
  bool setup();
  void close();
  bool loop();
};
