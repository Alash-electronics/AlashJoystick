// MIT License. Alash Electronics.
#include "AlashJoystick.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

AlashJoystick::AlashJoystick(uint8_t pinX, uint8_t pinY, int8_t pinButton) : _px(pinX), _py(pinY), _pb(pinButton) {}

bool AlashJoystick::begin() {
  if (_pb >= 0) pinMode(_pb, INPUT_PULLUP);
  _started = true;
  _cx = (_adcMin + _adcMax) / 2;           // пока не замерили, центр это середина диапазона
  _cy = _cx;
  if (_pb >= 0) {
    _bRaw = _bDown = (digitalRead(_pb) == LOW);
    _bRawAt = millis();
  }
  return calibrateCenter();
}

int16_t AlashJoystick::sample(uint8_t pin) {
  int32_t sum = 0;
  for (uint8_t i = 0; i < _smooth; i++) sum += analogRead(pin);
  return (int16_t)(sum / _smooth);
}

// Центр у каждого джойстика свой (±5-10 % от середины). Замеряем при старте при отпущенной ручке.
bool AlashJoystick::calibrateCenter() {
  _err = ERR_OK;
  uint8_t keep = _smooth;
  _smooth = 16;                            // для замера центра усредняем сильнее
  int16_t cx = sample(_px), cy = sample(_py);
  _smooth = keep;
  int16_t range = _adcMax - _adcMin;
  int16_t mid = _adcMin + range / 2;
  if (cx <= _adcMin + range / 50 || cx >= _adcMax - range / 50 || cy <= _adcMin + range / 50 || cy >= _adcMax - range / 50) {
    _err = ERR_NO_SIGNAL;                  // ось на упоре: это не центр, а обрыв или питание
  } else if (abs(cx - mid) > range / 4 || abs(cy - mid) > range / 4) {
    _err = ERR_NOT_CENTERED;               // центр слишком далеко от середины: ручку держали
  }
  if (_err == ERR_OK) {
    _cx = cx;
    _cy = cy;
  }
  return _err == ERR_OK;
}

// raw -> -100..100 относительно центра, с мёртвой зоной. Половины шкалы по разные стороны центра растягиваются отдельно.
int8_t AlashJoystick::scale(int16_t raw, int16_t center, bool invert) {
  int32_t d = (int32_t)raw - center;
  int32_t span = d >= 0 ? (int32_t)_adcMax - center : (int32_t)center - _adcMin;
  if (span <= 0) return 0;
  int32_t v = d * 100 / span;
  if (v > 100) v = 100;
  if (v < -100) v = -100;
  int32_t a = v < 0 ? -v : v;
  if (a <= _dead) return 0;
  a = (a - _dead) * 100 / (100 - _dead);   // за мёртвой зоной шкала снова начинается с 0
  v = v < 0 ? -a : a;
  return (int8_t)(invert ? -v : v);
}

int16_t AlashJoystick::rawX() { return sample(_px); }
int16_t AlashJoystick::rawY() { return sample(_py); }
int8_t AlashJoystick::x() {
  if (!_started) begin();
  return scale(sample(_px), _cx, _invX);
}
int8_t AlashJoystick::y() {
  if (!_started) begin();
  return scale(sample(_py), _cy, _invY);   // + при росте значения АЦП; если «вверх» у вас отрицательный, вызовите invertY()
}

uint8_t AlashJoystick::magnitude() {
  int32_t xv = x(), yv = y();
  int32_t m = (int32_t)sqrt((double)(xv * xv + yv * yv));
  return m > 100 ? 100 : (uint8_t)m;
}

float AlashJoystick::angle() {
  int8_t xv = x(), yv = y();
  if (xv == 0 && yv == 0) return NAN;
  float a = atan2((float)yv, (float)xv) * 57.2957795f;
  return a < 0 ? a + 360.0f : a;
}

AlashJoystick::Direction AlashJoystick::direction() {
  int8_t xv = x(), yv = y();
  if (xv == 0 && yv == 0) return DIR_CENTER;
  float a = atan2((float)yv, (float)xv) * 57.2957795f;
  if (a < 0) a += 360.0f;
  uint8_t sector = (uint8_t)((a + 22.5f) / 45.0f) % 8;   // 0 = вправо, 1 = вправо-вверх, ... против часовой
  static const Direction map[8] = {DIR_RIGHT, DIR_UP_RIGHT, DIR_UP, DIR_UP_LEFT, DIR_LEFT, DIR_DOWN_LEFT, DIR_DOWN, DIR_DOWN_RIGHT};
  return map[sector];
}

