#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define pulsoTriac 5 //D1
#define cruceZero 14 //D5

int Slider=0;
int brillo;
bool bandera = 0;

char auth[] = "";    // Auth Token, disponible al crear la aplicación en Blynk.
char ssid[] = "";    // Nombre de la red Wifi.
char pass[] = "";    // Contraseña de la red Wifi.

void ICACHE_RAM_ATTR acon();  // Evita errores debido a la interrupción (Línea de código recomendada)

BLYNK_WRITE(V1)   // Función para la detección de variables de Blynk
{
  Slider = param.asInt(); // Asigna el valor de V1 proveniente de Blynk a una variable del microcontrolador
}

void setup()
{
  Serial.begin(9600);
  pinMode(cruceZero, INPUT_PULLUP);
  pinMode(pulsoTriac, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  attachInterrupt(digitalPinToInterrupt(cruceZero), acon, FALLING); // Interrupción en el pin 14 = D5 de la nodeMCU
}

void loop()
{
  Blynk.run();
  brillo = map(Slider, 0, 1023,7200, 100); // Instante en el que se dispara el Triac: 0.1ms para máximo brillo, 72.0ms para mínimo
  if(bandera == 1)
  {
    delayMicroseconds(brillo);
    digitalWrite(pulsoTriac, HIGH);

    delayMicroseconds(80);  // Espera 0.08ms antes de apagar el Triac para que conduzca correctamente.
    digitalWrite(pulsoTriac, LOW);
    bandera = 0;  // Se restablece a la bandera.
  }
}

void acon()
{
  bandera = 1;  // Una sola instrucción que no le toma mucho al microcontrolador correrla cuando entra en la subrutina de interrupción.
}
