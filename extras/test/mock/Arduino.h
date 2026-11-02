// Минимальная имитация Arduino.h для запуска тестов на компьютере (g++).
#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using __FlashStringHelper = char;
#define F(x) ((const __FlashStringHelper *)(x))
#define INPUT 0
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3
#define LOW 0
#define HIGH 1
inline uint32_t g_now = 0;
inline int g_analog[64];
inline uint8_t g_level[64];
inline uint32_t millis() { return g_now; }
inline uint8_t g_mode[64];
inline void pinMode(uint8_t p, uint8_t m) { g_mode[p] = m; g_level[p] = (m == INPUT_PULLDOWN) ? 0 : 1; }
inline int digitalRead(uint8_t p) { return g_level[p]; }
inline int analogRead(uint8_t p) { return g_analog[p]; }
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(int v) { printf("%d", v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(int v) { printf("%d\n", v); }
};
