#pragma once
#include <Arduino.h>

// =====================================================
// MÓDULO DE JOGO — Lógica do Escudo do Defensor
// =====================================================

// ── Estados possíveis do escudo ───────────────────────
enum EstadoEscudo {
  JOGO_PAUSADO,        // aguardando o comando START do Nexus
  ESCUDO_ATIVO,        // absorvendo tiros normalmente
  ESCUDO_RECARREGANDO  // escudo zerado, recarregando
};

// ── Funções públicas (chamadas de main.cpp) ───────────
void jogo_iniciar();
void jogo_iniciarPartida();   // chamada pelo callback MQTT ao receber START

void jogo_verificarLaser();   // verificar hit — chame em TODO loop()
void jogo_verificarRecarga(); // gerenciar recarga — chame em TODO loop()

// ── Getters (para leitura de estado) ──────────────────
int          jogo_getHP();
EstadoEscudo jogo_getEstado();
