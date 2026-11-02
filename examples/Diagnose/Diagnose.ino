// [ru] AlashJoystick: джойстик ведёт себя странно? Скетч покажет сырые значения, центр и подскажет настройки.
//      Пины: Uno / Nano  VRx=A0, VRy=A1, SW=D2;  ESP32  VRx=GPIO34, VRy=GPIO35, SW=GPIO27 (аналоговые пины ADC1).
// [kk] AlashJoystick: джойстик біртүрлі жұмыс істей ме? Скетч шикі мәндерді, орталықты көрсетеді және баптауларды ұсынады.
//      Пиндер: Uno / Nano  VRx=A0, VRy=A1, SW=D2;  ESP32  VRx=GPIO34, VRy=GPIO35, SW=GPIO27 (ADC1 аналогтық пиндері).
// [en] AlashJoystick: the joystick behaves strangely? The sketch shows the raw values, the center and suggests settings.
//      Pins: Uno / Nano  VRx=A0, VRy=A1, SW=D2;  ESP32  VRx=GPIO34, VRy=GPIO35, SW=GPIO27 (ADC1 analog pins).
#if defined(ESP32)
const uint8_t PIN_X = 34, PIN_Y = 35, PIN_SW = 27;
#elif defined(ESP8266)
const uint8_t PIN_X = A0, PIN_Y = A0, PIN_SW = 13;   // у ESP8266 один аналоговый вход: джойстик с двумя осями не подключить напрямую
#else
const uint8_t PIN_X = A0, PIN_Y = A1, PIN_SW = 2;
#endif
#include <AlashJoystick.h>

AlashJoystick joy(PIN_X, PIN_Y, PIN_SW);

void setup() {
  Serial.begin(115200);
  delay(500);
#if defined(ESP32)
  joy.setAdcRange(0, 4095);               // у ESP32 12-битный АЦП
#endif
  joy.setLanguage(AlashJoystick::LANG_RU);   // LANG_KK: қазақша, LANG_EN: English
  joy.begin();
}

void loop() {
  joy.diagnose(Serial);
  Serial.println();
  delay(1000);
}
