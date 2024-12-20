
void stopStep() { // выключение двигателя
  Step(0, 0, 0, 0);
}

void rotateR() // 24 шага по часовой стрелке = 1 оборот оси = 1 мм движения штока вниз
{
  for (byte i = 0; i < 6; i++) { // один такой блок сдвигает шток на 1/6 мм
    Step(1, 0, 1, 0);            // один такой шаг сдвигает шток на 1/24 мм
    Step(0, 1, 1, 0);
    Step(0, 1, 0, 1);
    Step(1, 0, 0, 1);
  }
}

void rotateL() // 24 шага по часовой стрелке = 1 оборота оси = 1 мм движения штока вверх
{
  for (byte i = 0; i < 6; i++) { // один такой блок сдвигает шток на 1/6 мм
    Step(1, 0, 0, 1);            // один такой шаг сдвигает шток на 1/24 мм
    Step(0, 1, 0, 1);
    Step(0, 1, 1, 0);
    Step(1, 0, 1, 0);
  }
}

void Step(boolean a, boolean b, boolean c, boolean d) // один шаг
{
  digitalWrite(A, a);
  digitalWrite(B, b);
  digitalWrite(C, c);
  digitalWrite(D, d);
  delay(delStep);
}