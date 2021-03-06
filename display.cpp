/**
 * Copyright (c) 2015 by Matt Carpenter <mattcarpenter@gmail.com>
 * Display class for RSSI Diversity Controller
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
#include "display.h"

Display::Display() {
  u8g = new U8GLIB_SSD1306_128X64 (U8G_I2C_OPT_NO_ACK);
  int _rawValues[3] = {0};
  int _activeChannel = 0;
}
 
Display::~Display() {
  if (NULL != u8g) {
    delete u8g;
    u8g = NULL;
  }
}
 
void Display::updateChannel(uint8_t channel, uint8_t rawValue, boolean active) {
  _rawValues[channel] = rawValue;
  _activeChannel = (active ? channel : _activeChannel);
}

void Display::drawCycle() {
  u8g->firstPage();
  do {
    drawChrome();
    drawText();
    drawIndicators();
  } while (u8g->nextPage());
}

void Display::drawChrome() {
  u8g->setColorIndex(1);
  
  // Draw channel ID boxes
  u8g->drawBox(12, 0, 21, 17);
  u8g->drawBox(12, 18, 21, 17);
  u8g->drawBox(12, 36, 21, 17);
  
  // Draw RSSI bar frames
  u8g->drawFrame(34, 0, 94, 17);
  u8g->drawFrame(34, 18, 94, 17);
  u8g->drawFrame(34, 36, 94, 17);
}

void Display::drawText() {
  String ch1 = String(_rawValues[0]) + "%";
  String ch2 = String(_rawValues[1]) + "%";
  String ch3 = String(_rawValues[2]) + "%";
  char buffer[5];
  
  u8g->setColorIndex(0);
  
  // Channel numbers
  u8g->setFont(u8g_font_6x10);
  u8g->drawStr( 14, 12, "CH1");
  u8g->drawStr( 14, 30, "CH2");
  u8g->drawStr( 14, 48, "CH3");
  
  // RSSI %
  u8g->setFont(u8g_font_9x15);
  u8g->setColorIndex(1);
  
  ch1.toCharArray(buffer,5);
  u8g->drawStr( 36, 14, buffer);
  
  ch2.toCharArray(buffer, 5);
  u8g->drawStr( 36, 32, buffer);
  
  ch3.toCharArray(buffer, 5);
  u8g->drawStr( 36, 50, buffer);
  
  // Footer
  u8g->setFont(u8g_font_04b_03r);
  u8g->drawStr(14, 62, "RSSI Diversity Controller");
}

 
void Display::drawIndicators() {
  int yOffset = (_activeChannel * 19) - (_activeChannel == 2 ? 1 : 0); 
 
  u8g->setColorIndex(1);
  
  // Active channel decorator
  u8g->drawTriangle(2, yOffset, 10, 8 + yOffset, 2, 16 + yOffset);
  
  // Strength bars
  // Max width 53 pixels
  u8g->drawBox(73, 2, (_rawValues[0] * 53 / 100), 13);
  u8g->drawBox(73, 20, (_rawValues[1] * 53 / 100), 13);
  u8g->drawBox(73, 38, (_rawValues[2] * 53 / 100), 13);
}
