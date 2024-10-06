// Example sketch to control a stepper motor with A4988 stepper motor driver 
// and Arduino without a library. 
// More info: https://www.makerguides.com

// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 1600

const int tempPin = A0; // Pin donde se conecta el sensor de temperatura
const float referenceResistance = 5000.0; // Resistencia del sensor en 25°C (5k ohm)
const float betaCoefficient = 3950.0; // Coeficiente beta para el termistor
const float tempOffset = 273.15; // Offset para convertir a Kelvin
const float r0 = referenceResistance; // Resistencia a 25°C
const float t0 = 298.15; // 25°C en Kelvin

enum State { CLOSED, OPEN }; // Posibles estados de la compuerta
State currentState = CLOSED;  // Estado inicial de la compuerta

void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600); // Iniciar comunicación serial
}

void loop() {
  float temperature = readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C - ");

  if (temperature < 10.0 && currentState == OPEN) {
    // Cierra la compuerta
    Serial.println("Estado: Cerrando compuerta.");
    digitalWrite(dirPin, HIGH); // Cambiar dirección a cerrar
    moveStepper();
    currentState = CLOSED; // Actualiza el estado
    Serial.println("Estado: Comp puerta cerrada.");
  } else if (temperature >= 10.0 && currentState == CLOSED) {
    // Abre la compuerta
    Serial.println("Estado: Abriendo compuerta.");
    digitalWrite(dirPin, LOW); // Cambiar dirección a abrir
    moveStepper();
    currentState = OPEN; // Actualiza el estado
    Serial.println("Estado: Comp puerta abierta.");
  }

  delay(5000); // Espera 5 segundos antes de volver a leer
}

void moveStepper() {
  // Spin the stepper motor 1 revolución
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}

float readTemperature() {
  int sensorValue = analogRead(tempPin);
  float resistance = (1023.0 / sensorValue - 1) * r0;
  float temperatureK = (1.0 / (log(resistance / r0) / betaCoefficient + 1 / t0));
  float temperatureC = temperatureK - tempOffset; // Convertir a Celsius
  return temperatureC;
}
