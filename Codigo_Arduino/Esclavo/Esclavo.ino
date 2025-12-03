#include <Wire.h>

#define I2C_SLAVE_ADDR 0x08

// ------------------ Clase Motor ------------------
class Motor {
  int pinDir1;
  int pinDir2; 
  int pinPWM;

public:
  Motor(int d1, int d2, int pwm){
    pinDir1 = d1;
    pinDir2 = d2;
    pinPWM = pwm;
  }

  void begin() {
    pinMode(pinDir1, OUTPUT);
    pinMode(pinDir2, OUTPUT);
    pinMode(pinPWM, OUTPUT);
    stop();
  }

  void forward(int speed) {
    digitalWrite(pinDir1, LOW);
    digitalWrite(pinDir2, HIGH);
    analogWrite(pinPWM, speed);
  }

  void backward(int speed) {
    digitalWrite(pinDir1, HIGH);
    digitalWrite(pinDir2, LOW);
    analogWrite(pinPWM, speed);
  }

  void stop() {
    analogWrite(pinPWM, 0);
  }
  
  // Nueva función para manejar el signo automáticamente
  void setSignedSpeed(int16_t speed) {
    if (speed > 0) {
      forward(constrain(speed, 0, 255));
    } else if (speed < 0) {
      backward(constrain(abs(speed), 0, 255));
    } else {
      stop();
    }
  }
};

// ------------------ Definición de Objetos------------------
Motor motorFR(22, 24, 9);
Motor motorFL(26, 28, 10);
Motor motorRR(5, 6, 11);
Motor motorRL(7, 8, 12);

// Estructura para recibir los datos (4 enteros cortos = 8 bytes)
union Packet {
  struct {
    int16_t fl;
    int16_t rl;
    int16_t fr;
    int16_t rr;
  } val;
  byte bytes[8];
};

Packet rxData;
volatile bool newData = false;

void setup() {
  Serial.begin(115200);
  
  // Iniciar motores
  motorFR.begin();
  motorFL.begin();
  motorRR.begin();
  motorRL.begin();

  // Iniciar I2C
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  
  Serial.println("Arduino Omni-Drive I2C Listo");
}

void loop() {
  if (newData) {
    // Debug para verificar qué llega
    // Serial.print("FL: "); Serial.print(rxData.val.fl);
    // Serial.print(" FR: "); Serial.println(rxData.val.fr);
    
    // Aplicar velocidades con signo
    // El orden depende de cómo envíe ROS los datos. 
    // Asumiremos: FL, RL, FR, RR (según tu wheel_velocity_publisher)
    motorFL.setSignedSpeed(rxData.val.fl);
    motorRL.setSignedSpeed(rxData.val.rl);
    motorFR.setSignedSpeed(rxData.val.fr);
    motorRR.setSignedSpeed(rxData.val.rr);
    
    newData = false;
  }
  
  // // Seguridad: Si no llega nada en 1 segundo, detenerse
  // static unsigned long lastUpdate = 0;
  // if(newData) lastUpdate = millis();
  // if(millis() - lastUpdate > 1000) {
  //     motorFL.stop(); motorRL.stop(); motorFR.stop(); motorRR.stop();
  // }
}

void receiveEvent(int bytes) {
  if (bytes >= 8) {
    // Leemos los 8 bytes directamente en nuestra unión
    for(int i=0; i<8; i++) {
      rxData.bytes[i] = Wire.read();
    }
    newData = true;
  } else {
    // Limpiar buffer si llega basura
    while(Wire.available()) Wire.read();
  }
}
