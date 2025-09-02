#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial2
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN

#define RXD2 16  // Pin RX2
#define TXD2 17  // Pin TX2
#define DXL_ID 2 // ID del motor Dynamixel

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

  DEBUG_SERIAL.println("Introduce la posicion (0 - 1023) y presiona ENTER:");
}

void loop() {
  if (DEBUG_SERIAL.available() > 0) {
    int position = DEBUG_SERIAL.parseInt();  // Lee número ingresado
    if (position >= 0 && position <= 1023) { // Validación
      dxl.setGoalPosition(DXL_ID, position);
      DEBUG_SERIAL.print("Posicion enviada al Dynamixel: ");
      DEBUG_SERIAL.println(position);
    } else {
      DEBUG_SERIAL.println("Valor invalido. Ingresa un numero entre 0 y 1023.");
    }
  }
}
