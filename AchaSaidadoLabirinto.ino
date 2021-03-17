#include <Servo.h>
#include <NewPing.h>

// Sonar Frente
#define trig_pinF A0
#define echo_pinF A1
// Sonar Direito
#define trig_pinD A2
#define echo_pinD A3
// Sonar Esquerdo
#define trig_pinE A4
#define echo_pinE A5

// Leitura Máxima do Sensor HC-SR04
#define maximum_distance 400

// Distância Padrão
int distanciafrente = 43;
int distanciaesquerda = 25;
int distanciadireita = 25;

NewPing sonarfrente(trig_pinF, echo_pinF, maximum_distance);
NewPing sonardireito(trig_pinD, echo_pinD, maximum_distance);
NewPing sonaresquerdo(trig_pinE, echo_pinE, maximum_distance);

//motor 1
int IN1 = 4 ;
int IN2 = 5 ;
int ena = 3;

//motor 2
int IN3 = 6 ;
int IN4 = 7 ;
int enb = 11;

//variavel auxiliar
int velocidade = 255;

// Variaveis para as distancias
int espacofrente;
int espacoesquerda;
int espacodireita;

// Tempo do Giro
int timegiro90=600;
int timegiro180=900;

//Inicializa Pinos
void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
}

void loop() {
  espacoesquerda=readPingE();
  if (espacoesquerda==0){
    espacofrente=350;
  }
  espacofrente=readPingF();
  if (espacofrente==0){
    espacofrente=350;
  }
  espacodireita=readPingD();
  if (espacodireita==0){
    espacofrente=350;
  }  
  if (espacofrente<=distanciafrente){
     if (espacoesquerda >= distanciaesquerda && espacodireita >= distanciadireita){
        // Parede na frente, mas as laterais livres.        
        Serial.println("Detectada Parede apenas na frente, as laterais estão livres: gire à esquerda 90 graus ::: Linha 71");
        giraesquerda();
        delay(timegiro90);
     } else if (espacoesquerda > distanciaesquerda && espacodireita < distanciadireita ){
            // parede na frente e parede ao lado direito
            Serial.println("Detectada Parede na frente e ao lado direito: dobrar à esquerda 90 graus ::: Linha 75");
            dobraesquerda();
            //delay(250);
     } else if (espacoesquerda <= distanciaesquerda && espacodireita >= distanciadireita ){
            // parede na frente e parede ao lado esquerdo
            Serial.println("Detectada Parede na frente e ao lado esquerdo: dobrar à direita 90 graus ::: Linha 79");    
            dobradireita();
            //delay(250);
     } else {
            // parede na frente, ao lado direito e esquerdo
            Serial.println("Beco sem saída detectado: Girar 180 graus e retornar ::: Linha 83");
            giraesquerda();
            delay(timegiro180);
     }
   } else {
        if (espacoesquerda >= distanciaesquerda && espacofrente >= distanciafrente && espacodireita >= distanciadireita ){
            // Tudo livre
            Serial.println("Não foi detectada nenhum objeto próximo aos sensores: siga em frente ::: Linha 89");     
            frente();
        } else if (espacoesquerda <= distanciaesquerda && espacodireita <= distanciadireita ){
            // detectado nos dois lados parede, mas a frente está livre
            Serial.println("Detectada parede dos dois lados, a frente está livre: siga em frente ::: Linha 93");     
            frente();
        } else if(espacoesquerda >= distanciaesquerda && espacodireita <= distanciadireita ){
            // detectado apenas uma parede ao lado direito, a frente está livre
            Serial.println("Detectada parede ao lado direito, a frente está livre: siga em frente ::: Linha 97");     
            frente();
        } else {
            // detectado apenas uma parede ao lado esquerdo, mas a frente está livre
            Serial.println("Detectada parede ao lado esquerdo, a frente está livre: siga em frente ::: Linha 101");     
            frente();
        }
   }
} 
int readPingE() {
  int sonarE = sonaresquerdo.ping_cm();
  delay(250);  
  Serial.print("Esquerda: ");
  Serial.print(sonarE);
  return sonarE;
}
int readPingF() {
  int sonarF = sonarfrente.ping_cm();
  delay(250);  
  Serial.print(" Frente: ");
  Serial.print(sonarF);
  return sonarF;
}
int readPingD() {
  int sonarD = sonardireito.ping_cm();
  delay(250);
  Serial.print(" Direta: ");
  Serial.println(sonarD);
  return sonarD;
}
void parar(){
  analogWrite(ena, 0);
  analogWrite(enb, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
} // 0 0 0 0
void frente() {
  analogWrite(ena, velocidade);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(enb, velocidade);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
} // 1 0 1 0
void re(){
  analogWrite(ena, velocidade);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  analogWrite(enb, velocidade);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
} // 0 1 0 1
void dobradireita() {
  //analogWrite(ena, velocidade-100);
  analogWrite(ena, 175);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(enb, velocidade);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
} // 1 0 0 1 com pwm reduzido
void dobraesquerda() {
  analogWrite(ena, velocidade);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  //analogWrite(enb, velocidade-100);
  analogWrite(enb, 175);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
} // 0 1 1 0 com pwm reduzido
void giraesquerda() {
  analogWrite(ena, velocidade);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(enb, velocidade);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
} // 1 0 0 1
void giradireita() {
  analogWrite(ena, velocidade);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(enb, velocidade);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
} // 0 1 1 0

