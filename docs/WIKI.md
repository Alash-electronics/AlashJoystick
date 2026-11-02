# AlashJoystick: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для **аналоговых джойстиков** (модуль KY-023 и подобные): сама находит центр, убирает дрожание у центра, отдаёт положение от -100 до +100, направление, угол и кнопку. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashJoystick** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Подключение (KY-023)
| Модуль | Uno / Nano | ESP32 |
|---|---|---|
| +5V | 5 В | 3.3 В |
| GND | GND | GND |
| VRx | A0 | GPIO34 |
| VRy | A1 | GPIO35 |
| SW | D2 | GPIO27 |

У ESP32 используйте пины АЦП1 (32-39): пины АЦП2 не работают вместе с Wi-Fi. У ESP8266 один аналоговый вход, на нём джойстик с двумя осями не подключить.

### Быстрый старт
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
- **Сама находит центр** при `begin()` (у каждого джойстика он свой и не равен 512). Заново: `calibrateCenter()`.
- **Мёртвая зона:** дрожание у центра не мешает (`setDeadzone(8)` %), за ней шкала плавно начинается с нуля.
- **Значения:** `x()`, `y()` от -100 до +100; крайние положения дают ровно ±100, даже если центр смещён.
- **Направления:** `direction()` (8 сторон и центр), `directionName()` (по-русски, по-казахски, по-английски), `angle()`, `magnitude()`.
- **Кнопка (SW):** `isPressed()`, `pressed()`, `clicked()`.
- **Arduino и ESP32:** `setAdcRange(0, 4095)` для 12-битного АЦП ESP32. `invertX()`, `invertY()` меняют направление осей.
- **Понятные подсказки** на русском, казахском и английском: `diagnose(Serial)`, `errorText()` (например, «ось стоит на упоре: проверьте питание»).

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin()` | Замерить центр (ручка отпущена) |
| `x() / y()` | Положение от -100 до +100 |
| `rawX() / rawY()` | Сырые значения АЦП |
| `angle() / magnitude()` | Угол и отклонение ручки (0..100) |
| `direction() / directionName()` | 8 направлений или центр, название |
| `clicked() / pressed() / isPressed()` | Кнопка джойстика (SW) |
| `setDeadzone(%)` | Мёртвая зона у центра |
| `setAdcRange(min, max)` | Диапазон АЦП (ESP32: 0..4095) |
| `invertX() / invertY()` | Перевернуть ось |
| `calibrateCenter()` | Замерить центр заново |
| `setReaders(readX, readY)` | Свои функции чтения осей: внешний АЦП (ADS1115), мультиплексор |
| `setButtonMode(mode, activeLow)` | Подтяжка кнопки JOY_PULLUP / JOY_EXTERNAL / JOY_PULLDOWN и уровень нажатой |
| `diagnose(Serial)` | Самопроверка |

### Если что-то не работает
**В покое значения не ноль**

Держите ручку свободно при включении: `begin()` замеряет центр. Потом `calibrateCenter()`; увеличьте мёртвую зону `setDeadzone(12)`.

**На ESP32 диапазон только до 1023**

Вызовите `joy.setAdcRange(0, 4095)` до `begin()`. Используйте пины АЦП1 (32-39): АЦП2 не работает вместе с Wi-Fi.

**Вверх оказался минусом**

Вызовите `joy.invertY()` (или `invertX()`).

**Джойстик на ESP8266 или нужна точность выше**

У ESP8266 один аналоговый вход и он шумный. Подключите джойстик к внешнему АЦП ADS1115 (библиотека AlashADS1115) и передайте свои функции чтения: `joy.setReaders(readX, readY)`, не забудьте `setAdcRange(0, 32767)` под диапазон функции.

### Чем отличается от других библиотек
Мы посмотрели Joystick (Arduino-Joystick-Library), AnalogJoystick, GyverJoystick и примеры из курсов:
- Большинство предполагает центр ровно 512 и даёт дрейф или «ползущего» робота; у нас центр замеряется.
- Нет мёртвой зоны, направлений и угла в одной библиотеке.
- Нет поддержки 12-битного АЦП ESP32 и подсказок, что делать, если значения неверные: у нас `diagnose()`.

Если у вашего модуля «вверх» оказалось минусом, вызовите `joy.invertY()`: полярность осей у разных модулей разная.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `Directions`, `Servo_AVR`, `Diagnose`.

---

## Қазақша

**Аналогтық джойстиктерге** (KY-023 модулі және ұқсастары) арналған қарапайым кітапхана: орталықты өзі табады, орталықтағы дірілді жояды, жағдайды -100-ден +100-ге дейін, бағытты, бұрышты және түймені береді. Тәуелділіксіз. Alash Electronics сабақтары мен жинақтарына арналған.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashJoystick** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Қосу (KY-023)
| Модуль | Uno / Nano | ESP32 |
|---|---|---|
| +5V | 5 В | 3.3 В |
| GND | GND | GND |
| VRx | A0 | GPIO34 |
| VRy | A1 | GPIO35 |
| SW | D2 | GPIO27 |

### Жылдам бастау
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
- **Орталықты өзі табады** `begin()` кезінде. Қайтадан: `calibrateCenter()`.
- **Өлі аймақ:** `setDeadzone(8)` %.
- **Мәндер:** `x()`, `y()` -100-ден +100-ге дейін.
- **Бағыттар:** `direction()` (8 бағыт және орталық), `directionName()`, `angle()`, `magnitude()`.
- **Түйме (SW):** `isPressed()`, `pressed()`, `clicked()`.
- **Arduino және ESP32:** `setAdcRange(0, 4095)`. `invertX()`, `invertY()`.
- **Түсінікті кеңестер** орысша, қазақша, ағылшынша: `diagnose(Serial)`, `errorText()`.

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin()` | Центрді өлшеу (тұтқа жіберілген) |
| `x() / y()` | -100-ден +100-ге дейінгі орын |
| `rawX() / rawY()` | АЦП шикі мәндері |
| `angle() / magnitude()` | Тұтқа бұрышы және ауытқуы (0..100) |
| `direction() / directionName()` | 8 бағыт не орталық, атауы |
| `clicked() / pressed() / isPressed()` | Джойстик түймесі (SW) |
| `setDeadzone(%)` | Орталықтағы өлі аймақ |
| `setAdcRange(min, max)` | АЦП диапазоны (ESP32: 0..4095) |
| `invertX() / invertY()` | Осьті аудару |
| `calibrateCenter()` | Центрді қайта өлшеу |
| `setReaders(readX, readY)` | Осьтерді оқу үшін өз функциялары |
| `setButtonMode(mode, activeLow)` | Түйме тартқышы және басылған деңгей |
| `diagnose(Serial)` | Өзін тексеру |

