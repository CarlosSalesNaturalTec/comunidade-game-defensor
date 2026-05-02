#include <Arduino.h>
#include "config.h"
#include "leds.h"
#include "mqtt_defensor.h"
#include "jogo_defensor.h"

// ==========================================================
// DEFENSOR — Firmware do Escudo de Luz
// Projeto: GAME — Comunidade de Inclusão Digital
//
// Hardware: NodeMCU ESP8266 v2
//           Fita WS2812B IP67 (5 LEDs cortados)
//           2x Módulo Sensor LDR
//
// Descrição do Escudo:
//   O Defensor protege a Torre dos tiros do Atacante.
//   Cada tiro do laser apaga 1 LED azul do escudo.
//   Quando todos os 5 LEDs apagam → escudo quebrado!
//   O escudo pisca laranja e recarrega em 4 segundos.
//   Se o Defensor bloquear com o escudo zerado →
//   a Torre recebe DANO DUPLO (punição)!
// ==========================================================

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("==============================================");
  Serial.println("   DEFENSOR — Escudo de Luz   v1.0");
  Serial.println("==============================================");

  leds_iniciar();    // prepara a fita WS2812B
  mqtt_iniciar();    // conecta ao Wi-Fi e ao broker MQTT
  jogo_iniciar();    // configura os pinos LDR e o estado inicial

  Serial.println("[SETUP] Pronto! Aguardando START do Nexus...");
  Serial.println();
}

void loop() {
  // ── Passo 1: manter a conexão com o broker ────────────
  //    Se cair durante a partida, reconecta sem resetar o HP
  if (!mqtt_conectado()) {
    mqtt_reconectar();
  }
  mqtt_processar();  // processa mensagens MQTT recebidas

  // ── Passo 2: verificar se o laser acertou o escudo ───
  jogo_verificarLaser();

  // ── Passo 3: gerenciar recarga e punição ─────────────
  jogo_verificarRecarga();
}
