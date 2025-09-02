# 📡 Derinlik Algılama ve Kamera Destekli Radar Sistemi

Bu proje, ESP32 tabanlı Deneyap Kart kullanılarak oluşturulmuş bir radar sistemidir. Derinlik sensörü ile mesafe ölçülür, servo motor ile tarama yapılır ve yakınlık algılandığında fotoğraf çekilerek web arayüzüne gönderilir. Ayrıca, radar verileri gerçek zamanlı olarak **Processing** uygulamasında görsel şekilde gösterilir.

---

## 🎯 Amaç

- Ortamı 180° tarayarak mesafe verisi toplamak
- 40 cm'den yakın nesne algılandığında otomatik olarak fotoğraf çekmek
- Son 3 fotoğrafı ve radar verilerini web arayüzü üzerinden canlı olarak göstermek
- Processing arayüzü ile radar çıktısını görsel olarak izlemek

---

## 🧰 Kullanılan Donanımlar

- ✅ Deneyap Kart (ESP32 tabanlı)
- ✅ Deneyap Derinlik Ölçer (ToF sensörü - I2C)
- ✅ Deneyap Servo Motor
- ✅ ESP32 Kamera Modülü (OV2640)
- ✅ Wi-Fi bağlantısı
- ✅ Bilgisayarda çalışan **Processing** uygulaması

---

## 🔌 Bağlantı Şeması

| Modül             | Deneyap Pin      |
|------------------|-------------------|
| Derinlik Ölçer   | I2C               |
| Servo Motor      | PWM (D9)          |
| Kamera Modülü    | CAMD2 - CAMSC     |

---

## ⚙️ Arduino (ESP32) Kodunun İşleyişi

- Servo motor 0° ile 180° arasında ileri ve geri döner.
- Her açıda ToF sensörü ile mesafe ölçülür.
- Ölçülen değer `/radar` endpoint'i üzerinden web arayüzüne aktarılır.
- Eğer mesafe 40 cm'den az ise, otomatik olarak fotoğraf çekilir ve son 3 fotoğraf `/lastImages` üzerinden gösterilir.

---
⚙️ Kurulum ve Kullanım

1. Arduino IDE’yi açın.
2. Gerekli kütüphaneleri yükleyin:
   - `ESPAsyncWebServer`
   - `ArduinoJson`
   - `esp_camera`
   - `base64`
   - `Deneyap_Servo`
   - `Deneyap_DerinlikOlcer`
3. Wi-Fi bilgilerinizi aşağıdaki kısma yazın:

const char* ssid = "WiFi_ADI";
const char* password = "WiFi_SIFRE"; 

4. Kodu ESP32'ye yükleyin.

5. Seri monitörden IP adresini alın ve tarayıcınıza yazın.


Web arayüzü üzerinden fotoğrafları ve radar verilerini görebilirsiniz.
## 🌐 Web Arayüzü Özellikleri

- 📸 **Son 3 Fotoğraf:** 3 saniyede bir yenilenir.
- 📏 **Radar Verileri:** 0.5 saniyede bir güncellenir.
- Tamamen HTML+JavaScript ile yazılmıştır.
- Arayüze erişim: WiFi bağlantısı üzerinden tarayıcıda IP'yi açarak erişilir.

---

## 🖥️ Processing ile Radar Görselleştirme

ESP32 tarafından seri port (örneğin `COM75`) üzerinden gönderilen açı ve mesafe bilgileri, **Processing** ile görselleştirilir. 

### ✅ İşlevler:
- Radar çizimi (daireler, açı çizgileri)
- Anlık açı çizgisi
- Cisim algılandığında kırmızı çizgi
- Sağ alt köşede dijital açı ve mesafe verisi

### ⚙️ Processing Kurulumu:

1. [Processing](https://processing.org/download) IDE’yi indirip kur.
2. Aşağıdaki kodu yeni bir sketch’e yapıştır.
3. `COM75` kısmını bilgisayarındaki seri port ismine göre güncelle.
4. Arduino devresi açıkken çalıştır.

⚠️ Not:
Aynı anda hem Arduino hem de Processing seri portu kullanmaya çalışırsa Processing bağlantı hatası verir veya boş veri alır.

✅ Çözüm:
Arduino kodunu yükledikten sonra Seri Monitörü kapatın.
Ardından Processing kodunu çalıştırın.
Aynı anda yalnızca bir uygulama seri portu kullanabilir.

👤 Geliştirici

Kevser ÜSTÜN
🎓 Giresun Üniversitesi – Bilgisayar Mühendisliği