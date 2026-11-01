// Тесты AlashJoystick без железа (значения АЦП и время задаёт тест):
//   g++ -std=c++17 -Wall -I mock -I ../../src test_joystick.cpp ../../src/AlashJoystick.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "AlashJoystick.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static void stick(int x, int y) { g_analog[1] = x; g_analog[2] = y; }

int main() {
  // 1) центр у реального джойстика не ровно 512: после калибровки это ноль
  stick(530, 495);
  AlashJoystick j(1, 2, 3);
  CHECK(j.begin());
  CHECK(j.x() == 0 && j.y() == 0);
  CHECK(j.direction() == AlashJoystick::DIR_CENTER);
  CHECK(isnan(j.angle()));

  // 2) крайние положения дают ±100 даже с несимметричным центром
  stick(1023, 495); CHECK(j.x() == 100);
  stick(0, 495);    CHECK(j.x() == -100);
  stick(530, 1023); CHECK(j.y() == 100);
  stick(530, 0);    CHECK(j.y() == -100);

  // 3) мёртвая зона: маленькие колебания у центра = 0, дальше шкала плавная
  stick(540, 495); CHECK(j.x() == 0);
  stick(530 + 80, 495); CHECK(j.x() > 0 && j.x() < 20);
  j.setDeadzone(0); stick(530 + 80, 495); CHECK(j.x() > 8);
  j.setDeadzone(8);

  // 4) направления
  stick(1023, 495); CHECK(j.direction() == AlashJoystick::DIR_RIGHT);
  stick(0, 495);    CHECK(j.direction() == AlashJoystick::DIR_LEFT);
  stick(530, 1023); CHECK(j.direction() == AlashJoystick::DIR_UP);
  stick(530, 0);    CHECK(j.direction() == AlashJoystick::DIR_DOWN);
  stick(1023, 1023); CHECK(j.direction() == AlashJoystick::DIR_UP_RIGHT);
  stick(0, 0);      CHECK(j.direction() == AlashJoystick::DIR_DOWN_LEFT);
  stick(1023, 0);   CHECK(j.direction() == AlashJoystick::DIR_DOWN_RIGHT);
  stick(0, 1023);   CHECK(j.direction() == AlashJoystick::DIR_UP_LEFT);

  // 5) угол и сила отклонения
  stick(1023, 495); CHECK(fabsf(j.angle() - 0.0f) < 1.0f && j.magnitude() == 100);
  stick(530, 1023); CHECK(fabsf(j.angle() - 90.0f) < 1.0f);
  stick(0, 495);    CHECK(fabsf(j.angle() - 180.0f) < 1.0f);
  stick(530, 0);    CHECK(fabsf(j.angle() - 270.0f) < 1.0f);
  stick(1023, 1023); CHECK(j.magnitude() == 100);        // по диагонали не больше 100

  // 6) инверсия осей
  j.invertX(); stick(1023, 495); CHECK(j.x() == -100);
  j.invertX(false);
  j.invertY(); stick(530, 1023); CHECK(j.y() == -100);
  j.invertY(false);

  // 7) ESP32: диапазон 0..4095, центр около 1880
  AlashJoystick e(1, 2);
  e.setAdcRange(0, 4095);
  stick(1880, 2010);
  CHECK(e.begin());
  stick(4095, 2010); CHECK(e.x() == 100);
  stick(0, 2010);    CHECK(e.x() == -100);

  // 8) ошибки калибровки: ручка в углу при запуске, оборванный провод
  AlashJoystick bad(1, 2);
  stick(1000, 500);
  CHECK(!bad.begin() && bad.error() == AlashJoystick::ERR_NOT_CENTERED);
  stick(0, 0);
  CHECK(!bad.calibrateCenter() && bad.error() == AlashJoystick::ERR_NO_SIGNAL);
  stick(512, 512);
  CHECK(bad.calibrateCenter() && bad.error() == AlashJoystick::ERR_OK);

  // 9) кнопка
  AlashJoystick b(1, 2, 3);
  stick(512, 512); g_level[3] = 1; b.begin();
  g_level[3] = 0; for (int i = 0; i < 10; i++) { g_now += 10; b.isPressed(); }
  CHECK(b.pressed() && b.isPressed());
  g_level[3] = 1; for (int i = 0; i < 10; i++) { g_now += 10; b.isPressed(); }
  CHECK(b.clicked() && !b.isPressed());

  // 10) названия и diagnose на трёх языках
  j.setLanguage(AlashJoystick::LANG_EN);
  stick(1023, 495);
  CHECK(strcmp((const char *)j.directionName(), "Right") == 0);
  j.setLanguage(AlashJoystick::LANG_RU);
  CHECK(strcmp((const char *)j.directionName(), "Вправо") == 0);
  j.diagnose(*(new Print));

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
