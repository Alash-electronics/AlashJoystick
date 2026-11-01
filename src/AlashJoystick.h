// AlashJoystick: аналоговый джойстик (две оси и кнопка): калибровка центра, мёртвая зона, значения -100..100, направления.
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashJoystick
#ifndef ALASH_JOYSTICK_H
#define ALASH_JOYSTICK_H

#include <Arduino.h>

class AlashJoystick {
 public:
  // Направления для direction()
  enum Direction : uint8_t {
    DIR_CENTER = 0,
    DIR_UP, DIR_UP_RIGHT, DIR_RIGHT, DIR_DOWN_RIGHT, DIR_DOWN, DIR_DOWN_LEFT, DIR_LEFT, DIR_UP_LEFT
  };
  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_NOT_CENTERED,   // при калибровке ручку держали не в центре
    ERR_NO_SIGNAL       // ось стоит на упоре (0 или максимум): не подключено питание или пин
  };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  // pinX, pinY: аналоговые пины осей (VRx, VRy). pinButton: кнопка SW (-1 = нет).
  AlashJoystick(uint8_t pinX, uint8_t pinY, int8_t pinButton = -1);

  // begin() замеряет центр: в этот момент ручка должна быть отпущена. Диапазон АЦП по умолчанию 0..1023 (Uno);
  // на ESP32 задайте setAdcRange(0, 4095) до begin().
  bool begin();
  bool calibrateCenter();                 // замерить центр заново (ручка отпущена)

  // ---- Значения ----------------------------------------------------------------
  int8_t x();                             // -100 ... 0 (центр) ... +100: плюс там, где растёт значение АЦП
  int8_t y();                             // то же для оси Y. Если у вашего модуля «вверх» оказалось минусом, вызовите invertY()
  int16_t rawX();                         // сырое значение АЦП
  int16_t rawY();
  float angle();                          // угол ручки, градусы: 0 при +X, 90 при +Y; NAN в центре
  uint8_t magnitude();                    // насколько отклонена ручка, 0..100
  Direction direction();                  // 8 направлений или центр
  const __FlashStringHelper *directionName();   // "Вверх", "Вправо-вверх", ... на выбранном языке

  // ---- Кнопка (если указан pinButton) ----------------------------------------------
  bool isPressed();
  bool pressed();                         // только что нажали (один раз)
  bool clicked();                         // короткий клик

  // ---- Настройки -----------------------------------------------------------------
  void setDeadzone(uint8_t percent) { _dead = percent > 50 ? 50 : percent; }   // мёртвая зона у центра, % (по умолчанию 8)
  void setAdcRange(int16_t minRaw, int16_t maxRaw) { _adcMin = minRaw; _adcMax = maxRaw; }   // диапазон АЦП платы
  void invertX(bool on = true) { _invX = on; }
  void invertY(bool on = true) { _invY = on; }
  void setSmoothing(uint8_t n) { _smooth = n < 1 ? 1 : n > 16 ? 16 : n; }       // усреднить n замеров (по умолчанию 4)

  // ---- Диагностика -----------------------------------------------------------------
  void setLanguage(Language lang) { _lang = lang; }
  Error error() const { return _err; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);              // сырые значения, центр, подсказки

 private:
  uint8_t _px, _py;
  int8_t _pb;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;
  int16_t _adcMin = 0, _adcMax = 1023;
  int16_t _cx = 512, _cy = 512;
  uint8_t _dead = 8, _smooth = 4;
  bool _invX = false, _invY = false;
  bool _started = false;

  bool _bRaw = false, _bDown = false;
  uint32_t _bRawAt = 0;
  bool _fPress = false, _fClick = false;

  int16_t sample(uint8_t pin);
  int8_t scale(int16_t raw, int16_t center, bool invert);
  void pollButton();
};

#endif  // ALASH_JOYSTICK_H
