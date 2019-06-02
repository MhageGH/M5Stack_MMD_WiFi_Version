#ifndef _TCPRECEIVER_H_
#define _TCPRECEIVER_H_

#include <M5Stack.h>
#include <M5TreeView.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <driver/spi_master.h>
#include <esp_heap_alloc_caps.h>
#include "ARlikeMotion.h"
#include "DMADrawer.h"
#include "tjpgdClass.h"

#define dmaColor(r, g, b)                                     \
  (uint16_t)(((uint8_t)(r)&0xF8) | ((uint8_t)(b)&0xF8) << 5 | \
             ((uint8_t)(g)&0xE0) >> 5 | ((uint8_t)(g)&0x1C) << 11)

#define BYTECLIP(v) Clip8[(uint16_t)(v)]

static const uint8_t Clip8[] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
    15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
    30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
    45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,
    75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
    90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255,
};

class TCPReceiver {
 public:
  TCPReceiver() {}

  bool setup() {
    M5.Lcd.fillScreen(0);
    M5.Lcd.setTextColor(0xFFFF, 0);
    Serial.println("setup");

    tcpStart();
    if (!_softap) {
      M5.Lcd.drawString(WiFi.localIP().toString(), 0, 0);
    } else {
      M5.Lcd.drawString(WiFi.softAPIP().toString(), 0, 0);
    }

    _dma.setup(DMA_BUF_LEN);
    _jdec.multitask_begin();

    ARlikeMotion::Init();
    return true;
  }

  void close() {
    _client = _tcp.available();
    if (_client.connected()) _client.stop();
    _tcp.stop();
    _dma.close();
    _jdec.multitask_end();
  }

  bool loop() {
    static uint16_t count = 0;

    if (5 <= _client.available() && 5 == _client.read(_tcpBuf, 5)) {
      count = 0;
      if (_tcpBuf[0] == 'J' && _tcpBuf[1] == 'P' && _tcpBuf[2] == 'G') {
        _recv_requested = false;
        _recv_remain = _tcpBuf[3] | (_tcpBuf[4] << 8);
        if (_recv_remain > 600) {
          if (drawJpg()) {
            ++_drawCount;
          }
        } else {
          Serial.println("jpg too short");
        }
        if (_recv_remain) {  // remove remain data
          Serial.println("clear remain data");
          int r;
          for (uint16_t retry = 1000; retry; --retry) {
            r = _client.read(_tcpBuf, _recv_remain < TCP_BUF_LEN ? _recv_remain
                                                                 : TCP_BUF_LEN);
            if (r > 0) {
              _recv_remain -= r;
              if (!_recv_remain) break;
            } else {
              delay(1);
              ++_delayCount;
            }
          }
        }
      } else {
        Serial.println("broken data");
        delay(10);
        while (0 < _client.read(_tcpBuf, TCP_BUF_LEN)) delay(10);
      }
    } else if (!_client.connected()) {
      _client = _tcp.available();
      _recv_requested = false;
      count = 0;
    } else {
      if (++count > 1000) {
        _recv_requested = false;
      } else {
        delay(1);
        ++_delayCount;
      }
      if (!_recv_requested) {
        count = 0;
        Serial.println("data request");
        _client.print("JPG\n");
        _recv_requested = true;
      }
    }

    if (_sec != millis() / 1000) {
      Serial.printf("%2d fps", _drawCount);
      if (_delayCount) Serial.printf(" / delay%3d", _delayCount);
      Serial.print("\r\n");
      _sec = millis() / 1000;
      _drawCount = 0;
      _delayCount = 0;
    }

    static long t = 0;
    int interval = 20000;
    if (t != micros() / interval) {
      t = micros() / interval;
      ARlikeMotion::ReviseIMU(interval / 1000000.0f);
    }
    return true;
  }

 private:
  enum {
    DMA_BUF_LEN = 10240  // 320x32 pixel
    ,
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

  void tcpStart(void) {
    Serial.println("wifi init");
    if (!_softap) {
      WiFi.mode(WIFI_MODE_STA);
      for (int i = 0; i < 10; ++i) {
        WiFi.disconnect(true);
        delay(100);
        WiFi.begin();
        for (int j = 0; j < 100; ++j) {
          if (WiFi.status() != WL_CONNECTED) {
            delay(50);
          }
        }
        if (WiFi.status() == WL_CONNECTED) {
          _tcp.setNoDelay(true);
          _tcp.begin(63333);
          Serial.println("WiFi Connected.");
          break;
        }
      }
    } else {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_MODE_AP);
      WiFi.begin();
      _tcp.setNoDelay(true);
      _tcp.begin(63333);
    }
  }

