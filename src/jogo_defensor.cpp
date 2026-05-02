#include "jogo_defensor.h"
#include "leds.h"
#include "mqtt_defensor.h"
#include "config.h"

// ── Variáveis internas de estado ──────────────────────
static EstadoEscudo  _estado          = JOGO_PAUSADO;
static int           _hpEscudo        = HP_MAXIMO;
static unsigned long _ultimoHitMs     = 0;
static unsigned long _inicioRecargaMs = 0;

// ─────────────────────────────────────────────────────
// FUNÇÕES INTERNAS (privadas — não aparecem no .h)
// ─────────────────────────────────────────────────────

// Retorna true se qualquer um dos dois sensores detectou o laser
static bool laserDetectado() {
  bool sensor1 = (digitalRead(PINO_LDR_1) == LDR_NIVEL_HIT);
  bool sensor2 = (digitalRead(PINO_LDR_2) == LDR_NIVEL_HIT);
  return sensor1 || sensor2;
}

// Retorna true se o intervalo mínimo entre dois hits foi respeitado
// Evita que um único tiro seja contado múltiplas vezes
static bool debounceOk() {
  return (millis() - _ultimoHitMs) >= DEBOUNCE_HIT_MS;
}

// ─────────────────────────────────────────────────────
// FUNÇÕES PÚBLICAS
// ─────────────────────────────────────────────────────

void jogo_iniciar() {
  pinMode(PINO_LDR_1, INPUT);
  pinMode(PINO_LDR_2, INPUT);
  _estado   = JOGO_PAUSADO;
  _hpEscudo = HP_MAXIMO;
  leds_apagar();

  #ifdef MODO_TESTE
    Serial.println("[JOGO] *** MODO TESTE ATIVO — escudo pronto sem START! ***");
    Serial.println("[JOGO] Aponte o laser nos sensores para testar.");
    jogo_iniciarPartida();
  #else
    Serial.println("[JOGO] Defensor inicializado. Aguardando START...");
  #endif
}

void jogo_iniciarPartida() {
  _hpEscudo        = HP_MAXIMO;
  _estado          = ESCUDO_ATIVO;
  _ultimoHitMs     = 0;
  _inicioRecargaMs = 0;

  leds_mostrarHP(_hpEscudo);

  Serial.println("[JOGO] *** PARTIDA INICIADA! Escudo com energia maxima. ***");
  mqtt_publicarHP(_hpEscudo);
}

// Verifica se o laser acertou o escudo — chame em TODO loop()
void jogo_verificarLaser() {
  // Só processa hits quando o escudo está ativo
  if (_estado != ESCUDO_ATIVO) return;

  if (laserDetectado() && debounceOk()) {
    _ultimoHitMs = millis();  // registra o instante do hit para o debounce

    // O laser acertou! Reduzir um ponto de energia do escudo.
    _hpEscudo--;
    leds_flashDano();
    leds_mostrarHP(_hpEscudo);

    Serial.print("[JOGO] HIT! HP do escudo: ");
    Serial.print(_hpEscudo);
    Serial.print(" / ");
    Serial.println(HP_MAXIMO);

    mqtt_publicarHP(_hpEscudo);

    if (_hpEscudo <= 0) {
      // Escudo zerou! O Defensor deve recuar agora.
      _hpEscudo        = 0;
      _estado          = ESCUDO_RECARREGANDO;
      _inicioRecargaMs = millis();
      Serial.println("[JOGO] *** Escudo QUEBRADO! Recarregando... Defensor, recua! ***");
    }
  }
}

// Gerencia recarga e punição — chame em TODO loop()
void jogo_verificarRecarga() {
  if (_estado != ESCUDO_RECARREGANDO) return;

  // ── Animação laranja piscante durante a recarga (não-bloqueante) ──
  leds_animarRecarga();

  // ── Punição: Defensor insistiu em bloquear com escudo zerado ──
  if (laserDetectado() && debounceOk()) {
    _ultimoHitMs = millis();
    Serial.println("[JOGO] PUNICAO! Defensor bloqueou com escudo zerado!");
    mqtt_publicarPunicao();
  }

  // ── Verificar se o tempo de recarga terminou ──
  if ((millis() - _inicioRecargaMs) >= TEMPO_RECARGA_MS) {
    _hpEscudo = HP_MAXIMO;
    _estado   = ESCUDO_ATIVO;
    leds_mostrarHP(_hpEscudo);
    Serial.println("[JOGO] Escudo RECARREGADO! De volta ao combate.");
    mqtt_publicarHP(_hpEscudo);
  }
}

int jogo_getHP() {
  return _hpEscudo;
}

EstadoEscudo jogo_getEstado() {
  return _estado;
}
