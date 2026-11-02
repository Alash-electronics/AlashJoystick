# AlashJoystick

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для **аналоговых джойстиков** (модуль KY-023 и подобные): сама находит центр, убирает дрожание у центра, отдаёт положение от -100 до +100, направление, угол и кнопку. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashJoystick.h>
AlashJoystick joy(A0, A1, 2);      // VRx, VRy, SW

void setup() { Serial.begin(115200); joy.begin(); }   // ручка отпущена!
void loop() {
  Serial.println(joy.x());         // -100 ... +100
  delay(100);
}
```

### Что умеет
- **Любой источник данных:** `setReaders(readX, readY)` для внешнего АЦП (ADS1115) или мультиплексора; кнопка с любой подтяжкой и уровнем `setButtonMode()`.
- **Сама находит центр** при `begin()` (у каждого джойстика он свой и не равен 512). Заново: `calibrateCenter()`.
- **Мёртвая зона:** дрожание у центра не мешает (`setDeadzone(8)` %), за ней шкала плавно начинается с нуля.
- **Значения:** `x()`, `y()` от -100 до +100; крайние положения дают ровно ±100, даже если центр смещён.
- **Направления:** `direction()` (8 сторон и центр), `directionName()` (по-русски, по-казахски, по-английски), `angle()`, `magnitude()`.
- **Кнопка (SW):** `isPressed()`, `pressed()`, `clicked()`.
- **Arduino и ESP32:** `setAdcRange(0, 4095)` для 12-битного АЦП ESP32. `invertX()`, `invertY()` меняют направление осей.
- **Понятные подсказки** на русском, казахском и английском: `diagnose(Serial)`, `errorText()` (например, «ось стоит на упоре: проверьте питание»).

### Подключение (KY-023)
| Модуль | Uno / Nano | ESP32 |
|---|---|---|
| +5V | 5 В | 3.3 В |
| GND | GND | GND |
| VRx | A0 | GPIO34 |
| VRy | A1 | GPIO35 |
| SW | D2 | GPIO27 |

У ESP32 используйте пины АЦП1 (32-39): пины АЦП2 не работают вместе с Wi-Fi. У ESP8266 один аналоговый вход, на нём джойстик с двумя осями не подключить.

### Чем отличается от других библиотек
Мы посмотрели Joystick (Arduino-Joystick-Library), AnalogJoystick, GyverJoystick и примеры из курсов:
- Большинство предполагает центр ровно 512 и даёт дрейф или «ползущего» робота; у нас центр замеряется.
- Нет мёртвой зоны, направлений и угла в одной библиотеке.
- Нет поддержки 12-битного АЦП ESP32 и подсказок, что делать, если значения неверные: у нас `diagnose()`.

Если у вашего модуля «вверх» оказалось минусом, вызовите `joy.invertY()`: полярность осей у разных модулей разная.

### Примеры
`Basic`, `Directions`, `Servo_AVR`, `Diagnose`.

### Тесты
Центр, мёртвая зона, ±100 по краям, направления, угол, диапазоны АЦП, ошибки калибровки проверены без железа: `extras/test`.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

**Аналогтық джойстиктерге** (KY-023 модулі және ұқсастары) арналған қарапайым кітапхана: орталықты өзі табады, орталықтағы дірілді жояды, жағдайды -100-ден +100-ге дейін, бағытты, бұрышты және түймені береді. Тәуелділіксіз. Alash Electronics сабақтары мен жинақтарына арналған.

```cpp
#include <AlashJoystick.h>
AlashJoystick joy(A0, A1, 2);      // VRx, VRy, SW

void setup() { Serial.begin(115200); joy.begin(); }   // тұтқа жіберулі!
void loop() {
  Serial.println(joy.x());         // -100 ... +100
  delay(100);
}
```

### Мүмкіндіктері
- **Деректердің кез келген көзі:** сыртқы АЦП (ADS1115) немесе мультиплексор үшін `setReaders(readX, readY)`; кез келген тартқышты және деңгейлі түйме `setButtonMode()`.
- **Орталықты өзі табады** `begin()` кезінде. Қайтадан: `calibrateCenter()`.
- **Өлі аймақ:** `setDeadzone(8)` %.
- **Мәндер:** `x()`, `y()` -100-ден +100-ге дейін.
- **Бағыттар:** `direction()` (8 бағыт және орталық), `directionName()`, `angle()`, `magnitude()`.
- **Түйме (SW):** `isPressed()`, `pressed()`, `clicked()`.
- **Arduino және ESP32:** `setAdcRange(0, 4095)`. `invertX()`, `invertY()`.
- **Түсінікті кеңестер** орысша, қазақша, ағылшынша: `diagnose(Serial)`, `errorText()`.

### Қосу (KY-023)
| Модуль | Uno / Nano | ESP32 |
|---|---|---|
| +5V | 5 В | 3.3 В |
| GND | GND | GND |
| VRx | A0 | GPIO34 |
| VRy | A1 | GPIO35 |
| SW | D2 | GPIO27 |

### Мысалдар
`Basic`, `Directions`, `Servo_AVR`, `Diagnose`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library for **analog joysticks** (KY-023 module and similar): finds the center by itself, ignores jitter near the center, returns the position from -100 to +100, direction, angle and the button. No dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashJoystick.h>
AlashJoystick joy(A0, A1, 2);      // VRx, VRy, SW

void setup() { Serial.begin(115200); joy.begin(); }   // stick released!
void loop() {
  Serial.println(joy.x());         // -100 ... +100
  delay(100);
}
```

### Features
- **Any data source:** `setReaders(readX, readY)` for an external ADC (ADS1115) or a multiplexer; a button with any pull and level via `setButtonMode()`.
- **Finds the center** in `begin()` (it is never exactly 512). Again: `calibrateCenter()`.
- **Dead zone:** `setDeadzone(8)` %, the scale restarts smoothly from 0 after it.
- **Values:** `x()`, `y()` from -100 to +100; the extremes give exactly ±100 even with an off-center rest position.
- **Directions:** `direction()` (8 sides + center), `directionName()`, `angle()`, `magnitude()`.
- **Button (SW):** `isPressed()`, `pressed()`, `clicked()`.
- **Arduino and ESP32:** `setAdcRange(0, 4095)` for the 12-bit ADC. `invertX()`, `invertY()`.
- **Clear tips** in Russian, Kazakh and English: `diagnose(Serial)`, `errorText()`.

### Wiring (KY-023)
| Module | Uno / Nano | ESP32 |
|---|---|---|
| +5V | 5 V | 3.3 V |
| GND | GND | GND |
| VRx | A0 | GPIO34 |
| VRy | A1 | GPIO35 |
| SW | D2 | GPIO27 |

If "up" turns out negative on your module, call `joy.invertY()`: axis polarity differs between modules.

### Examples
`Basic`, `Directions`, `Servo_AVR`, `Diagnose`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

### License
MIT, Alash Electronics.
