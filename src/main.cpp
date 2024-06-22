#include <Arduino.h>
#include <M5Unified.h>

void drawDiceType(int diceType)
{
  M5.Lcd.clear();
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextDatum(TL_DATUM);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print(diceType);
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);
  drawDiceType(4);
}

void loop()
{
  M5.update();
  auto imu_update = M5.Imu.update();
  static int dices[] = {4, 6, 8, 12, 20, 100};
  static int dicesSize = 6;
  float magnitude;
  static float shakeThreshold = 4;
  static int diceIndex = 0;

  static bool isReady = false;
  static bool lastButtonAState = false;

  if (imu_update)
  {
    auto data = M5.Imu.getImuData();
    magnitude = sqrt(data.accel.x * data.accel.x + data.accel.y * data.accel.y + data.accel.z * data.accel.z);
  }

  bool currentButtonAState = M5.BtnA.isPressed();

  if (currentButtonAState && !lastButtonAState)
  {
    drawDiceType(dices[diceIndex]);
    M5.Speaker.tone(4000, 150);
    isReady = true;

    lastButtonAState = currentButtonAState;

    if (isReady)
    {

      if (magnitude > shakeThreshold)
      {

        M5.Lcd.clear();
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setTextSize(7);
        long randNumber = random(1, dices[diceIndex] + 1);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.drawNumber(randNumber, M5.Lcd.width() / 2, M5.Lcd.height() / 2);
        isReady = false;
      }
    }

    if (M5.BtnB.isPressed())
    {
      M5.Speaker.tone(4700, 150);
      diceIndex++;

      if (diceIndex >= dicesSize)
      {
        diceIndex = 0;
      }
      drawDiceType(dices[diceIndex]);
    }

    delay(100);
  }
