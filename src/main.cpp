#include <Arduino.h>

const int botao = 11;
const int led_vermelho = 9;
const int led_azul = 10;
const int segmentos[] = {8, 7, 6, 5, 4, 3, 2};
const int pinoPonto = 9;

// Códigos Morse para números (. = ponto, - = traço)
const String codigoMorse[] = {
  "-----", // 0
  ".----", // 1
  "..---", // 2
  "...--", // 3
  "....-", // 4
  ".....", // 5
  "-....", // 6
  "--...", // 7
  "---..", // 8
  "----."  // 9
};

const byte numeros[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9

};

String entradaMorse = "";

unsigned long tempoPressionado = 0;
unsigned long tempoSolto = 0;

bool botaoPressionado = false;

const int TEMPO_PONTO = 250; // Tempo máximo para um ponto
const int TEMPO_ENTRE_SINAIS = 500; // Tempo entre sinais

void exibirNumero(int numero);
void piscarLedAzul();
void piscarLedVermelho();

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(segmentos[i], OUTPUT);
  }

  pinMode(pinoPonto, OUTPUT);
  pinMode(botao, INPUT_PULLUP);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_azul, OUTPUT);

  digitalWrite(pinoPonto, LOW);
  exibirNumero(0);
  Serial.begin(9600); // Para debug
}

void loop() {
  int estadoBotao = digitalRead(botao);

  // Botão pressionado
  if (estadoBotao == LOW && !botaoPressionado) {
    tempoPressionado = millis();
    botaoPressionado = true;
  }

  // Botão solto
  if (estadoBotao == HIGH && botaoPressionado) {
    unsigned long duracao = millis() - tempoPressionado;
    botaoPressionado = false;

    if (duracao < TEMPO_PONTO) {
      entradaMorse += ".";
    } else {
      entradaMorse += "-";
    }
    tempoSolto = millis();
    Serial.println(entradaMorse); // Debug
  }

  // Verifica se passou tempo suficiente desde o último sinal
  if (!botaoPressionado && (millis() - tempoSolto) > TEMPO_ENTRE_SINAIS && entradaMorse.length() > 0) {
    bool encontrado = false;
    // Procura o número correspondente ao código Morse
    for (int i = 0; i < 10; i++) {
      if (codigoMorse[i] == entradaMorse) {
        exibirNumero(i);
        piscarLedAzul();
        encontrado = true;
        break;
      }
    }

    if (!encontrado) {
      piscarLedVermelho();
    }
    entradaMorse = ""; // Limpa a entrada para o próximo código
  }
}

void exibirNumero(int numero) {
  byte mascara = numeros[numero];

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentos[i], (mascara >> i) & 0x01);
  }
}

void piscarLedAzul() {
  digitalWrite(led_azul, HIGH);
  delay(500);
  digitalWrite(led_azul, LOW);
}

void piscarLedVermelho() {
  digitalWrite(led_vermelho, HIGH);
  delay(500);
  digitalWrite(led_vermelho, LOW);
}
