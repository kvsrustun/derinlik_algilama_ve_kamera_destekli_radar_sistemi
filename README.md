# 📡 Derinlik Algılama ve Kamera Destekli Radar Sistemi

[![Donanım](https://img.shields.io/badge/Donanım-Deneyap%20Kart%20%28ESP32%29-007acc?style=for-the-badge&logo=espressif&logoColor=white)](https://deneyapkart.org)
[![Sensör](https://img.shields.io/badge/Sensör-VL53L0X%20ToF%20Lazer-059669?style=for-the-badge)](https://deneyapkart.org)
[![Processing](https://img.shields.io/badge/Görselleştirme-Processing%20IDE-2b5b84?style=for-the-badge&logo=processingfoundation&logoColor=white)](https://processing.org)
[![Kamera](https://img.shields.io/badge/Görüntüleme-OV2640%20Kamera-d97706?style=for-the-badge)](https://deneyapkart.org)
[![Lisans](https://img.shields.io/badge/Lisans-MIT-brightgreen?style=for-the-badge)](#-lisans)

> **Deneyap Kart (ESP32)**, **VL53L0X ToF Lazer Mesafe Sensörü**, **Servo Motor** ve **OV2640 Kamera** kullanılarak geliştirilmiş; 180° ortam taraması yapan, 40 cm yakınlığa giren nesneleri otomatik fotoğraflayan ve askeri radar ekranı gibi **Processing** üzerinde canlı görselleştiren otonom güvenlik ve radar sistemi.

---

## 📑 İçindekiler

1. [Proje Özeti ve Temel Amaç](#-proje-özeti-ve-temel-amaç)
2. [Kullanılan Donanımlar ve Modüller](#-kullanılan-donanımlar-ve-modüller)
3. [Devre Bağlantı Şeması ve Pin Haritası](#-devre-bağlantı-şeması-ve-pin-haritası)
4. [Sistem Mimarisi ve Çalışma Mantığı](#-sistem-mimarisi-ve-çalışma-mantığı)
5. [Web Arayüzü Özellikleri](#-web-arayüzü-özellikleri)
6. [Processing ile 2D Radar Görselleştirme](#-processing-ile-2d-radar-görselleştirme)
7. [Adım Adım Kurulum ve Çalıştırma Rehberi](#-adım-adım-kurulum-ve-çalıştırma-rehberi)
8. [Karşılaşılabilecek Sorunlar ve Çözümleri](#-karşılaşılabilecek-sorunlar-ve-çözümleri)
9. [Geliştirici](#-geliştirici)
10. [Lisans](#-lisans)

---

## 🎯 Proje Özeti ve Temel Amaç

Bu projenin temel hedefi, fiziksel çevreyi sürekli tarayan ve şüpheli yakınlaşmaları algılayıp belgeleyen otonom bir güvenlik radarı oluşturmaktır:

- **180° Mekanik Tarama:** Servo motor, ToF derinlik sensörünü 0° ile 180° arasında sürekli olarak sağa ve sola çevirerek tarama yapar.
- **Milimetrik Lazer Ölçüm:** VL53L0X ToF (Time-of-Flight) sensörü, ışık hızıyla mesafe ölçerek ortamdaki nesnelerin uzaklığını hesaplar.
- **Otonom Fotoğraf Tetikleme:** Algılanan mesafe **40 cm'den yakın** olduğu anda OV2640 kamera otomatik olarak fotoğraf çeker ve belleğe kaydeder.
- **Canlı Web Arayüzü:** ESP32 üzerinde koşan dahili web sunucusu üzerinden son 3 fotoğraf ve anlık açı/mesafe verisi canlı izlenebilir.
- **2D Radar Ekranı:** Seri porttan aktarılan koordinat verileri Processing uygulamasıyla gerçek zamanlı bir radar dairesine dönüştürülür.

---

## 🧰 Kullanılan Donanımlar ve Modüller

| Bileşen | Model / Tip | Görevi |
| :--- | :--- | :--- |
| **Ana İşlemci** | Deneyap Kart (ESP32 Tabanlı) | Sensör okuma, servo yönetimi ve Wi-Fi web sunucusu |
| **Mesafe Sensörü** | Deneyap Derinlik Ölçer (VL53L0X ToF) | I2C protokolü üzerinden lazer mesafe ölçümü |
| **Mekanik Eyleyici**| SG90 Servo Motor | Sensörü 0°–180° arasında açılı olarak döndürme |
| **Görüntüleme** | OV2640 Kamera Modülü | 40 cm altına giren hedefleri fotoğraflama |
| **Haberleşme** | Dahili Wi-Fi & USB-UART Seri Port | Web arayüzü ve Processing veri aktarımı |

---

## 🔌 Devre Bağlantı Şeması ve Pin Haritası

<p align="center">
  <img src="radar_semasi.png" alt="Radar Devre Şeması" width="85%" style="border-radius: 12px; box-shadow: 0 4px 20px rgba(0,0,0,0.15);" />
</p>

> 💡 **Bağlantı Notu:** Servo motor ani dönüş anında güç dalgalanması oluşturabileceğinden harici 5V besleme veya stabil bir USB güç kaynağı kullanılması önerilir. Ortak GND bağlantısı unutulmamalıdır.

```
+-----------------------------+              +-----------------------------+
|       Deneyap Kart 1A       |              |        Modül / Sensör       |
|                             |              |                             |
|  I2C SDA & SCL (0x29)       |<------------>| VL53L0X ToF Derinlik Ölçer  |
|  D9 (PWM Çıkışı)            |------------->| SG90 Servo Motor (Sinyal)   |
|  CAM Veri Yolu (D2 - D9)    |<------------>| OV2640 Kamera Modülü        |
|  CAMSC / CAMSD              |<------------>| Kamera I2C Kontrol Hatları  |
|  3.3V / 5V                  |------------->| Sensör ve Modül Beslemeleri |
|  GND                        |<------------>| Ortak GND Toprak Hattı      |
+-----------------------------+              +-----------------------------+
```

---

## 📡 Sistem Mimarisi ve Çalışma Mantığı

```
[ Servo Motor (0°-180°) ] ──► [ ToF Mesafe Sensörü ]
                                      │
                                      ▼ (Mesafe < 40 cm ?)
                         ┌────────────┴────────────┐
                         │ EVET                    │ HAYIR
                         ▼                         ▼
               [ Kamera Fotoğraf Çeker ]   [ Taramaya Devam Et ]
                         │                         │
                         ▼                         ▼
         [ Web Sunucu (/lastImages) ]     [ Seri Port (Açı,Mesafe) ]
                         │                         │
                         ▼                         ▼
                  [ Web Tarayıcısı ]        [ Processing Radar GUI ]
```

---

## 🌐 Web Arayüzü Özellikleri

ESP32 üzerinde koşan hafif ve asenkron web sunucusu:
- **URL:** `http://<ESP32_IP_ADRESI>/`
- **📸 Son 3 Fotoğraf:** 40 cm altına inen hedeflerin yakalanan son 3 karesi 3 saniyede bir otomatik yenilenerek sunulur (`/lastImages`).
- **📏 Canlı Radar Verisi:** Anlık açı ve mesafe değerleri her 0.5 saniyede bir güncellenir (`/radar`).

---

## 🖥️ Processing ile 2D Radar Görselleştirme

Bilgisayarda çalışan **Processing (`radar.pde`)** uygulaması seri port üzerinden ESP32'den gelen verileri dinler:

1. Yeşil dairesel radar ızgarası 0°'den 180°'ye doğru bir radar ibresi gibi taranır.
2. Mesafe algılandığında tam o koordinata **kırmızı nokta/çizgi** çizilir.
3. Ekranın sağ altında anlık **Açı (Angle)** ve **Mesafe (Distance cm)** dijital olarak yansıtılır.

```
             [90°]
         /     |     \
     /         |         \
 [0°] ------- [o] ------- [180°]
          (Servo + ToF)
```

---

## 🚀 Adım Adım Kurulum ve Çalıştırma Rehberi

### 1. Arduino / Deneyap IDE Hazırlığı
Arduino IDE Kütüphane Yöneticisi'nden aşağıdaki kütüphaneleri kurun:
- `ESPAsyncWebServer`
- `ArduinoJson`
- `base64`
- `Deneyap_Servo`
- `Deneyap_DerinlikOlcer`

### 2. Wi-Fi Bilgilerini Güncelleme
`radar_kamera.ino` dosyasını açıp kendi Wi-Fi ağ bilgilerinizi girin:
```cpp
const char* ssid = "WiFi_Adiniz";
const char* password = "WiFi_Sifreniz";
```

### 3. Kodu Yükleme
Kodu Deneyap Kart'a yükleyin. Seri Monitör'ü (`115200 baud`) açarak atanan IP adresini not edin:
```
✅ WiFi bağlı. IP: 192.168.1.100
```
Tarayıcınızda bu IP adresini açarak web kokpitine ulaşabilirsiniz.

### 4. Processing Arayüzünü Başlatma
1. [Processing IDE](https://processing.org/download) indirin ve açın.
2. Depodaki `radar/radar.pde` dosyasını açın.
3. Koddaki `COM75` portunu kendi bilgisayarınızdaki Deneyap Kart portu ile değiştirin (örneğin `COM3`).
4. Çalıştır butonuna (▶) basın.

---

## ⚠️ Karşılaşılabilecek Sorunlar ve Çözümleri

> 🔴 **Önemli Seri Port Kuralı:** Bilgisayardaki bir seri port (COM) aynı anda sadece tek bir program tarafından okunabilir.  
> Arduino IDE Seri Monitörü açıkken Processing'i başlatırsanız port meşgul hatası alırsınız.  
> **Çözüm:** Kodu karta yükledikten sonra Arduino Seri Monitörünü kapatın, ardından Processing'i çalıştırın!

---

## 👩‍💻 Geliştirici

**Kevser ÜSTÜN**  
🎓 Giresun Üniversitesi — Bilgisayar Mühendisliği  
🔗 [GitHub Profilim](https://github.com/kvsrustun)

---

## 📄 Lisans

Bu proje [MIT Lisansı](LICENSE) kapsamında açık kaynak olarak paylaşılmıştır.