const __FlashStringHelper *AlashJoystick::directionName() {
  switch (direction()) {
    case DIR_UP: return T("Вверх", "Жоғары", "Up");
    case DIR_UP_RIGHT: return T("Вправо-вверх", "Оңға-жоғары", "Up-right");
    case DIR_RIGHT: return T("Вправо", "Оңға", "Right");
    case DIR_DOWN_RIGHT: return T("Вправо-вниз", "Оңға-төмен", "Down-right");
    case DIR_DOWN: return T("Вниз", "Төмен", "Down");
    case DIR_DOWN_LEFT: return T("Влево-вниз", "Солға-төмен", "Down-left");
    case DIR_LEFT: return T("Влево", "Солға", "Left");
    case DIR_UP_LEFT: return T("Влево-вверх", "Солға-жоғары", "Up-left");
    default: return T("Центр", "Орталық", "Center");
  }
}

void AlashJoystick::pollButton() {
  if (_pb < 0) return;
  uint32_t now = millis();
  bool raw = (digitalRead(_pb) == LOW);
  if (raw != _bRaw) {
    _bRaw = raw;
    _bRawAt = now;
  }
  if (_bRaw != _bDown && (uint32_t)(now - _bRawAt) >= 30) {
    _bDown = _bRaw;
    if (_bDown) _fPress = true;
    else _fClick = true;
  }
}
bool AlashJoystick::isPressed() { pollButton(); return _bDown; }
bool AlashJoystick::pressed() { pollButton(); bool f = _fPress; _fPress = false; return f; }
bool AlashJoystick::clicked() { pollButton(); bool f = _fClick; _fClick = false; return f; }

const __FlashStringHelper *AlashJoystick::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_NOT_CENTERED:
      return T("Центр замерен неверно: отпустите ручку при запуске (или вызовите calibrateCenter() с отпущенной ручкой)",
               "Орталық қате өлшенді: іске қосқанда тұтқаны жіберіңіз (немесе тұтқа жіберулі кезде calibrateCenter() шақырыңыз)",
               "The center was measured wrongly: release the stick at start (or call calibrateCenter() with the stick released)");
    case ERR_NO_SIGNAL:
      return T("Ось стоит на упоре (0 или максимум): проверьте питание модуля (+5 В / 3.3 В, GND) и пины VRx / VRy",
               "Ось шегінде тұр (0 немесе максимум): модуль қуатын (+5 В / 3.3 В, GND) және VRx / VRy пиндерін тексеріңіз",
               "The axis is stuck at its limit (0 or max): check the module power (+5 V / 3.3 V, GND) and the VRx / VRy pins");
  }
  return F("");
}

void AlashJoystick::diagnose(Print &out) {
  if (!_started) begin();
  out.println(T("AlashJoystick: диагностика", "AlashJoystick: диагностика", "AlashJoystick: diagnostics"));
  out.print(T("Сырые значения X, Y: ", "Шикі мәндер X, Y: ", "Raw values X, Y: "));
  out.print((int)rawX());
  out.print(F(", "));
  out.println((int)rawY());
  out.print(T("Центр X, Y: ", "Орталық X, Y: ", "Center X, Y: "));
  out.print((int)_cx);
  out.print(F(", "));
  out.println((int)_cy);
  out.print(T("Диапазон АЦП: ", "АЦП ауқымы: ", "ADC range: "));
  out.print((int)_adcMin);
  out.print(F(" ... "));
  out.println((int)_adcMax);
  out.print(T("X, Y: ", "X, Y: ", "X, Y: "));
  out.print((int)x());
  out.print(F(", "));
  out.println((int)y());
  if (_err != ERR_OK) out.println(errorText());
  out.println(T("Двигайте ручку до упора во все стороны: X и Y должны доходить до -100 и +100. Если не доходят на ESP32, вызовите setAdcRange(0, 4095) до begin(). Если направление обратное, вызовите invertX() или invertY().",
                "Тұтқаны барлық жаққа шегіне дейін жылжытыңыз: X және Y -100 және +100 дейін жетуі керек. ESP32-де жетпесе, begin()-нен бұрын setAdcRange(0, 4095) шақырыңыз. Бағыт кері болса, invertX() немесе invertY() шақырыңыз.",
                "Move the stick fully in every direction: X and Y must reach -100 and +100. On ESP32 call setAdcRange(0, 4095) before begin() if they do not. If a direction is reversed, call invertX() or invertY()."));
}