### Бірдеңе жұмыс істемесе
**Тыныштықта мәндер нөл емес**

Қосқанда тұтқаны жіберіп тұрыңыз: `begin()` центрді өлшейді. Сосын `calibrateCenter()`; `setDeadzone(12)` үлкейтіңіз.

**ESP32-де диапазон тек 1023-ке дейін**

`begin()` алдында `joy.setAdcRange(0, 4095)` шақырыңыз. АЦП1 пиндерін (32-39) қолданыңыз.

**Жоғары минус болып шықты**

`joy.invertY()` (не `invertX()`) шақырыңыз.

**ESP8266-дағы джойстик немесе жоғары дәлдік керек**

ESP8266-да бір аналогтық кіріс бар және ол шулы. Джойстикті сыртқы ADS1115 АЦП-ына (AlashADS1115 кітапханасы) қосып, өз оқу функцияларыңызды беріңіз: `joy.setReaders(readX, readY)`, функция диапазонына `setAdcRange(0, 32767)` беруді ұмытпаңыз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `Directions`, `Servo_AVR`, `Diagnose`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library for **analog joysticks** (KY-023 module and similar): finds the center by itself, ignores jitter near the center, returns the position from -100 to +100, direction, angle and the button. No dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashJoystick** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Wiring (KY-023)
| Module | Uno / Nano | ESP32 |
|---|---|---|
| +5V | 5 V | 3.3 V |
| GND | GND | GND |
| VRx | A0 | GPIO34 |
| VRy | A1 | GPIO35 |
| SW | D2 | GPIO27 |

If "up" turns out negative on your module, call `joy.invertY()`: axis polarity differs between modules.

### Quick start
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
- **Finds the center** in `begin()` (it is never exactly 512). Again: `calibrateCenter()`.
- **Dead zone:** `setDeadzone(8)` %, the scale restarts smoothly from 0 after it.
- **Values:** `x()`, `y()` from -100 to +100; the extremes give exactly ±100 even with an off-center rest position.
- **Directions:** `direction()` (8 sides + center), `directionName()`, `angle()`, `magnitude()`.
- **Button (SW):** `isPressed()`, `pressed()`, `clicked()`.
- **Arduino and ESP32:** `setAdcRange(0, 4095)` for the 12-bit ADC. `invertX()`, `invertY()`.
- **Clear tips** in Russian, Kazakh and English: `diagnose(Serial)`, `errorText()`.

### Method reference
| Method | What it does |
|---|---|
| `begin()` | Measure the center (stick released) |
| `x() / y()` | Position from -100 to +100 |
| `rawX() / rawY()` | Raw ADC values |
| `angle() / magnitude()` | Angle and deflection (0..100) |
| `direction() / directionName()` | 8 directions or center, and its name |
| `clicked() / pressed() / isPressed()` | The joystick button (SW) |
| `setDeadzone(%)` | Dead zone around the center |
| `setAdcRange(min, max)` | ADC range (ESP32: 0..4095) |
| `invertX() / invertY()` | Flip an axis |
| `calibrateCenter()` | Measure the center again |
| `setReaders(readX, readY)` | Your own axis read functions: an external ADC (ADS1115), a multiplexer |
| `setButtonMode(mode, activeLow)` | Button pull JOY_PULLUP / JOY_EXTERNAL / JOY_PULLDOWN and the pressed level |
| `diagnose(Serial)` | Self-check |

### If something does not work
**Values are not zero at rest**

Keep the stick free at start-up: `begin()` measures the center. Then `calibrateCenter()`; raise the dead zone `setDeadzone(12)`.

**On ESP32 the range stops at 1023**

Call `joy.setAdcRange(0, 4095)` before `begin()`. Use ADC1 pins (32-39): ADC2 does not work with Wi-Fi.

**Up is negative**

Call `joy.invertY()` (or `invertX()`).

**A joystick on an ESP8266 or you need higher precision**

The ESP8266 has one analog input and it is noisy. Connect the joystick to an external ADS1115 ADC (the AlashADS1115 library) and pass your own read functions: `joy.setReaders(readX, readY)`, and do not forget `setAdcRange(0, 32767)` for the function range.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `Directions`, `Servo_AVR`, `Diagnose`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashJoystick
