/* ************************************************************** */
/* ************* Irrigação Automática - Horta ELIMU ************* */
/* ************************************************************** */

// bibliotecas:
#include <LiquidCrystal.h>
//#include <NewPing.h>
#include <Ultrasonic.h>

// definição dos pinos de conexão Arduino <> Display LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// declaracao das variaveis
const int pinoSensor = A0;
const int pinoValvula = 10;
const int pinBuzzer = 9;
const int pinAgua = A2;
const int triggerPin = 6; 
const int echoPin = 7;    

const float limiarSeco = 50.0; // Percentual empirico de umidade
const float tempoRega = 12.0;  // Tempo de rega em segundos
int distancia;     // Variavel para dist. medida

float umidadeSolo = 0.0;
float umidadeReal = 0.0;
float convert = 0.0;

//Inicializa os pinos
//NewPing sonar(triggerPin, echoPin, nivelMin);
Ultrasonic ultrasonic(triggerPin,echoPin); 

void setup() {
  pinMode(echoPin,INPUT);
  pinMode(triggerPin,OUTPUT);
  
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

//metodo 'hcsr04()' fara a medicao da distancia utilizando o sensor sonar
void hcsr04(){
  digitalWrite(triggerPin,LOW);
  delay(2);    
  digitalWrite(triggerPin,HIGH);
  delay(10); 
  digitalWrite(triggerPin,LOW);
  distancia=(ultrasonic.Ranging(CM)); //func 'Ranging' converte o tempo de resposta para CM 
}


void loop() {
  // Mede a umidade a cada x segundos. Uma hora = 3600 segundos.
  for(int i=1; i<=150; i++) {
    // Posiciona o cursor do LCD na coluna 0 linha 1 (Obs: linha 0 eh a primeira)
    lcd.setCursor(0, 1);
    // Faz a leitura do sensor de umidade do solo
    umidadeSolo = analogRead(pinoSensor);
    // Converte a variação do sensor de 0 a 1023 para 0 a 100
    convert = map(umidadeSolo, 1023, 0, 0, 100);
    // Somatório de todos os valores lidos no periodo de tempo (para melhor acuracia do valor lido)
    umidadeReal = umidadeReal + convert;
    // Exibe a mensagem no Display LCD:
    lcd.print(String("Umidade: ") + String(umidadeReal/i) + String("% ") + String(i) + String("s"));
    delay(1000);
  }

  // Conversão para unico valor
  // divisão pelo mesmo valor de iter do laço 
  umidadeReal = umidadeReal/150.0;

  // verificação se o reservatório possui agua
  //float water = analogRead(pinAgua);
  //Serial.println(water);
  //int water = sonar.ping_cm();
  //Serial.println(water);

  
  hcsr04(); //chamada do metodo 'hcsr04()'
  Serial.println(distancia);
    

  if(distancia > 12){
    lcd.setCursor(0, 1);
    // Exibe a mensagem no Display LCD:
    lcd.print("  > SEM AGUA! <  ");
    // Aciona o buzzer na frequencia relativa ao 'Dó' em Hz x vezes
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
    lcd.setCursor(0, 1);
    // Exibe a mensagem no Display LCD:
    lcd.print("    Regando!    ");
    // Aciona o buzzer na frequencia relativa ao 'Mi' em Hz
    tone(pinBuzzer,329);      
    delay(500);
    noTone(pinBuzzer);
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
      lcd.setCursor(0, 1);
      // Exibe a mensagem no Display LCD:
      lcd.print("Solo Encharcado!");
      // Zera a variável como despejo de memória
      umidadeReal = 0.0;
      delay(2000);
    }
}
