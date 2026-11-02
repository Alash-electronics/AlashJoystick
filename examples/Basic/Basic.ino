// [ru] AlashJoystick: самый простой пример. Положение джойстика от -100 до +100 по двум осям.
//
//      Подключение модуля KY-023: +5V к 5 В (ESP32: 3.3 В), GND к GND, VRx и VRy на аналоговые пины, SW на кнопку.
//      При запуске ручка должна быть отпущена: по ней библиотека находит центр.
//      Пины: Uno / Nano  VRx=A0, VRy=A1, SW=D2;  ESP32  VRx=GPIO34, VRy=GPIO35, SW=GPIO27 (аналоговые пины ADC1).
// [kk] AlashJoystick: ең қарапайым мысал. Джойстиктің екі ось бойынша -100-ден +100-ге дейінгі орны.
//
//      KY-023 модулін қосу: +5V 5 В-қа (ESP32: 3.3 В), GND GND-ға, VRx және VRy аналогтық пиндерге, SW батырмаға.
//      Іске қосқанда тұтқа босатылған болуы керек: кітапхана орталықты содан табады.
//      Пиндер: Uno / Nano  VRx=A0, VRy=A1, SW=D2;  ESP32  VRx=GPIO34, VRy=GPIO35, SW=GPIO27 (ADC1 аналогтық пиндері).
// [en] AlashJoystick: the simplest example. The joystick position from -100 to +100 on two axes.
//
//      KY-023 module wiring: +5V to 5 V (ESP32: 3.3 V), GND to GND, VRx and VRy to analog pins, SW to the button.
//      At start the stick must be released: the library finds the center from it.
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
#if defined(ESP32)
  joy.setAdcRange(0, 4095);               // у ESP32 12-битный АЦП
#endif
  if (!joy.begin()) Serial.println(joy.errorText());   // замеряет центр
}

void loop() {
  Serial.print("X: ");
  Serial.print(joy.x());
  Serial.print("  Y: ");
  Serial.print(joy.y());
  Serial.println(joy.isPressed() ? "  кнопка нажата" : "");
  delay(100);
}
