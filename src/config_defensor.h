#pragma once

// =====================================================
// ARQUIVO DE CONFIGURAÇÃO — DEFENSOR
// Ajuste aqui sem precisar mexer no resto do código!
// =====================================================

// ── Rede Wi-Fi ───────────────────────────────────────
#define SSID_DA_REDE       "GAME_NEXUS"
#define SENHA_DA_REDE      ""           // sem senha na rede do jogo

// ── Broker MQTT (notebook com IP fixo) ───────────────
#define IP_DO_BROKER       "192.168.1.100"
#define PORTA_MQTT         1883
#define ID_MQTT            "defensor_01"  // ID único — troque se tiver 2 defensores

// ── Tópicos MQTT ─────────────────────────────────────
#define TOPICO_HP          "game/defensor/hp"
#define TOPICO_PUNICAO     "game/defensor/punicao"
#define TOPICO_COMANDO     "game/nexus/comando"

// ── Pinos do Hardware ─────────────────────────────────
//    NodeMCU v2 — veja o diagrama de ligação (WIRING.md)
#define PINO_LEDS          D2   // GPIO4  — sinal da fita WS2812B (via resistor 330Ω)
#define PINO_LDR_1         D5   // GPIO14 — pino DO do Módulo LDR 1
#define PINO_LDR_2         D6   // GPIO12 — pino DO do Módulo LDR 2

// ── Calibração dos Sensores LDR ──────────────────────
//    Teste com o laser apontado no sensor e observe o Serial Monitor.
//    Se "HIT" aparecer sem laser → mude para LOW.
//    Se nada aparecer com laser → mude para HIGH.
#define LDR_NIVEL_HIT      HIGH  // nível do pino DO quando o laser acerta o sensor

// ── Fita WS2812B ─────────────────────────────────────
#define TOTAL_LEDS         5    // 5 LEDs cortados da fita
#define BRILHO_LEDS        80   // 0–255 (80 ≈ 30% — econômico e visível em eventos)

// ── Parâmetros de Jogo ────────────────────────────────
#define HP_MAXIMO          5
#define TEMPO_RECARGA_MS   4000
#define DEBOUNCE_HIT_MS    350

// ── Modo de Teste (hardware sem rede) ─────────────────
//    Descomente para testar LEDs e LDRs sem Wi-Fi nem START.
//    LEMBRE de comentar novamente antes do evento!
//
// #define MODO_TESTE
