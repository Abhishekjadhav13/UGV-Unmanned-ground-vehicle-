#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// =====================================================
// CAMERA BOARD CONFIGURATION
// =====================================================

#include "board_config.h"



// =====================================================
// WIFI
// =====================================================
//char auth[] = "UcwvgP_ORAB4g6RBUC8eGV4uc5D4IRJ5";
const char *ssid = "*******";
const char *pass = "********";

#define TRIG_PIN 13
#define ECHO_PIN 14

// Obstacle detection limit
#define OBSTACLE_DISTANCE 30

BlynkTimer timer;

// =====================================================
// FUNCTION DECLARATION
// =====================================================

void startCameraServer();

// =====================================================
// BLYNK V12
// ONE SWITCH CONTROLS:
// 1. CAMERA FLASH
// 2. EXTERNAL LED
// =====================================================
void checkDistance()
{
// Trigger ultrasonic sensor
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);

digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);

digitalWrite(TRIG_PIN, LOW);

// Read echo
long duration = pulseIn(ECHO_PIN, HIGH, 30000);

// No valid reading
if (duration == 0)
{
Blynk.virtualWrite(V19, "No Obstacles");
Blynk.virtualWrite(V18, 0);
Blynk.virtualWrite(V20, 0);

Serial.println("No reading"); 
return; 

}

// Calculate distance
float distance = duration * 0.0343 / 2.0;

// Send distance to Gauge V19

// Obstacle detection
if (distance <= OBSTACLE_DISTANCE)
{
// V18 Label
Blynk.virtualWrite(V18, distance);

// V20 LED HIGH 
Blynk.virtualWrite(V20, 1); 
Blynk.virtualWrite(V19, "Object"); 
Serial.print("Obstacle Detected: "); 
Serial.print(distance); 
Serial.println(" cm"); 

}
else
{
// V18 Label
Blynk.virtualWrite(V18, distance);
Blynk.virtualWrite(V19, "No Object");
// V20 LED LOW
Blynk.virtualWrite(V20, 0);

Serial.print("No Obstacles: "); 
Serial.print(distance); 
Serial.println(" cm"); 

}
}

BLYNK_WRITE(V12)
{
int state = param.asInt();

// -------------------------------
// Camera Flash
// -------------------------------
#if defined(LED_GPIO_NUM)

digitalWrite(LED_GPIO_NUM, state);

#endif

Serial.print("V12 = ");

if (state)
{
Serial.println("ON");
}
else
{
Serial.println("OFF");
}
}



// =====================================================
// SETUP
// =====================================================

