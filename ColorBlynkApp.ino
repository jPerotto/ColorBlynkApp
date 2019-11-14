#define DEBUG
//------------FILTRO PASSA BAIXA----------
#define Fcut                  1         //Cut-OFF Frequency
#define Ts                    0.01      //Sampling time
#define Pi                    3.141     //value of pi
//------------PINAGEM----------
#define S0    D4
#define S1    D5
#define S2    D6
#define S3    D7
#define OUT   D8

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WidgetLCD lcd(V2);

//char auth[] = "mS2ugn3FTAyWVUM_ix3s1R8fRCBPuj_j";
//char auth[] = "CHAVA_DO_NOVO_APP";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CervejaPura";
char pass[] = "puracerveja";

//char ssid[] = "Magneto";
//char pass[] = "Sup3r@R3d3";

float red = 0;
float green = 0;
float blue = 0;
String impureza;
String pureza;
boolean leitura = false;
boolean Connect = false;

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  if (Serial)
    Serial.println(F("SETUP"));
#endif
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

  if (leitura)
  {
    float vlrPercent = (blue / (red + green)) * 100;
    if (vlrPercent >= 100)
      vlrPercent = 100.00;

    pureza = "Cerveja: " + String(vlrPercent);
    impureza = "Outros: " + String(100 - vlrPercent);

    Blynk.virtualWrite(V1, vlrPercent);
    lcd.clear();
    lcd.print(0, 0, pureza);
    lcd.print(0, 1, impureza);
#ifdef DEBUG
    if (Serial)
      Serial.print("R: ");
    Serial.print(red, DEC);
    Serial.print(" G: ");
    Serial.print(green, DEC);
    Serial.print(" B: ");
    Serial.print(blue, DEC);
    Serial.print(F(" Percent: "));
    Serial.println(vlrPercent);
#endif
    leitura = false;
  } else if (!Connect)
  {
    lcd.clear();
    lcd.print(4, 0, F("Conectado"));
    lcd.print(4, 1, F("Aguardando!"));
  }
  delay(1000);
}

BLYNK_WRITE(V0)
{
#ifdef DEBUG
  if (Serial)
    Serial.println(F("Blink"));
#endif
  lcd.clear();
  lcd.print(0, 0, F("Fazendo leitura"));
  lcd.print(4, 1, F("Aguarde!"));
  color();
}

void color()
{
#ifdef DEBUG
  if (Serial)
    Serial.println(F("COLOR"));
#endif
  digitalWrite(S0, HIGH);
  delay(100);
  for (uint8_t x = 0; x < 50; x++)
  {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = lowPass(red, pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH));    //count OUT, pRed, RED
    digitalWrite(S3, HIGH);
    blue = lowPass(blue, pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH));   //count OUT, pBLUE, BLUE
    digitalWrite(S2, HIGH);
    green = lowPass(green, pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH));  //count OUT, pGreen, GREEN
    delay(100);
  }
  digitalWrite(S0, LOW);
  leitura = true;
  Connect = true;
  lcd.clear();
  lcd.print(2, 0, F("Processando"));
  lcd.print(4, 1, F("Aguarde!"));
  delay(2000);
}

uint16_t lowPass(uint16_t x, uint16_t u)
{
  x = x + Ts * (2 * Pi * Fcut * u - 2 * Pi * Fcut * x);
  return (uint16_t)x;
}
