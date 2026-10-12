# 📡 Derinlik Algılama ve Kamera Destekli Radar Sistemi

<p align="center">
  <a href="#donanimlar"><img src="https://img.shields.io/badge/Donan%C4%B1m-Deneyap%20Kart%201A-24292e?style=flat-square&logo=espressif" alt="Donanım" /></a>
  <a href="#donanimlar"><img src="https://img.shields.io/badge/Sens%C3%B6r-VL53L0X%20ToF%20Lazer-24292e?style=flat-square" alt="Sensör" /></a>
  <a href="#processing-radar"><img src="https://img.shields.io/badge/G%C3%B6rselle%C5%9Ftirme-Processing%20IDE-24292e?style=flat-square&logo=processingfoundation" alt="Processing" /></a>
  <a href="#donanimlar"><img src="https://img.shields.io/badge/G%C3%B6r%C3%BCnt%C3%BCleme-OV2640%20Kamera-24292e?style=flat-square" alt="Kamera" /></a>
  <a href="#lisans"><img src="https://img.shields.io/badge/Lisans-MIT-24292e?style=flat-square" alt="Lisans" /></a>
</p>

> **Deneyap Kart (ESP32)**, **VL53L0X ToF Lazer Mesafe Sensörü**, **Servo Motor** ve **OV2640 Kamera** kullanılarak geliştirilmiş; 180° ortam taraması yapan, 40 cm yakınlığa giren nesneleri otomatik fotoğraflayan ve askeri radar ekranı gibi **Processing** üzerinde canlı görselleştiren otonom güvenlik ve radar sistemi.

---

<a id="icindekiler"></a>
## 📑 İçindekiler

