/* ************************************************************** */
/* ************* Irrigação Automática - Horta ELIMU ************* */
/* ************************************************************** */

// inclui a biblioteca:
#include <LiquidCrystal.h>

// definição dos pinos de conexão Arduino <> Display LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// variáveis
const int pinoSensor = A0;
const int pinoValvula = 10;

const int pinBuzzer = 9;
const int pinAgua = A2;
float water = 0.0;

const float limiarSeco = 50.0; // Percentual EMPÍRICO de umidade
const int tempoRega = 5; // Tempo de rega em segundos
float umidadeSolo = 0.0;
float umidadeReal = 0.0;
float convert = 0.0;

void setup() {
  pinMode(pinBuzzer,OUTPUT);
  pinMode(pinoValvula, OUTPUT);
  // Desliga a bomba
  digitalWrite(pinoValvula, LOW);
  // define o tamanho do Display LCD
  lcd.begin(16, 2);
  // Exibe a mensagem no Display LCD.
  lcd.print(" Horta ELIMU ");

  Serial.begin(9600);
}


void loop() {
  // Mede a umidade a cada x segundos. Uma hora = 3600 segundos.
  for(int i=1; i<=10; i++) {
    // Posiciona o cursor do LCD na coluna 0 linha 1
    // (Obs: linha 1 é a segunda linha, a contagem começa em 0
    lcd.setCursor(0, 1);
    // Exibe a mensagem no Display LCD:
    // Faz a leitura do sensor de umidade do solo
    umidadeSolo = analogRead(pinoSensor);
    // Converte a variação do sensor de 0 a 1023 para 0 a 100
    convert = map(umidadeSolo, 1023, 0, 0, 100);
    // Somatório de todos os valores lidos no período de tempo (para melhor acurácia do valor lido)
    umidadeReal = umidadeReal + convert;
    // Exibe a mensagem no Display LCD:
    lcd.print(String("Umidade: ") + String(umidadeReal/i) + String("%"));
    //lcd.print(umidadeReal/i);
    //lcd.print("%");
    // Espera um segundo
    delay(1000);
  }

  // Conversão para único valor
  // divisão pelo mesmo valor do laço (que define também o tempo)
  umidadeReal = umidadeReal/10.0;

  // verificação se o reservatório possui água
  // se sim, a saída é zero, senão, valor
  water = analogRead(pinAgua);
  if(water > 0.0){
    lcd.setCursor(0, 1);
    // Exibe a mensagem no Display LCD:
    lcd.print("  > SEM AGUA! <  ");
    
    // Aciona o buzzer na frequencia relativa ao Dó em Hz
    for(int i=0; i<10; i++){
      tone(pinBuzzer,261);
      delay(500);
      //Desativa o buzzer
      noTone(pinBuzzer);
      delay(2000);
    }
    // zera a variável
    umidadeReal = 0.0;
  }
  
  else if(umidadeReal < limiarSeco) {
    // Posiciona o cursor do LCD na coluna 0 linha 1
    // (Obs: linha 1 é a segunda linha, a contagem começa em 0
    lcd.setCursor(0, 1);
    // Exibe a mensagem no Display LCD:
    lcd.print("    Regando!    ");
    // Liga Bomba
    digitalWrite(pinoValvula, HIGH);
    // Mantém bomba ligada por tempo determinado
    delay(tempoRega*1000);
    // Desliga Bomba
    digitalWrite(pinoValvula, LOW);
    // Zera a variável como despejo de memória
    umidadeReal = 0.0;
    delay(2000);
    }
    
    else {
      // Posiciona o cursor do LCD na coluna 0 linha 1
      // (Obs: linha 1 é a segunda linha, a contagem começa em 0
      lcd.setCursor(0, 1);
      // Exibe a mensagem no Display LCD:
      lcd.print("Solo Encharcado!");
      // Zera a variável como despejo de memória
      umidadeReal = 0.0;
      delay(2000);
    }
}


/*
 * RELEY-TEST

#define dpin 7
void setup() {
  pinMode(dpin, OUTPUT);
}
void loop() {
  digitalWrite(dpin, HIGH);
  delay(10000);
  digitalWrite(dpin, LOW);
  delay(10000);
}

  *MOINSTURE SENSOR TEST
#define SensorPin A0 
float sensorValue = 0;
 
void setup() { 
 Serial.begin(9600); 
}
 
void loop() { 
 for (int i = 0; i <= 100; i++) { 
 sensorValue = sensorValue + analogRead(SensorPin); 
 delay(5); 
 }
  
 sensorValue = sensorValue/100.0; 
 Serial.println(sensorValue); 
 delay(30); 
} 
*/
