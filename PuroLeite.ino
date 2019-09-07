#define S0    D4
#define S1    D5
#define S2    D6
#define S3    D7
#define OUT   D8

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WidgetLCD lcd(V2);

char auth[] = "mS2ugn3FTAyWVUM_ix3s1R8fRCBPuj_j";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PuroLeite";
char pass[] = "passLeitePuro";

int red = 0;
int green = 0;
int blue = 0;
String strColor;

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();

  Serial.print("Vermelho :");
  Serial.print(red, DEC);
  Serial.print(" Verde : ");
  Serial.print(green, DEC);
  Serial.print(" Azul : ");
  Serial.print(blue, DEC);
  Serial.println();

  if (red < blue && red < green && red < 100)
    strColor = F("Vermelho");
  else if (blue < red && blue < green && blue < 1000)
    strColor = F("Azul");
  else if (green < red && green < blue)
    strColor = F("Verde");

  Blynk.virtualWrite(V1, red);
  lcd.clear();
  lcd.print(4, 0, F("Coloração"));
  lcd.print(4, 1, strColor);

}

BLYNK_WRITE(V0)
{
  Serial.println(F("Blink"));
  color();
}

void color()
{
  Serial.println(F("COLOR"));
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);
  digitalWrite(S3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);
  digitalWrite(S2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);
}
