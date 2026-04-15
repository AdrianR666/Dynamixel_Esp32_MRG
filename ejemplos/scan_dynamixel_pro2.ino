#include <Arduino.h>
#include <Dynamixel2Arduino.h>

#define DXL_SERIAL   Serial2
#define DEBUG_SERIAL Serial
#define DXL_DIR_PIN  2

#define DXL_RX_PIN 16
#define DXL_TX_PIN 17

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const uint32_t baudrates[] = {57600, 115200, 1000000, 2000000, 3000000, 4000000, 4500000};
const int numBaudrates = sizeof(baudrates) / sizeof(baudrates[0]);

void scanProtocol(float protocolVersion) {
  dxl.setPortProtocolVersion(protocolVersion);

  DEBUG_SERIAL.println("====================================");
  DEBUG_SERIAL.print("Probando protocolo: ");
  DEBUG_SERIAL.println(protocolVersion, 1);
  DEBUG_SERIAL.println("====================================");

  for (int b = 0; b < numBaudrates; b++) {
    uint32_t baud = baudrates[b];

    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("Baudrate: ");
    DEBUG_SERIAL.println(baud);

    dxl.begin(baud, SERIAL_8N1, DXL_RX_PIN, DXL_TX_PIN);
    delay(200);

    for (uint8_t id = 0; id < 253; id++) {
      bool ok = dxl.ping(id);

      if (ok) {
        DEBUG_SERIAL.println("------------------------------------");
        DEBUG_SERIAL.print("Motor detectado en ID: ");
        DEBUG_SERIAL.println(id);

        uint16_t model = dxl.getModelNumber(id);
        DEBUG_SERIAL.print("Model Number: ");
        DEBUG_SERIAL.println(model);

        int32_t firmware = dxl.readControlTableItem(6, id);
        DEBUG_SERIAL.print("Firmware Version: ");
        DEBUG_SERIAL.println(firmware);

        int32_t baudReg = dxl.readControlTableItem(8, id);
        DEBUG_SERIAL.print("Baud Register: ");
        DEBUG_SERIAL.println(baudReg);

        int32_t opMode = dxl.readControlTableItem(11, id);
        DEBUG_SERIAL.print("Operating Mode: ");
        DEBUG_SERIAL.println(opMode);

        int32_t torque = dxl.readControlTableItem(64, id);
        DEBUG_SERIAL.print("Torque Enable: ");
        DEBUG_SERIAL.println(torque);

        int32_t hwerr = dxl.readControlTableItem(70, id);
        DEBUG_SERIAL.print("Hardware Error Status: ");
        DEBUG_SERIAL.println(hwerr);

        int32_t pos = dxl.readControlTableItem(132, id);
        DEBUG_SERIAL.print("Present Position: ");
        DEBUG_SERIAL.println(pos);

        int32_t vin = dxl.readControlTableItem(144, id);
        DEBUG_SERIAL.print("Input Voltage raw: ");
        DEBUG_SERIAL.println(vin);

        int32_t temp = dxl.readControlTableItem(146, id);
        DEBUG_SERIAL.print("Temperature: ");
        DEBUG_SERIAL.println(temp);

        DEBUG_SERIAL.println("------------------------------------");
      }
    }
  }
}

void setup() {
  DEBUG_SERIAL.begin(115200);
  delay(1000);

  DEBUG_SERIAL.println("\nESP32 - SCAN DYNAMIXEL XL430");
  DEBUG_SERIAL.println("Iniciando...");

  scanProtocol(2.0);   // XL430 normalmente aquí
  scanProtocol(1.0);   // solo por descarte
}

void loop() {
}
