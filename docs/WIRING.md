# Ligação Elétrica — Defensor

## Corte da Fita WS2812B IP67 (5 LEDs)

A fita IP67 vem com revestimento de silicone extrudado (tubo ao redor da fita).
Siga os passos abaixo para cortar e preparar os 5 LEDs:

### Ferramentas necessárias
- Estilete ou bisturi
- Tesoura de corte fino (ou alicate de corte)
- Ferro de solda + estanho
- Tubo termo-retrátil (3–4 mm) ou cola quente
- Álcool isopropílico + cotonete

### Passo a passo

1. **Localizar os pontos de corte.**
   Entre cada LED há 3 pastilhas de cobre expostas (GND, +5V, DI).
   A fita tem marcas de tesoura impressas nesses intervalos.
   Conte 5 LEDs a partir de uma das extremidades.

2. **Marcar o ponto de corte.**
   Após o 5º LED, localize o intervalo com as 3 pastilhas de cobre.
   Este é o ponto de corte.

3. **Cortar o silicone.**
   Com o estilete, pressione levemente ao redor do tubo de silicone
   no ponto de corte — apenas para marcar a posição.
   NÃO force: o silicone é flexível e o estilete guia o corte.

4. **Cortar a fita.**
   Use a tesoura para cortar exatamente sobre as pastilhas de cobre,
   no centro do intervalo marcado.
   Resultado: uma seção com 5 LEDs e uma extremidade de entrada (original)
   e uma extremidade de saída (cortada).

5. **Preparar a extremidade cortada (saída — sem uso).**
   Com o estilete, descasque ~8 mm do silicone a partir do corte.
   Limpe os pads com álcool isopropílico.
   Aplique uma gota de cola quente ou dobre o tubo e vede com
   termo-retrátil para evitar curto-circuito.

6. **Preparar a extremidade de entrada (onde entram os fios).**
   Descasque ~10 mm do silicone.
   Os 3 pads ficam expostos:
     - Pad 1: GND  (preto / negativo)
     - Pad 2: +5V  (vermelho / positivo)
     - Pad 3: DI   (verde ou amarelo / dado)
   Estanhe os 3 pads com ferro de solda.

7. **Soldar os fios.**
   Conecte 3 fios (preferencialmente nas cores convencionais):
     - Preto  → GND
     - Vermelho → +5V
     - Verde/Amarelo → DI

---

## Ligação ao NodeMCU

```
NodeMCU           Fita WS2812B (entrada)
─────────         ────────────────────────
Vin (5V)  ──┬──── +5V
             │
            [1000µF]   ← capacitor entre Vin e GND
             │          (protege a fita de pico de corrente)
GND       ──┴──── GND
D2        ──[330Ω]──── DI   ← resistor de 330 ohms em série

NodeMCU           Módulo LDR 1
─────────         ─────────────
3V3       ──────── VCC
GND       ──────── GND
D5        ──────── DO  (saída digital)

NodeMCU           Módulo LDR 2
─────────         ─────────────
3V3       ──────── VCC
GND       ──────── GND
D6        ──────── DO  (saída digital)
```

### Componentes adicionais
| Componente | Valor | Posição |
|---|---|---|
| Resistor | 330 Ω | em série entre D2 e DI da fita |
| Capacitor | 1000 µF / 6.3V | entre Vin e GND, próximo à fita |

---

## Calibração dos Módulos LDR

Cada módulo LDR possui um potenciômetro azul (trimmer).

1. Ligue o NodeMCU e abra o Serial Monitor (115200 baud).
2. Aponte o laser diretamente no LDR 1.
3. Gire o potenciômetro até o LED indicador do módulo mudar de estado
   (acender ou apagar) — nesse ponto o laser está sendo detectado.
4. Verifique no Serial Monitor: "HIT!" deve aparecer ao apontar o laser.
5. Repita para o LDR 2.

Se os hits aparecerem sem o laser apontado:
  → Ajuste LDR_NIVEL_HIT em config.h de HIGH para LOW.
Se nada aparecer mesmo com o laser:
  → Ajuste LDR_NIVEL_HIT de LOW para HIGH.

### Dica de montagem no escudo
Para maior precisão, coloque um canudo (palha) curto (~3 cm) sobre
cada sensor LDR. O canudo age como colimador: só o laser vindo reto
acerta o sensor, bloqueando a luz ambiente lateral.
```
  [===| LDR |===]  ← visão de cima
       ↑ canudo protege a lateral
```
