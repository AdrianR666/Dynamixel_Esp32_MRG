#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial2
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN

#define RXD2 16  // Pin RX2
#define TXD2 17  // Pin TX2

// IDs de los Dynamixel detectados
int dxl_ids[] = {1, 2, 3, 4, 5};  
const int numMotors = 4;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL);

  dxl.begin(1000000); // Configura el baudrate
  dxl.setPortProtocolVersion(1.0); // Protocolo 1.0 (AX-12A, AX-12W)

  // Inicializar cada motor
  for (int i = 0; i < numMotors; i++) {
    dxl.torqueOff(dxl_ids[i]);
    dxl.setOperatingMode(dxl_ids[i], OP_POSITION);
    dxl.torqueOn(dxl_ids[i]);
  }

  DEBUG_SERIAL.println("Sistema listo.");
  DEBUG_SERIAL.println("Escribe: <ID> <ANGULO>");
  DEBUG_SERIAL.println("Ejemplo: 3 90 (mueve motor con ID=3 a 90 grados)");
}

void loop() {
  if (DEBUG_SERIAL.available() > 0) {
    int id = DEBUG_SERIAL.parseInt();       // Primer número = ID
    int angle = DEBUG_SERIAL.parseInt();    // Segundo número = Ángulo (0–180)

    if (id >= 1 && id <= 5) { // Validar ID válido
      if (angle >= 0 && angle <= 300) { 
        int position = map(angle, 0, 300, 0, 1023); // Conversión a unidades Dynamixel
        dxl.setGoalPosition(id, position);

        DEBUG_SERIAL.print("Motor ");
        DEBUG_SERIAL.print(id);
        DEBUG_SERIAL.print(" movido a ");
        DEBUG_SERIAL.print(angle);
        DEBUG_SERIAL.print(" grados (posicion interna: ");
        DEBUG_SERIAL.print(position);
        DEBUG_SERIAL.println(")");
      } else {
        DEBUG_SERIAL.println("Ángulo invalido (0 - 180 grados).");
      }
    } else {
      DEBUG_SERIAL.println("ID invalido (usa 2,3,4,5).");
    }
  }
}

