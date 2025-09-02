#include <WiFi.h>
#include <esp_camera.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <Deneyap_DerinlikOlcer.h>
#include <Deneyap_Servo.h>

TofRangeFinder depthSensor;
Servo myServo;

const char* ssid = "EYUP DENEYAP";
const char* password = "DENEYAP-2025";

AsyncWebServer server(80);

String lastImages[3];  // Son 3 fotoğraf

// ========== Kamera Ayarları ==========
void cameraInit() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = CAMD2;
  config.pin_d1 = CAMD3;
  config.pin_d2 = CAMD4;
  config.pin_d3 = CAMD5;
  config.pin_d4 = CAMD6;
  config.pin_d5 = CAMD7;
  config.pin_d6 = CAMD8;
  config.pin_d7 = CAMD9;
  config.pin_xclk = CAMXC;
  config.pin_pclk = CAMPC;
  config.pin_vsync = CAMV;
  config.pin_href = CAMH;
  config.pin_sscb_sda = CAMSD;
  config.pin_sscb_scl = CAMSC;
  config.pin_pwdn = -1;
  config.pin_reset = -1;
  config.xclk_freq_hz = 15000000;
  config.frame_size = FRAMESIZE_QQVGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 15;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Kamera başlatılamadı! Hata: 0x%x\n", err);
  } else {
    Serial.println("📷 Kamera başarıyla başlatıldı.");
  }
}

// ========== Web Arayüzü ==========
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Kamera & Radar</title>
</head>
<body>
  <h2>📸 Son 3 Fotoğraf</h2>
  <div id="lastImages" style="display:flex; gap: 10px;">
    <img id="img0" width="100" height="75" />
    <img id="img1" width="100" height="75" />
    <img id="img2" width="100" height="75" />
  </div>
  <h2>📏 Mesafe ve Açı</h2>
  <pre id="radarData"></pre>
  <script>
    function updateImages() {
      fetch("/lastImages")
        .then(res => res.json())
        .then(data => {
          for(let i = 0; i < 3; i++) {
            document.getElementById("img" + i).src = data.images[i] || "";
          }
        });
    }

    function updateRadar() {
      fetch("/radar")
        .then(res => res.text())
        .then(data => {
          document.getElementById("radarData").textContent = data;
        });
    }

    setInterval(updateImages, 3000);
    setInterval(updateRadar, 500); // yarım saniyede bir radar güncelle
    updateImages();
    updateRadar();
  </script>
</body>
</html>
)rawliteral";

// ========== Fotoğraf Çek ve Sakla ==========
void captureAndStoreImage() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("❌ Kamera fotoğrafı alınamadı!");
    return;
  }

  String base64Image = "data:image/jpeg;base64," + base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  lastImages[0] = lastImages[1];
  lastImages[1] = lastImages[2];
  lastImages[2] = base64Image;

  Serial.println("✅ Yeni fotoğraf eklendi.");
}

String radarBuffer = "";

// ========== Setup ==========
void setup() {
  Serial.begin(115200);
  depthSensor.begin(0x29);
  myServo.attach(D9);
  cameraInit();

  WiFi.begin(ssid, password);
  Serial.print("WiFi bağlanıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi bağlı. IP: " + WiFi.localIP().toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/lastImages", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument doc(2000);
    JsonArray arr = doc.createNestedArray("images");
    for (int i = 0; i < 3; i++) arr.add(lastImages[i]);
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/radar", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", radarBuffer);
  });

  server.begin();
  Serial.println("🌐 Web sunucusu başladı.");
}

// ========== Loop ==========
void loop() {
  radarBuffer = "";

  // Servo 0->180
  for(int angle=0; angle<=180; angle+=5){
    myServo.write(angle);
    delay(30);
    float distance = depthSensor.ReadDistance();
    radarBuffer += "Açı: " + String(angle) + "°, Mesafe: " + String(distance) + " cm\n";

    if(distance < 40 && distance > 0){
      captureAndStoreImage();
      delay(200); // üst üste çekimi önler
    }
  }

  // Servo 180->0
  for(int angle=180; angle>=0; angle-=5){
    myServo.write(angle);
    delay(30);
    float distance = depthSensor.ReadDistance();
    radarBuffer += "Açı: " + String(angle) + "°, Mesafe: " + String(distance) + " cm\n";

    if(distance < 40 && distance > 0){
      captureAndStoreImage();
      delay(200);
    }
  }
}
