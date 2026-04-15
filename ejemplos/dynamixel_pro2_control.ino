#include <Arduino.h>
#include <Dynamixel2Arduino.h>

#define DXL_SERIAL   Serial2
#define DEBUG_SERIAL Serial
#define DXL_DIR_PIN  2

#define DXL_RX_PIN 16
#define DXL_TX_PIN 17

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const uint8_t ID1 = 1;
const uint8_t ID2 = 2;
const uint32_t DXL_BAUD = 1000000;

bool motor1_ok = false;
bool motor2_ok = false;

// =========================
// FUNCIONES AUXILIARES
// =========================
void printMenu() {
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println("==============================================");
  DEBUG_SERIAL.println(" MENU XL430 - 2 MOTORES");
  DEBUG_SERIAL.println("==============================================");
  DEBUG_SERIAL.println("1  -> LED ON en ambos");
  DEBUG_SERIAL.println("2  -> LED OFF en ambos");
  DEBUG_SERIAL.println("3  -> Torque ON en ambos");
  DEBUG_SERIAL.println("4  -> Torque OFF en ambos");
  DEBUG_SERIAL.println("5  -> Position Mode en ambos");
  DEBUG_SERIAL.println("6  -> Velocity Mode en ambos");
  DEBUG_SERIAL.println("7  -> Mover ambos a 90 deg");
  DEBUG_SERIAL.println("8  -> Mover ambos a 180 deg");
  DEBUG_SERIAL.println("9  -> Mover ambos a 270 deg");
  DEBUG_SERIAL.println("a  -> Velocidad +10 RPM en ambos");
  DEBUG_SERIAL.println("b  -> Velocidad -10 RPM en ambos");
  DEBUG_SERIAL.println("c  -> Detener ambos");
  DEBUG_SERIAL.println("d  -> Leer posicion actual");
  DEBUG_SERIAL.println("e  -> Leer velocidad actual");
  DEBUG_SERIAL.println("m  -> Mostrar menu");
  DEBUG_SERIAL.println("==============================================");
  DEBUG_SERIAL.print("Selecciona opcion: ");
}

void ledBoth(bool state) {
  if (motor1_ok) {
    if (state) dxl.ledOn(ID1);
    else dxl.ledOff(ID1);
  }
  if (motor2_ok) {
    if (state) dxl.ledOn(ID2);
    else dxl.ledOff(ID2);
  }
}

void torqueBoth(bool state) {
  if (motor1_ok) {
    if (state) dxl.torqueOn(ID1);
    else dxl.torqueOff(ID1);
  }
  if (motor2_ok) {
    if (state) dxl.torqueOn(ID2);
    else dxl.torqueOff(ID2);
  }
}

void setModeBoth(uint8_t mode) {
  if (motor1_ok) dxl.torqueOff(ID1);
  if (motor2_ok) dxl.torqueOff(ID2);

  bool ok1 = true;
  bool ok2 = true;

  if (motor1_ok) ok1 = dxl.setOperatingMode(ID1, mode);
  if (motor2_ok) ok2 = dxl.setOperatingMode(ID2, mode);

  if (motor1_ok) dxl.torqueOn(ID1);
  if (motor2_ok) dxl.torqueOn(ID2);

  DEBUG_SERIAL.print("Motor 1 modo -> ");
  DEBUG_SERIAL.println(ok1 ? "OK" : "ERROR");
  DEBUG_SERIAL.print("Motor 2 modo -> ");
  DEBUG_SERIAL.println(ok2 ? "OK" : "ERROR");
}

void movePositionBoth(float deg) {
  if (motor1_ok) dxl.setGoalPosition(ID1, deg, UNIT_DEGREE);
  if (motor2_ok) dxl.setGoalPosition(ID2, deg, UNIT_DEGREE);

  DEBUG_SERIAL.print("Moviendo ambos a ");
  DEBUG_SERIAL.print(deg);
  DEBUG_SERIAL.println(" grados...");
}

void moveVelocityBoth(float rpm) {
  if (motor1_ok) dxl.setGoalVelocity(ID1, rpm, UNIT_RPM);
  if (motor2_ok) dxl.setGoalVelocity(ID2, rpm, UNIT_RPM);

  DEBUG_SERIAL.print("Moviendo ambos a ");
  DEBUG_SERIAL.print(rpm);
  DEBUG_SERIAL.println(" RPM...");
}

void stopBoth() {
  if (motor1_ok) dxl.setGoalVelocity(ID1, 0, UNIT_RPM);
  if (motor2_ok) dxl.setGoalVelocity(ID2, 0, UNIT_RPM);
  DEBUG_SERIAL.println("Motores detenidos.");
}

