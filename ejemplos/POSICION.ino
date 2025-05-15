#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial2
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN

#define RXD2 16  // Pin RX2
#define TXD2 17  // Pin TX2
#define DXL_ID 0 // ID del motor Dynamixel

#define POT_PIN 35 // Pin del potenciómetro

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL);

  dxl.begin(1000000); // Configura el baudrate
  dxl.setPortProtocolVersion(1.0); // Ajusta la versión del protocolo

  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);
}

void loop() {
  int potValue = analogRead(POT_PIN); // Lee el potenciómetro
  int position = map(potValue, 0, 1023, 0, 1023); // Mapea la lectura a un rango de 0 a 4095
  
  dxl.setGoalPosition(DXL_ID, position); // Envía la posición al Dynamixel

  // Imprimir datos para el Serial Plotter
  DEBUG_SERIAL.print("Potenciometro: ");
  DEBUG_SERIAL.print(potValue);
  DEBUG_SERIAL.print("\tDynamixel: ");
  DEBUG_SERIAL.println(position);
  
  DEBUG_SERIAL.print(potValue);
  DEBUG_SERIAL.print("\t");  // Tabulación entre valores
  DEBUG_SERIAL.println(position);


  delay(10); // Pequeña pausa para estabilidad
}
