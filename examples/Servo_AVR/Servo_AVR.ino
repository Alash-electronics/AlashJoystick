// AlashJoystick: джойстик управляет двумя сервоприводами (например, поворот камеры).
// Сервоприводы на пины 9 и 10.
// Скетч для плат Arduino AVR (Uno / Nano / Mega) со стандартной библиотекой Servo.
#include <AlashJoystick.h>
#include <Servo.h>

AlashJoystick joy(A0, A1);
Servo panServo, tiltServo;
float pan = 90, tilt = 90;

void setup() {
  joy.begin();
  panServo.attach(9);
  tiltServo.attach(10);
}

void loop() {
  // Ручка отклонена: угол плавно меняется, отпущена: сервопривод стоит на месте
  pan = constrain(pan + joy.x() * 0.05, 0, 180);
  tilt = constrain(tilt + joy.y() * 0.05, 0, 180);
  panServo.write((int)pan);
  tiltServo.write((int)tilt);
  delay(15);
}