void readPositionBoth() {
  if (motor1_ok) {
    float pos1 = dxl.getPresentPosition(ID1, UNIT_DEGREE);
    DEBUG_SERIAL.print("Motor 1 posicion: ");
    DEBUG_SERIAL.print(pos1);
    DEBUG_SERIAL.println(" deg");
  } else {
    DEBUG_SERIAL.println("Motor 1 no disponible.");
  }

  if (motor2_ok) {
    float pos2 = dxl.getPresentPosition(ID2, UNIT_DEGREE);
    DEBUG_SERIAL.print("Motor 2 posicion: ");
    DEBUG_SERIAL.print(pos2);
    DEBUG_SERIAL.println(" deg");
  } else {
    DEBUG_SERIAL.println("Motor 2 no disponible.");
  }
}

void readVelocityBoth() {
  if (motor1_ok) {
    float vel1 = dxl.getPresentVelocity(ID1, UNIT_RPM);
    DEBUG_SERIAL.print("Motor 1 velocidad: ");
    DEBUG_SERIAL.print(vel1);
    DEBUG_SERIAL.println(" RPM");
  } else {
    DEBUG_SERIAL.println("Motor 1 no disponible.");
  }

  if (motor2_ok) {
    float vel2 = dxl.getPresentVelocity(ID2, UNIT_RPM);
    DEBUG_SERIAL.print("Motor 2 velocidad: ");
    DEBUG_SERIAL.print(vel2);
    DEBUG_SERIAL.println(" RPM");
  } else {
    DEBUG_SERIAL.println("Motor 2 no disponible.");
  }
}

void detectMotors() {
  DEBUG_SERIAL.println("Detectando motores...");

  motor1_ok = dxl.ping(ID1);
  motor2_ok = dxl.ping(ID2);

  DEBUG_SERIAL.print("Motor ID 1: ");
  DEBUG_SERIAL.println(motor1_ok ? "DETECTADO" : "NO RESPONDE");

  DEBUG_SERIAL.print("Motor ID 2: ");
  DEBUG_SERIAL.println(motor2_ok ? "DETECTADO" : "NO RESPONDE");

  if (motor1_ok) {
    DEBUG_SERIAL.print("Model ID1: ");
    DEBUG_SERIAL.println(dxl.getModelNumber(ID1));
  }

  if (motor2_ok) {
    DEBUG_SERIAL.print("Model ID2: ");
    DEBUG_SERIAL.println(dxl.getModelNumber(ID2));
  }
}

// =========================
// SETUP
// =========================
void setup() {
  DEBUG_SERIAL.begin(115200);
  delay(1000);

  DEBUG_SERIAL.println("\n=== XL430-W250-T | 2 MOTORES ===");

  dxl.begin(DXL_BAUD, SERIAL_8N1, DXL_RX_PIN, DXL_TX_PIN);
  dxl.setPortProtocolVersion(2.0);

  detectMotors();

  if (!motor1_ok && !motor2_ok) {
    DEBUG_SERIAL.println("❌ Ningun motor detectado.");
    return;
  }

  torqueBoth(false);
  printMenu();
}

// =========================
// LOOP
// =========================
void loop() {
  if (DEBUG_SERIAL.available()) {
    char cmd = DEBUG_SERIAL.read();

    switch (cmd) {
      case '1':
        ledBoth(true);
        DEBUG_SERIAL.println("\nLED ON en ambos.");
        break;

      case '2':
        ledBoth(false);
        DEBUG_SERIAL.println("\nLED OFF en ambos.");
        break;

      case '3':
        torqueBoth(true);
        DEBUG_SERIAL.println("\nTorque ON en ambos.");
        break;

      case '4':
        torqueBoth(false);
        DEBUG_SERIAL.println("\nTorque OFF en ambos.");
        break;

      case '5':
        DEBUG_SERIAL.println("\nConfigurando Position Mode en ambos...");
        setModeBoth(OP_POSITION);
        break;

      case '6':
        DEBUG_SERIAL.println("\nConfigurando Velocity Mode en ambos...");
        setModeBoth(OP_VELOCITY);
        break;

      case '7':
        movePositionBoth(90.0);
        break;

      case '8':
        movePositionBoth(180.0);
        break;

      case '9':
        movePositionBoth(270.0);
        break;

      case 'a':
        moveVelocityBoth(10.0);
        break;

      case 'b':
        moveVelocityBoth(-10.0);
        break;

      case 'c':
        stopBoth();
        break;

      case 'd':
        readPositionBoth();
        break;

      case 'e':
        readVelocityBoth();
        break;

      case 'm':
        printMenu();
        break;

      case '\n':
      case '\r':
        break;

      default:
        DEBUG_SERIAL.println("\nComando no valido.");
        printMenu();
        break;
    }

    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("Selecciona opcion: ");
  }
}
