#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <DHT.h>

// =====================================================
// WiFi
// =====================================================

char auth[] = "BLYNK_AUTH_TOKEN";
char ssid[] = "*******";
char pass[] = "********";

// =====================================================
// PIN DEFINITIONS - NODEMCU ESP8266
// =====================================================

// ---------------- PAN / TILT SERVOS ----------------

#define PAN_SERVO_PIN  D3
#define TILT_SERVO_PIN D4

Servo panServo;
Servo tiltServo;

// ---------------- LEFT SIDE MOTORS ----------------

#define ENA D1
#define IN1 D2
#define IN2 D0

// ---------------- RIGHT SIDE MOTORS ----------------

#define IN3 D6
#define IN4 D7
#define ENB D8

// =====================================================
// DHT11
// =====================================================

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// =====================================================
// MQ-2 ANALOG SENSOR
// =====================================================

#define MQ2_AOUT_PIN A0

// ESP8266 ADC = 0 to 1023
#define GAS_THRESHOLD 200

// =====================================================
// BUZZER
// =====================================================

// Buzzer is connected to D8 in this version
// IMPORTANT: D8 is also ENB in the motor section above.
// Therefore use another available pin if your hardware allows.
//
// Recommended: use D8 ONLY for buzzer and move ENB
// to another suitable PWM-capable pin.
//
// If you need all 6 motor pins + 2 servos + DHT + buzzer,
// NodeMCU has very limited GPIO availability.

#define BUZZER_PIN 3

// =====================================================
// JOYSTICK VARIABLES
// =====================================================

int x = 50;
int y = 50;

int Speed = 150;

// =====================================================
// BLYNK TIMER
// =====================================================

BlynkTimer timer;

// =====================================================
// GAS STATE
// =====================================================

bool gasDetected = false;
bool previousGasState = false;

// =====================================================
// GAS SENSOR
// =====================================================

void checkGas()
{
  int gasValue = analogRead(MQ2_AOUT_PIN);

  Serial.print("MQ-2 Gas Value: ");
  Serial.println(gasValue);

  // Send MQ-2 value to Blynk
  Blynk.virtualWrite(V13, gasValue);

  // ===================================================
  // GAS DETECTED
  // ===================================================

  if (gasValue >= GAS_THRESHOLD)
  {
    gasDetected = true;

    Serial.println("!!! GAS DETECTED !!!");

    digitalWrite(BUZZER_PIN, HIGH);

    // V8 = LED
    Blynk.virtualWrite(V8, 1);

    // V9 = status
    Blynk.virtualWrite(V9, "GAS DETECTED");

    // Notification only once when gas is detected
    if (previousGasState == false)
    {
      Blynk.logEvent(
        "gas_alert",
        "Gas Detected! Buzzer ON"
      );
    }
  }

  // ===================================================
  // SAFE
  // ===================================================

  else
  {
    gasDetected = false;

    Serial.println("SAFE - No Gas");

    digitalWrite(BUZZER_PIN, LOW);

    Blynk.virtualWrite(V8, 0);

    Blynk.virtualWrite(V9, "SAFE");
  }

  previousGasState = gasDetected;
}

// =====================================================
// DHT11 TEMPERATURE + HUMIDITY
// =====================================================

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT11!");
    return;
  }

  // V10 = Humidity
  Blynk.virtualWrite(V10, h);

  // V11 = Temperature
  Blynk.virtualWrite(V11, t);

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C");

  Serial.print("    Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // ===================================================
  // SERVO
  // ===================================================

  panServo.attach(PAN_SERVO_PIN);
  tiltServo.attach(TILT_SERVO_PIN);

  panServo.write(90);
  tiltServo.write(90);

  // ===================================================
  // MOTOR
  // ===================================================

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // ===================================================
  // BUZZER
  // ===================================================

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // ===================================================
  // MQ-2
  // ===================================================

  pinMode(MQ2_AOUT_PIN, INPUT);

  analogWriteRange(255);

  // ===================================================
  // DHT11
  // ===================================================

  dht.begin();

  // ===================================================
  // STOP CAR
  // ===================================================

  carStop();

  // ===================================================
  // BLYNK
  // ===================================================

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  // DHT11 every 2 seconds
  timer.setInterval(2000L, sendSensor);

  // MQ-2 every 500 ms
  timer.setInterval(500L, checkGas);

  Serial.println("=================================");
  Serial.println("NodeMCU Robot Started");
  Serial.println("DHT11 Ready");
  Serial.println("MQ-2 Gas Detection Ready");
  Serial.println("=================================");
}

