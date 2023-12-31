// Libraries
#include "esp_camera.h"
#include <WiFi.h>
#include <esp_timer.h>
#include <ESP32Servo.h>
//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM

// Custom code
#include "camera_pins.h"
#include "src/command/command.h"
#include "src/timer/timer.h"
#define VERTICAL_SERVO_PIN 2
#define HORIZONTAL_SERVO_PIN 14

// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "";
const char* password = "";

void startCameraServer();
void setupLedFlash(int pin);

Servo verticalServo;
Servo horizontalServo;

// Servo control variables
int verticalAngle = 0;
int horizontalAngle = 0;
int verticalStep = 1;
int horizontalStep = 1;

// NO 'STOP' command hanling, as it should just stop the interrupt
void IRAM_ATTR timer_interrupt(void* arg)
{
  char* direction = get_current_direction();

  Serial.print("Entering interrupt");
  if (direction == NULL) {
    Serial.println("\nDirection is null, no commands still received");
  }
  Serial.print(", target direction: ");
  Serial.println(direction);
  if (!strcmp(direction, SERVO_CMD_DOWN) && verticalAngle < 146) {
    verticalAngle += verticalStep;
    Serial.print("Moving down, new angle: ");
    Serial.println(verticalAngle);
    verticalServo.write(verticalAngle);
  }

  if (!strcmp(direction, SERVO_CMD_UP) && verticalAngle > 0) {
    verticalAngle -= verticalStep;
    Serial.print("Moving up, new angle: ");
    Serial.println(verticalAngle);
    verticalServo.write(verticalAngle);
  }

  if (!strcmp(direction, SERVO_CMD_LEFT) && horizontalAngle < 180) {
    horizontalAngle += horizontalStep;
    Serial.print("Moving left, new angle: ");
    Serial.println(horizontalAngle);
    horizontalServo.write(horizontalAngle);
  }

  if (!strcmp(direction, SERVO_CMD_RIGHT) && horizontalAngle > 0) {
    horizontalAngle -= horizontalStep;
    Serial.print("Moving right, new angle: ");
    Serial.println(horizontalAngle);
    horizontalServo.write(horizontalAngle);
  }
}

const esp_timer_create_args_t timer_args = {
    .callback = &timer_interrupt,
    .name = "servo_timer",
};

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Servos setup
  verticalServo.attach(VERTICAL_SERVO_PIN);
  verticalServo.write(verticalAngle);
  horizontalServo.attach(HORIZONTAL_SERVO_PIN);
  horizontalServo.write(horizontalAngle);

  setupTimer(timer_args);

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
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG){
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  // Do nothing. Everything is done in another task by the web server
  delay(10000);
}
