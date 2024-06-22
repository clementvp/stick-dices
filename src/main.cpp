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
  auto cfg = M5.config(); // Assign a structure for initializing M5Stack
  M5.begin(cfg);
  drawDiceType(3);
}

void loop()
{
  M5.update();
  auto imu_update = M5.Imu.update();
  static int dices[] = {3, 6, 8, 12, 20, 100};
  static int dicesSize = 6;
  float magnitude;
  static float shakeThreshold = 4;
  static int diceIndex = 0;

  static bool isReady = false;
  static bool lastButtonAState = false; // Variable pour stocker le dernier état du bouton

  if (imu_update)
  {
    auto data = M5.Imu.getImuData();
    magnitude = sqrt(data.accel.x * data.accel.x + data.accel.y * data.accel.y + data.accel.z * data.accel.z);
  }

  bool currentButtonAState = M5.BtnA.isPressed(); // Lit l'état actuel du bouton

  // Vérifie si le bouton a été relâché et si l'état a changé
  if (currentButtonAState && !lastButtonAState)
  {
    drawDiceType(dices[diceIndex]);
    M5.Speaker.tone(4000, 150);
    isReady = true; // Le bouton a été relâché, prêt à déclencher la fonction
  }

  // Met à jour l'état précédent du bouton
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
      isReady = false; // Réinitialise l'état pour nécessiter une nouvelle pression du bouton
    }
  }

  // Vérifier si le bouton a été appuyé
  if (M5.BtnB.isPressed())
  {
    M5.Speaker.tone(4700, 150);
    // Incrémenter l'index
    diceIndex++;

    // Revenir au début du tableau si on atteint la fin
    if (diceIndex >= dicesSize)
    {
      diceIndex = 0;
    }
    drawDiceType(dices[diceIndex]);
  }

  delay(100);
}
