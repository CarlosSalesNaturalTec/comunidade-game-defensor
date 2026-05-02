#include "leds.h"

// ── Array interno da fita ─────────────────────────────
static CRGB _fita[TOTAL_LEDS];

// ── Controle de animação não-bloqueante ───────────────
static unsigned long _ultimoBlinkMs = 0;
static bool          _blinkEstado   = false;

// ─────────────────────────────────────────────────────
void leds_iniciar() {
  FastLED.addLeds<WS2812B, PINO_LEDS, GRB>(_fita, TOTAL_LEDS);
  FastLED.setBrightness(BRILHO_LEDS);
  leds_apagar();
  Serial.println("[LEDS] Fita WS2812B pronta!");
}

// Acende tantos LEDs azuis quanto o HP atual do escudo
//   HP 5 → 5 LEDs azuis (escudo cheio)
//   HP 2 → 2 LEDs azuis (escudo fraco)
//   HP 0 → tudo apagado (escudo quebrado)
void leds_mostrarHP(int hp) {
  for (int i = 0; i < TOTAL_LEDS; i++) {
    _fita[i] = (i < hp) ? CRGB::Blue : CRGB::Black;
  }
  FastLED.show();
}

// Flash vermelho curto ao tomar um dano
// Bloqueante por ~200ms — seguro porque o debounce já está ativo
void leds_flashDano() {
  fill_solid(_fita, TOTAL_LEDS, CRGB::Red);
  FastLED.show();
  delay(140);
  leds_apagar();
  delay(60);
}

// Pisca laranja enquanto o escudo recarrega
// Não-bloqueante: chame a cada ciclo do loop()
void leds_animarRecarga() {
  if (millis() - _ultimoBlinkMs >= 380) {
    _ultimoBlinkMs = millis();
    _blinkEstado   = !_blinkEstado;

    // Laranja (255, 80, 0) alternando com apagado
    CRGB cor = _blinkEstado ? CRGB(255, 80, 0) : CRGB::Black;
    fill_solid(_fita, TOTAL_LEDS, cor);
    FastLED.show();
  }
}

void leds_apagar() {
  fill_solid(_fita, TOTAL_LEDS, CRGB::Black);
  FastLED.show();
}