  static uint16_t jpgRead(TJpgD *jdec, uint8_t *buf, uint16_t len) {
    TCPReceiver *me = (TCPReceiver *)jdec->device;
    WiFiClient *client = &me->_client;
    if (!client->connected()) return 0;
    uint16_t retry;

    int necessary = (len == TJPGD_SZBUF)
                        ? 1
                        : (len < me->_recv_remain ? len : me->_recv_remain);

    if (len == TJPGD_SZBUF && me->_recv_remain < TJPGD_SZBUF * 2 &&
        len < me->_recv_remain) {  // dataend read tweak
      len = me->_recv_remain - len;
    }

    for (retry = 1000; client->available() < necessary && retry; --retry) {
      delay(1);
      ++me->_delayCount;
    }
    // if (retry != 1000) Serial.printf("jpgRead wait %d\r\n", 1000 -
    // retry);

    int l = client->read(buf ? buf : me->_tcpBuf, len);
    if (l <= 0) return 0;
    len = l;
    me->_recv_remain -= len;
    if (!me->_recv_requested && me->_recv_remain < 3) {
      if (me->_recv_remain >= client->available()) {
        float r[3][3];
        ARlikeMotion::GetRotation(r);
        char s[256];
        sprintf(s,
                "JPG\n %2.5f %2.5f %2.5f %2.5f %2.5f %2.5f %2.5f %2.5f "
                "%2.5f\n",
                r[0][0], r[0][1], r[0][2], r[1][0], r[1][1], r[1][2], r[2][0],
                r[2][1], r[2][2]);
        client->print(s);
        // client->print("JPG\n"); // request the next image from the
        // client
        me->_recv_requested = true;
        // Serial.printf("prerequest %d \r\n", len);
      } else {
        Serial.println("excessive request");
        me->_recv_requested = true;
      }
    }
    return len;
  }

  static uint16_t jpgWrite(TJpgD *jdec, void *bitmap, JRECT *rect) {
    TCPReceiver *me = (TCPReceiver *)jdec->device;
    uint16_t width = jdec->width;
    uint16_t x = rect->left;
    uint16_t y = rect->top;
    uint16_t w = rect->right + 1 - x;
    uint16_t h = rect->bottom + 1 - y;
    uint16_t *p = me->_dma.getNextBuffer();
    uint16_t *dst;

    uint8_t *data = (uint8_t *)bitmap;
    uint8_t magnify = me->_jpg_magnify;
    uint8_t line;
    uint8_t ww = w;
    uint8_t hh = h;
    uint8_t yy = 0;

    if (!magnify) {
      while (hh--) {
        line = ww;
        dst = p + x + width * yy;
        while (line--) {
          *dst++ = dmaColor(data[0], data[1], data[2]);
          data += 3;
        }
        ++yy;
      }
    } else {
      uint16_t addy = width << 1;
      uint8_t r, g, b;
      while (hh--) {
        line = ww;
        dst = p + ((x + addy * yy) << 1);
        while (line--) {
          r = *data++;
          g = *data++;
          b = *data++;
          *dst++ = dmaColor(r, g, b);
          *dst-- = dmaColor(BYTECLIP(r + 4), BYTECLIP(g + 2), BYTECLIP(b + 4));
          dst += addy;
          *dst++ = dmaColor(BYTECLIP(r + 6), BYTECLIP(g + 3), BYTECLIP(b + 6));
          *dst++ = dmaColor(BYTECLIP(r + 2), BYTECLIP(g + 1), BYTECLIP(b + 2));
          dst -= addy;
        }
        ++yy;
      }
    }

    return 1;
  }

  static uint16_t jpgLine(TJpgD *jdec, uint16_t y, uint8_t h) {
    TCPReceiver *me = (TCPReceiver *)jdec->device;
    uint8_t magnify = me->_jpg_magnify;
    me->_dma.draw(me->_jpg_x, me->_jpg_y + (y << magnify),
                  jdec->width << magnify, h << magnify);
    return 1;
  }

  uint32_t _ms = 0;

  bool drawJpg() {
    uint32_t ms1 = micros();
    JRESULT jres = _jdec.prepare(jpgRead, this);
    if (jres != JDR_OK) {
      Serial.printf("prepare failed! %d\r\n", jres);
      return false;
    }
    uint32_t ms2 = micros();

    if (_jdec.width > 160 || _jdec.height > 120) {
      _jpg_magnify = 0;
      _jpg_x = 160 - _jdec.width / 2;
      _jpg_y = 120 - _jdec.height / 2;
    } else {
      _jpg_magnify = 1;
      _jpg_x = 160 - _jdec.width;
      _jpg_y = 120 - _jdec.height;
    }
    if (M5.BtnC.isPressed()) {  // DEBUG
      jres = _jdec.decomp(jpgWrite, jpgLine);
    } else {
      jres = _jdec.decomp_multitask(jpgWrite, jpgLine);
    }
    uint32_t ms3 = micros();
    if (jres != JDR_OK) {
      Serial.printf("decomp failed! %d\r\n", jres);
      return false;
    }
    if (M5.BtnC.isPressed()) {
      Serial.printf("micros: %d:%d:%d\r\n", ms2 - ms1, ms3 - ms2, ms1 - _ms);
    }
    _ms = ms3;
    return true;
  }
};

#endif
