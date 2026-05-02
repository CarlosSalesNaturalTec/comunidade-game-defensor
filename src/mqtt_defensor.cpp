#include "mqtt_defensor.h"
#include "jogo_defensor.h"
#include "config.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ── Objetos de rede (privados) ────────────────────────
static WiFiClient   _wifi;
static PubSubClient _mqtt(_wifi);

// ─────────────────────────────────────────────────────
// CALLBACK: chamado automaticamente quando chega mensagem
// ─────────────────────────────────────────────────────
static void aoReceberMensagem(char* topico, byte* payload, unsigned int tamanho) {
  // Converter o payload de bytes para texto terminado em '\0'
  char mensagem[tamanho + 1];
  memcpy(mensagem, payload, tamanho);
  mensagem[tamanho] = '\0';

  Serial.print("[MQTT] Mensagem em '");
  Serial.print(topico);
  Serial.print("': ");
  Serial.println(mensagem);

  // Processar apenas comandos vindos do Nexus
  if (String(topico) == TOPICO_COMANDO) {
    StaticJsonDocument<64> doc;
    if (deserializeJson(doc, mensagem) != DeserializationError::Ok) return;

    const char* cmd = doc["cmd"] | "";

    if (strcmp(cmd, "START") == 0) {
      Serial.println("[MQTT] → Comando START recebido!");
      jogo_iniciarPartida();
    }
    // Futuros comandos (PAUSE, STOP) podem ser adicionados aqui
  }
}

// ─────────────────────────────────────────────────────
void mqtt_iniciar() {
  // ── 1. Configurar broker e callback (independe do Wi-Fi) ──
  _mqtt.setServer(IP_DO_BROKER, PORTA_MQTT);
  _mqtt.setCallback(aoReceberMensagem);

  // ── 2. Tentar conectar ao Wi-Fi com timeout ────────
  //    Se a rede não estiver disponível em 10s, o escudo
  //    entra em MODO OFFLINE: LEDs e LDRs funcionam
  //    normalmente. O loop() tentará reconectar depois.
  Serial.print("[MQTT] Conectando ao Wi-Fi '");
  Serial.print(SSID_DA_REDE);
  Serial.print("' (timeout: 10s)");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_DA_REDE, SENHA_DA_REDE);

  constexpr unsigned long TIMEOUT_WIFI_MS = 10000;
  unsigned long inicio = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - inicio >= TIMEOUT_WIFI_MS) {
      Serial.println();
      Serial.println("[MQTT] Wi-Fi nao encontrado — MODO OFFLINE ativado.");
      Serial.println("[MQTT] O escudo funciona normalmente.");
      Serial.println("[MQTT] Reconexao automatica sera tentada no loop().");
      return;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.print(" OK! IP: ");
  Serial.println(WiFi.localIP());

  // ── 3. Conectar ao broker MQTT ────────────────────
  mqtt_reconectar();
}

void mqtt_reconectar() {
  if (_mqtt.connected()) return;

  // ── Sem Wi-Fi → tenta reconectar a rede primeiro ──
  if (WiFi.status() != WL_CONNECTED) {
    static unsigned long _ultimaTentativaWifi = 0;
    if (millis() - _ultimaTentativaWifi < 5000) return;
    _ultimaTentativaWifi = millis();

    Serial.println("[MQTT] Wi-Fi desconectado. Tentando reconectar...");
    WiFi.disconnect();
    WiFi.begin(SSID_DA_REDE, SENHA_DA_REDE);
    return;
  }

  // ── Wi-Fi OK → tenta o broker MQTT ───────────────
  Serial.print("[MQTT] Conectando ao broker...");

  if (_mqtt.connect(ID_MQTT)) {
    Serial.println(" Conectado!");

    _mqtt.subscribe(TOPICO_COMANDO);
    Serial.print("[MQTT] Assinando: ");
    Serial.println(TOPICO_COMANDO);

    // Republicar HP atual ao reconectar — mantém o Nexus sincronizado
    mqtt_publicarHP(jogo_getHP());

  } else {
    Serial.print(" Falhou! Codigo MQTT: ");
    Serial.println(_mqtt.state());
  }
}

bool mqtt_conectado() {
  return _mqtt.connected();
}

void mqtt_processar() {
  _mqtt.loop();
}

// ─────────────────────────────────────────────────────

void mqtt_publicarHP(int hp) {
  // Monta: {"hp": 3, "status": "ativo"}
  StaticJsonDocument<64> doc;
  doc["hp"]     = hp;
  doc["status"] = (hp > 0) ? "ativo" : "quebrado";

  char payload[64];
  serializeJson(doc, payload);

  _mqtt.publish(TOPICO_HP, payload);

  Serial.print("[MQTT] Publicou HP: ");
  Serial.println(payload);
}

void mqtt_publicarPunicao() {
  // Monta: {"tipo": "DANO_DUPLO"}
  // Publicado 2x com pequeno intervalo — PubSubClient v2 usa QoS 0 por padrão.
  // A publicação dupla compensa a eventual perda de pacote UDP em redes lotadas.
  StaticJsonDocument<48> doc;
  doc["tipo"] = "DANO_DUPLO";

  char payload[48];
  serializeJson(doc, payload);

  _mqtt.publish(TOPICO_PUNICAO, payload);
  delay(60);
  _mqtt.publish(TOPICO_PUNICAO, payload);

  Serial.println("[MQTT] Punicao publicada: DANO_DUPLO (2x para garantir entrega)");
}