// =====================================================
// BLYNK V0 - X AXIS
// =====================================================

BLYNK_WRITE(V0)
{
  x = param.asInt();

  Serial.print("X = ");
  Serial.println(x);
}

// =====================================================
// BLYNK V1 - Y AXIS
// =====================================================

BLYNK_WRITE(V1)
{
  y = param.asInt();

  Serial.print("Y = ");
  Serial.println(y);
}

// =====================================================
// BLYNK V2 - SPEED
// =====================================================

BLYNK_WRITE(V2)
{
  Speed = param.asInt();

  Speed = constrain(Speed, 0, 255);

  Serial.print("Speed = ");
  Serial.println(Speed);
}

// =====================================================
// BLYNK V3 - PAN SERVO
// =====================================================

BLYNK_WRITE(V3)
{
  int angle = param.asInt();

  angle = constrain(angle, 0, 180);

  panServo.write(angle);

  Serial.print("PAN = ");
  Serial.println(angle);
}

// =====================================================
// BLYNK V4 - TILT SERVO
// =====================================================

BLYNK_WRITE(V4)
{
  int angle = param.asInt();

  angle = constrain(angle, 0, 180);

  tiltServo.write(angle);

  Serial.print("TILT = ");
  Serial.println(angle);
}

// =====================================================
// LEFT MOTOR
// =====================================================

void leftMotor(int speed)
{
  speed = constrain(speed, -255, 255);

  if (speed > 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    analogWrite(ENA, speed);
  }

  else if (speed < 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    analogWrite(ENA, -speed);
  }

  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    analogWrite(ENA, 0);
  }
}

// =====================================================
// RIGHT MOTOR
// =====================================================

void rightMotor(int speed)
{
  speed = constrain(speed, -255, 255);

  if (speed > 0)
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENB, speed);
  }

  else if (speed < 0)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENB, -speed);
  }

  else
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    analogWrite(ENB, 0);
  }
}

// =====================================================
// FORWARD
// =====================================================

void carForward()
{
  leftMotor(Speed);
  rightMotor(Speed);
}

// =====================================================
// FORWARD RIGHT
// =====================================================

void carForwardRight()
{
  leftMotor(Speed / 2);
  rightMotor(Speed);
}

// =====================================================
// RIGHT
// =====================================================

void carRight()
{
  leftMotor(-Speed);
  rightMotor(Speed);
}

// =====================================================
// BACKWARD RIGHT
// =====================================================

void carBackwardRight()
{
  leftMotor(-Speed / 2);
  rightMotor(-Speed);
}

// =====================================================
// BACKWARD
// =====================================================

void carBackward()
{
  leftMotor(-Speed);
  rightMotor(-Speed);
}

// =====================================================
// BACKWARD LEFT
// =====================================================

void carBackwardLeft()
{
  leftMotor(-Speed);
  rightMotor(-Speed / 2);
}

// =====================================================
// LEFT
// =====================================================

void carLeft()
{
  leftMotor(Speed);
  rightMotor(-Speed);
}

// =====================================================
// FORWARD LEFT
// =====================================================

void carForwardLeft()
{
  leftMotor(Speed);
  rightMotor(Speed / 2);
}

// =====================================================
// STOP
// =====================================================

void carStop()
{
  leftMotor(0);
  rightMotor(0);
}

// =====================================================
// 8-DIRECTION CONTROL
// =====================================================

void smartcar()
{
  if (y > 70 && x >= 30 && x <= 70)
  {
    carForward();
  }

  else if (y > 70 && x > 70)
  {
    carForwardRight();
  }

  else if (x > 70 && y >= 30 && y <= 70)
  {
    carRight();
  }

  else if (y < 30 && x > 70)
  {
    carBackwardRight();
  }

  else if (y < 30 && x >= 30 && x <= 70)
  {
    carBackward();
  }

  else if (y < 30 && x < 30)
  {
    carBackwardLeft();
  }

  else if (x < 30 && y >= 30 && y <= 70)
  {
    carLeft();
  }

  else if (y > 70 && x < 30)
  {
    carForwardLeft();
  }

  else
  {
    carStop();
  }
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  Blynk.run();
  timer.run();

  smartcar();
}