void setup()
{
// ===================================================
// SERIAL
// ===================================================

Serial.begin(115200);

Serial.setDebugOutput(true);

Serial.println();
Serial.println("================================");
Serial.println("ESP32-CAM + BLYNK");
Serial.println("================================");

//ultrasonic
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

digitalWrite(TRIG_PIN, LOW);

Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

// Check distance every 500 ms
timer.setInterval(500L, checkDistance);


// ===================================================
// CAMERA FLASH
// ===================================================

#if defined(LED_GPIO_NUM)

pinMode(LED_GPIO_NUM, OUTPUT);

digitalWrite(LED_GPIO_NUM, LOW);

Serial.print("Camera Flash GPIO: ");
Serial.println(LED_GPIO_NUM);

#else

Serial.println("Camera Flash GPIO not defined");

#endif

// ===================================================
// CAMERA CONFIGURATION
// ===================================================

camera_config_t config;

config.ledc_channel = LEDC_CHANNEL_0;
config.ledc_timer = LEDC_TIMER_0;

config.pin_d0 = Y2_GPIO_NUM;
config.pin_d1 = Y3_GPIO_NUM;
config.pin_d2 = Y4_GPIO_NUM;
config.pin_d3 = Y5_GPIO_NUM;
config.pin_d4 = Y6_GPIO_NUM;
config.pin_d5 = Y7_GPIO_NUM;
config.pin_d6 = Y8_GPIO_NUM;
config.pin_d7 = Y9_GPIO_NUM;

config.pin_xclk = XCLK_GPIO_NUM;
config.pin_pclk = PCLK_GPIO_NUM;
config.pin_vsync = VSYNC_GPIO_NUM;
config.pin_href = HREF_GPIO_NUM;

config.pin_sccb_sda = SIOD_GPIO_NUM;
config.pin_sccb_scl = SIOC_GPIO_NUM;

config.pin_pwdn = PWDN_GPIO_NUM;
config.pin_reset = RESET_GPIO_NUM;

config.xclk_freq_hz = 20000000;

// ===================================================
// CAMERA SETTINGS
// ===================================================

config.frame_size = FRAMESIZE_QQVGA;

// JPEG is required for web streaming
config.pixel_format = PIXFORMAT_JPEG;
config.frame_size = FRAMESIZE_QQVGA;
config.jpeg_quality = 15;
config.fb_count = 1;
// ===================================================
// PSRAM
// ===================================================

if (psramFound())
{
  Serial.println("PSRAM FOUND");

  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 15;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
}
else
{
  Serial.println("PSRAM NOT FOUND");

  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 18;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_DRAM;
}

{
config.frame_size = FRAMESIZE_240X240;

#if CONFIG_IDF_TARGET_ESP32S3

config.fb_count = 2; 

#endif
}

// ===================================================
// CAMERA INIT
// ===================================================

Serial.println("Initializing camera...");

esp_err_t err = esp_camera_init(&config);

if (err != ESP_OK)
{
Serial.printf(
"Camera init failed with error 0x%x\n",
err
);

return; 

}

Serial.println("Camera initialized successfully");

// ===================================================
// CAMERA SENSOR
// ===================================================

sensor_t *s = esp_camera_sensor_get();

// ===================================================
// OV3660 SETTINGS
// ===================================================

if (s->id.PID == OV3660_PID)
{
s->set_vflip(s, 1);

s->set_brightness(s, 1); 

s->set_saturation(s, -2); 

}

// ===================================================
// LOWER FRAME SIZE
// IMPROVES STREAMING SPEED
// ===================================================

if (config.pixel_format == PIXFORMAT_JPEG)
{
s->set_framesize(
s,
FRAMESIZE_QVGA
);
}

// ===================================================
// M5STACK CAMERA SETTINGS
// ===================================================

#if defined(CAMERA_MODEL_M5STACK_WIDE) || \
defined(CAMERA_MODEL_M5STACK_ESP32CAM)

s->set_vflip(s, 1);

s->set_hmirror(s, 1);

#endif

// ===================================================
// ESP32-S3 EYE
// ===================================================

#if defined(CAMERA_MODEL_ESP32S3_EYE)

s->set_vflip(s, 1);

#endif

// ===================================================
// WIFI
// ===================================================

Serial.println();
Serial.println("Connecting to WiFi...");

WiFi.begin(
ssid,
pass
);

// Disable WiFi sleep
// Helps camera streaming responsiveness
WiFi.setSleep(false);

while (WiFi.status() != WL_CONNECTED)
{
delay(500);

Serial.print("."); 

}

Serial.println();

Serial.println("WiFi connected");

Serial.print("IP Address: ");

Serial.println(
WiFi.localIP()
);

// ===================================================
// BLYNK
// ===================================================

Serial.println();

Serial.println("Connecting to Blynk...");

Blynk.config(
BLYNK_AUTH_TOKEN
);

// Try connecting to Blynk
while (!Blynk.connect())
{
Serial.print(".");

delay(500); 

}

Serial.println();

Serial.println("Blynk connected");

// ===================================================
// START CAMERA SERVER
// ===================================================

startCameraServer();

// ===================================================
// CAMERA READY
// ===================================================

Serial.println();

Serial.println("==============================");

Serial.print("Camera Ready! Open: http://");

Serial.println(
WiFi.localIP()
);

Serial.println("==============================");

Serial.println();
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
// Keep Blynk responsive
Blynk.run();

timer.run();
// No long delay here
}
