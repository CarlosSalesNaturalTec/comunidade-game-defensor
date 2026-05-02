#pragma once
#include <Arduino.h>

// =====================================================
// MÓDULO MQTT — Comunicação com o Broker
// =====================================================

// ── Ciclo de vida ─────────────────────────────────────
void mqtt_iniciar();       // conecta Wi-Fi e broker (chame no setup())
void mqtt_reconectar();    // reconecta sem resetar o estado do jogo
bool mqtt_conectado();
void mqtt_processar();     // processa mensagens recebidas (chame no loop())

// ── Publicações para o Nexus ──────────────────────────
void mqtt_publicarHP(int hp);
void mqtt_publicarPunicao();   // publica DANO_DUPLO → Torre aplica ao receber
