#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial2
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN

#define RXD2 16  // Pin RX2
#define TXD2 17  // Pin TX2
#define DXL_ID 0 // ID del motor Dynamixel

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL);

  dxl.begin(1000000); // Configura el baudrate
  dxl.setPortProtocolVersion(1.0); // Ajusta la versión del protocolo

  // Verificar si el motor está conectado
  if (!dxl.ping(DXL_ID)) {
    DEBUG_SERIAL.println("Error: Motor no encontrado. Verifica la conexión.");
    while (1); // Detener ejecución si no se encuentra el motor
  }

  DEBUG_SERIAL.println("Motor detectado correctamente.");
  
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_VELOCITY); // Cambia a modo de control de velocidad
  dxl.torqueOn(DXL_ID);
}

void loop() {
  DEBUG_SERIAL.println("Girando en sentido horario...");
  dxl.setGoalVelocity(DXL_ID, 1024 + 500); // Velocidad positiva (CW)
  delay(3000);

  DEBUG_SERIAL.println("Deteniendo...");
  dxl.setGoalVelocity(DXL_ID, 0); // Detener
  delay(2000);

  DEBUG_SERIAL.println("Girando en sentido antihorario...");
  dxl.setGoalVelocity(DXL_ID, 500); // Velocidad negativa (CCW)
  delay(3000);

  DEBUG_SERIAL.println("Deteniendo...");
  dxl.setGoalVelocity(DXL_ID, 0); // Detener
  delay(2000);
}
