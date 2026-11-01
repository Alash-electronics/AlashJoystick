// AlashJoystick: восемь направлений и угол. Название направления печатается на русском
// (setLanguage меняет язык). В центре печатать не надо.
// Пины: Uno / Nano  VRx=A0, VRy=A1, SW=D2;  ESP32  VRx=GPIO34, VRy=GPIO35, SW=GPIO27 (аналоговые пины ADC1).
#if defined(ESP32)
const uint8_t PIN_X = 34, PIN_Y = 35, PIN_SW = 27;
#elif defined(ESP8266)
const uint8_t PIN_X = A0, PIN_Y = A0, PIN_SW = 13;   // у ESP8266 один аналоговый вход: джойстик с двумя осями не подключить напрямую
#else
const uint8_t PIN_X = A0, PIN_Y = A1, PIN_SW = 2;
#endif
#include <AlashJoystick.h>

AlashJoystick joy(PIN_X, PIN_Y, PIN_SW);
AlashJoystick::Direction last = AlashJoystick::DIR_CENTER;

void setup() {
  Serial.begin(115200);
#if defined(ESP32)
  joy.setAdcRange(0, 4095);               // у ESP32 12-битный АЦП
#endif
  joy.begin();
  joy.setLanguage(AlashJoystick::LANG_RU);
}

void loop() {
  AlashJoystick::Direction d = joy.direction();
  if (d != last) {                        // печатаем только когда направление поменялось
    last = d;
    Serial.print(joy.directionName());
    if (d != AlashJoystick::DIR_CENTER) {
      Serial.print(", угол ");
      Serial.print(joy.angle());
      Serial.print(", сила ");
      Serial.print(joy.magnitude());
      Serial.print(" %");
    }
    Serial.println();
  }
  delay(20);
}