1. [Proje Özeti ve Temel Amaç](#proje-ozeti)
2. [Kullanılan Donanımlar ve Modüller](#donanimlar)
3. [Prototip Donanım ve Canlı Çalışma Demosu](#prototip-ve-demo)
4. [Devre Bağlantı Şeması ve Pin Haritası](#devre-baglantisi)
5. [Sistem Mimarisi ve Çalışma Mantığı](#sistem-mimarisi)
6. [Web Arayüzü Özellikleri](#web-arayuzu)
7. [Processing ile 2D Radar Görselleştirme](#processing-radar)
8. [Adım Adım Kurulum ve Çalıştırma Rehberi](#kurulum-ve-calistirma)
9. [Karşılaşılabilecek Sorunlar ve Çözümleri](#sorunlar-ve-cozumleri)
10. [Geliştirici](#gelistirici)
11. [Lisans](#lisans)

---

<a id="proje-ozeti"></a>
## 🎯 1. Proje Özeti ve Temel Amaç

Bu projenin temel hedefi, fiziksel çevreyi sürekli tarayan ve şüpheli yakınlaşmaları algılayıp belgeleyen otonom bir güvenlik radarı oluşturmaktır:

- **180° Mekanik Tarama:** SG90 servo motor, ToF derinlik sensörünü 0° ile 180° arasında sürekli olarak sağa ve sola çevirerek ortam taraması gerçekleştirir.
- **Milimetrik Lazer Ölçüm:** VL53L0X ToF (Time-of-Flight) sensörü, ışık hızıyla mesafe ölçümü yaparak ortamdaki nesnelerin uzaklığını hesaplar.
- **Otonom Fotoğraf Tetikleme:** Algılanan mesafe **40 cm'den yakın** olduğu anda OV2640 kamera otomatik olarak fotoğraf çeker ve belleğe kaydeder.
- **Canlı Web Arayüzü:** ESP32 üzerinde koşan dahili web sunucusu üzerinden son 3 fotoğraf ve anlık açı/mesafe verisi canlı izlenebilir.
- **2D Radar Ekranı:** Seri porttan aktarılan koordinat verileri Processing uygulamasıyla gerçek zamanlı bir radar dairesine dönüştürülür.

---

<a id="donanimlar"></a>
## 🧰 2. Kullanılan Donanımlar ve Modüller

| Bileşen | Model / Tip | Görevi |
| :--- | :--- | :--- |
| **Ana İşlemci** | Deneyap Kart (ESP32 Tabanlı) | Sensör okuma, servo yönetimi ve Wi-Fi web sunucusu |
| **Mesafe Sensörü** | Deneyap Derinlik Ölçer (VL53L0X ToF) | I2C protokolü üzerinden lazer mesafe ölçümü |
| **Mekanik Eyleyici**| SG90 Servo Motor | Sensörü 0°–180° arasında açılı olarak döndürme |
| **Görüntüleme** | OV2640 Kamera Modülü | 40 cm altına giren hedefleri fotoğraflama |
| **Haberleşme** | Dahili Wi-Fi & USB-UART Seri Port | Web arayüzü ve Processing veri aktarımı |

---

<a id="prototip-ve-demo"></a>
## 📸 3. Prototip Donanım ve Canlı Çalışma Demosu

> 💡 **Not:** Devre şeması yerine projenin bizzat kurulan fiziksel prototipi ve çalışır haldeki canlı radar takip ekranı aşağıda yer almaktadır.

<a id="prototip-ve-demo"></a>
## 📸 3. Prototip Donanım ve Canlı Çalışma Demosu

> 💡 **Not:** Devre şeması yerine projenin bizzat kurulan fiziksel prototip fotoğrafı ve çalışır haldeki canlı radar tarama videosu aşağıda yan yana sunulmuştur.

<div align="center">
  <table>
    <tr>
      <td width="50%" align="center" valign="top">
        <b>🛠️ Fiziksel Donanım Prototipi</b><br/><br/>
        <img src="https://github.com/user-attachments/assets/66aeed7e-55ad-483e-ae33-9e1abe4b4fcd" alt="Radar ve Kamera Donanım Prototipi" width="95%" style="border-radius: 8px; box-shadow: 0 4px 15px rgba(0,0,0,0.15);" />
        <br/><br/><em>Deneyap Kart, ToF Lazer Sensörü, Servo ve Kamera Modülü</em>
      </td>
      <td width="50%" align="center" valign="top">
        <b>🎥 Canlı Radar & Tarama Demosu</b><br/><br/>
        <video src="https://github.com/user-attachments/assets/2d473dc3-e8cf-4ded-87e8-f8c0f5b02401" controls="controls" width="95%" style="border-radius: 8px; box-shadow: 0 4px 15px rgba(0,0,0,0.15);"></video>
        <br/><br/><em>0°–180° Mekanik Tarama & Otonom Hedef Tespiti</em>
      </td>
    </tr>
  </table>
</div>

---

<a id="devre-baglantisi"></a>
## 🔌 4. Devre Bağlantı Şeması ve Pin Haritası

```text
+-----------------------------------+              +-----------------------------+
|          Deneyap Kart 1A          |              |        Modül / Sensör       |
|                                   |              |                             |
|  I2C SDA & SCL (0x29)             |<------------>| VL53L0X ToF Derinlik Ölçer  |
|  D9 (PWM Sinyal Çıkışı)           |------------->| SG90 Servo Motor (Sinyal)   |
|  CAM Veri Yolu (D2 - D9)          |<------------>| OV2640 Kamera Modülü        |
|  CAMSC / CAMSD                    |<------------>| Kamera I2C Kontrol Hatları  |
|  3.3V / 5V                        |------------->| Sensör ve Modül Beslemeleri |
|  GND                              |<------------>| Ortak GND Toprak Hattı      |
+-----------------------------------+              +-----------------------------+
```

> ⚡ **Önemli Güç İpucu:** Servo motor ani dönüş anında ani akım dalgalanması oluşturabileceğinden harici stabil bir 5V besleme veya USB güç kaynağı kullanılması önerilir. Toprak hatları (GND) mutlaka ortak bağlanmalıdır.

---

<a id="sistem-mimarisi"></a>
## 📡 5. Sistem Mimarisi ve Çalışma Mantığı

```text
[ Servo Motor (0°-180°) ] ──► [ ToF Lazer Sensörü ]
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

<a id="web-arayuzu"></a>
## 🌐 6. Web Arayüzü Özellikleri

ESP32 üzerinde koşan dahili asenkron web sunucusu:
- **Erişim Adresi:** `http://<ESP32_IP_ADRESI>/`
- **📸 Son 3 Fotoğraf:** 40 cm altına inen hedeflerin yakalanan son 3 karesi 3 saniyede bir otomatik güncellenerek gösterilir (`/lastImages`).
- **📏 Canlı Radar Verisi:** Anlık açı ve mesafe değerleri her 0.5 saniyede bir sunucudan çekilir (`/radar`).

---

<a id="processing-radar"></a>
## 🖥️ 7. Processing ile 2D Radar Görselleştirme

Bilgisayarda çalışan **Processing (`radar.pde`)** uygulaması seri port üzerinden ESP32'den gelen verileri dinler:
1. **Dairesel Radar Izgarası:** 0° ile 180° arasında dönen yeşil radar çizgisi çevreyi tarar.
2. **Hedef Tespiti:** 40 cm altında nesne görüldüğü koordinata **kırmızı iz/nokta** yerleştirilir.
3. **Telemetri Paneli:** Ekranın sağ altında anlık **Açı (Angle)** ve **Mesafe (Distance cm)** sayısal olarak yazılır.

```text
             [90°]
         /     |     \
     /         |         \
 [0°] ------- [o] ------- [180°]
          (Servo + ToF)
```

---

<a id="kurulum-ve-calistirma"></a>
## 🚀 8. Adım Adım Kurulum ve Çalıştırma Rehberi

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

### 3. Kodu Yükleme ve Web Arayüzüne Bağlanma
Kodu Deneyap Kart'a yükleyin. Seri Monitör'ü (`115200 baud`) açarak kartın aldığı IP adresini not edin:
```text
✅ WiFi bağlı. IP: 192.168.1.100
```
Tarayıcınızda bu IP adresine girerek çekilen son 3 fotoğrafı ve mesafe değerlerini görebilirsiniz.

### 4. Processing Radarını Çalıştırma
1. [Processing IDE](https://processing.org/download) indirin ve açın.
2. Depodaki `radar/radar.pde` dosyasını açın.
3. Koddaki `COM75` portunu kendi kartınızın bağlı olduğu port ile değiştirin (örneğin `COM3` veya `/dev/ttyUSB0`).
4. **Çalıştır (▶)** butonuna basın.

---

<a id="sorunlar-ve-cozumleri"></a>
## ⚠️ 9. Karşılaşılabilecek Sorunlar ve Çözümleri

### 1. "Port Busy" / Port Meşgul Hatası
> 🔴 **Önemli Kural:** Bilgisayardaki bir seri port (COM) aynı anda sadece tek bir program tarafından okunabilir.
> - **Neden:** Arduino IDE Seri Monitörü açıkken Processing başlatılırsa port çakışması yaşanır.
> - **Çözüm:** Kodu karta yükledikten sonra **Arduino Seri Monitörünü mutlaka kapatın**, ardından Processing uygulamasını çalıştırın!

### 2. Processing Radar Ekranında Veri Akmıyor / Sabit Kalıyor
- **Neden:** ESP32 kodu hem web sunucusuna veri yazıp hem de seri porta `Açı,Mesafe.` formatında göndermelidir.
- **Çözüm:** `radar_kamera.ino` dosyasındaki döngüye (`loop`) şu satırın eklendiğinden emin olun:
```cpp
Serial.print(angle);
Serial.print(",");
Serial.print((int)distance);
Serial.print(".");
```

### 3. Kamera Görüntüsü Gelmiyor veya Reset Atıyor
- **Neden:** OV2640 kamera ve servo motor aynı anda yüksek anlık akım çeker.
- **Çözüm:** Kartı kaliteli ve en az 2A güç veren bir USB kablosu veya harici adaptör ile besleyin.

---

<a id="gelistirici"></a>
## 👩‍💻 10. Geliştirici

**Kevser ÜSTÜN**  
🎓 Giresun Üniversitesi — Bilgisayar Mühendisliği  
🔗 [GitHub Profilim](https://github.com/kvsrustun)

---

<a id="lisans"></a>
## 📄 11. Lisans

Bu proje [MIT Lisansı](LICENSE) kapsamında açık kaynak olarak paylaşılmıştır.



