#include <WiFi.h>
#include <PubSubClient.h>

// Wifi MQTT
String ssid = "Maeda";
String senha = "50312312";
String broker = "broker.hivemq.com";
const int portaBroker = 1883;

WiFiClient espClient;
PubSubClient mqtt(espClient);

#define TOPICO_PUBLISH "maeda/esp32/bpm"

// AD8232
const int pinoECG = 33;
int limite = 2000;
bool pulsoDetectado = false;
unsigned long ultimoPico = 0;
float ultimoBPM = 0;

// Buzzer
const int pinoBuzzer = 25;

// Conexão Wi-Fi
void WifiConnect() {
  Serial.print("Conectando-se ao WiFi");
  WiFi.begin(ssid.c_str(), senha.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }

  Serial.println("\nWiFi conectado!");
}

// Conexão MQTT
void MQTTconnect() {
  while (!mqtt.connected()) {
    Serial.println("Conectando MQTT");
    if (mqtt.connect("ESP32_maeda")) {
      Serial.println("Conectado!");
    } else {
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pinoECG, INPUT);
  pinMode(pinoBuzzer, OUTPUT);

  WifiConnect();
  mqtt.setServer(broker.c_str(), portaBroker);

  Serial.println("Sistema inicializado.");
}

void loop() {
  if (!mqtt.connected()) {
    MQTTconnect();
  }
  mqtt.loop();

  int leituraECG = analogRead(pinoECG);
  unsigned long tempoAtual = millis();

  // Detecta pico de sinal
  if (leituraECG > limite && !pulsoDetectado) {
    pulsoDetectado = true;
    unsigned long intervaloBPM = tempoAtual - ultimoPico;
    ultimoPico = tempoAtual;

    // Buzzer
    tone(pinoBuzzer, 500);
    delay(40);
    noTone(pinoBuzzer);

    // Calcula o BPM 
    if (intervaloBPM > 300 && intervaloBPM < 2000) {
      float bpm = 60000 / intervaloBPM;
      bpm = (bpm + ultimoBPM) / 2;
      ultimoBPM = bpm;

      Serial.print("BPM atual: ");
      Serial.println(bpm);

      String mensagem = "BPM=" + String(bpm, 1);
      unsigned long inicioEnvio = millis();
      mqtt.publish(TOPICO_PUBLISH, mensagem.c_str());
      unsigned long fimEnvio = millis();
      Serial.print("Medição MQTT: ");
      Serial.println(fimEnvio - inicioEnvio);
    }
  }

  // Reseta a flag
  if (leituraECG < (limite - 100)) {
    pulsoDetectado = false;
  }

  delay(5);
}
