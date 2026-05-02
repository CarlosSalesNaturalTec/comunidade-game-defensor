#pragma once
#define FASTLED_ESP8266_RAW_PIN_ORDER   // necessário para NodeMCU ESP8266
#include <FastLED.h>
#include "config.h"

// =====================================================
// MÓDULO DE LEDS — Fita WS2812B
// Controla os 5 LEDs do escudo do Defensor
// =====================================================

// ── Inicialização ─────────────────────────────────────
void leds_iniciar();

// ── Modos de Exibição ─────────────────────────────────
void leds_mostrarHP(int hp);   // acende N LEDs azuis (N = HP atual)
void leds_flashDano();          // pisca vermelho ao receber um hit
void leds_animarRecarga();      // pisca laranja durante a recarga (não-bloqueante)
void leds_apagar();             // apaga todos os LEDs
