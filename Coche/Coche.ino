#include <SoftwareSerial.h>

// Pines bluetooth
int txd = 2;
int rxd = 3;

// Pines motores
int motoresIzquierdosAvanzar = 7;
int motoresIzquierdosRetroceder = 6;
int motoresDerechosAvanzar = 5;
int motoresDerechosRetroceder = 4;

// Pines leds
int ledEncendido = 8;
int ledDcha = 9;
int ledIzq = 10;
int claxon = 11;

// Sensor distancia
int trig = 12;
int echo = 13;
float distancia;

// Variables de control
boolean modoAutomatico = false;
float distanciaMinima = 0.20;
char orden;
int randNum;
int delayGiro = 750;
boolean encontradoObstaculoAnteriormente;

SoftwareSerial BT(txd, rxd);

void setup() {
  BT.begin(9600);
  pinMode(trig, HIGH);
  pinMode(echo, INPUT);
  pinMode(ledEncendido, OUTPUT);
  pinMode(ledIzq, OUTPUT);
  pinMode(ledDcha, OUTPUT);
  pinMode(claxon, OUTPUT);
  pinMode(motoresIzquierdosAvanzar, OUTPUT);
  pinMode(motoresIzquierdosRetroceder, OUTPUT);
  pinMode(motoresDerechosAvanzar, OUTPUT);
  pinMode(motoresDerechosRetroceder, OUTPUT);
  randomSeed(analogRead(0)); 
}

void loop() { 
  // Si el modulo a manda dato, guardarlo en estado.
  if (BT.available()) {
    orden = BT.read();
    BT.println(BT.read());

    if (orden == '1') {
      avanzarCoche();      
    } else if (orden == '2') {
      retrocederCoche();
    } else if (orden == '3') {
      girarCocheIzq();
    } else if (orden == '4') {
      girarCocheDcha();
    } else if (orden == '5') {
      //retrocederCoche();
    } else if (orden == '6') {
      //retrocederCoche();
    } else if (orden == '7') {
      //retrocederCoche();
    } else if (orden == '8') {
      //retrocederCoche();
    } else if (orden == '9') {
      if(!modoAutomatico) {
        moverAI();
        modoAutomatico = true;
      } else {
        modoAutomatico = false;
      } 
    } else if (orden == 'a') {
      //retrocederCoche();
    } else if (orden == 'b') {
      pararCoche();
    } 
  } else {
    pararCoche();
  }
}
 
/* ============================*/
/* = Funciones de movimiento  =*/
/* ============================*/

/**
 * Función para hacer que el coche se mueva hacia delante.
 */
void avanzarCoche() {
  if(medirDistancia() > distanciaMinima) {
    apagarLeds();
    digitalWrite(motoresIzquierdosAvanzar, HIGH);
    digitalWrite(motoresDerechosAvanzar, HIGH);    
  } else {
    pararCoche();
  }
}

/**
 * Función para hacer que el coche se mueva hacia delante.
 */
void retrocederCoche() {
  digitalWrite(motoresIzquierdosAvanzar, LOW);
  digitalWrite(motoresDerechosAvanzar, LOW);
  digitalWrite(motoresIzquierdosRetroceder, HIGH);
  digitalWrite(motoresDerechosRetroceder, HIGH);
}

/**
 * Función para hacer que el coche se pare.
 */
void pararCoche() {
  encenderLedsParada();
  pararCocheIzq();
  pararCocheDcha();
}

/**
 * Función para parar los motores de la izq
 */
void pararCocheIzq() {
  digitalWrite(motoresIzquierdosAvanzar, LOW);
  digitalWrite(motoresIzquierdosRetroceder, LOW);
}

/**
 * Función para parar los motores de la dcha
 */
void pararCocheDcha() {
  digitalWrite(motoresDerechosAvanzar, LOW);
  digitalWrite(motoresDerechosRetroceder, LOW);
}

/**
 * Función para hacer que el coche gire hacia la izquierda.
 */
void girarCocheDcha() {
  apagarLeds();
  pararCocheIzq();
  encenderLedDcha();
  if (giroCerrado()) {
    digitalWrite(motoresIzquierdosAvanzar, HIGH);
    digitalWrite(motoresDerechosRetroceder, HIGH);
  } else {
    digitalWrite(motoresIzquierdosAvanzar, HIGH); 
    digitalWrite(motoresDerechosRetroceder, LOW); 
  }
}

/**
 * Función para hacer que el coche gire hacia la izquierda.
 */
void girarCocheIzq() {
  apagarLeds();
  pararCocheDcha();
  encenderLedIzq();
  if (giroCerrado()) {
    digitalWrite(motoresDerechosAvanzar, HIGH);
    digitalWrite(motoresIzquierdosRetroceder, HIGH);  
  } else {
    digitalWrite(motoresDerechosAvanzar, HIGH);
    digitalWrite(motoresIzquierdosRetroceder, LOW);  
  }
}

/**
 * Función modo automático. Detectará obstáculos e intentará evitarlos.
 */

void moverAI() {
  
  if(medirDistancia() > distanciaMinima) {
    avanzarCoche();
  } else {
    pararCoche();
    // Encontrado obstáculo. Decide en qué dirección girar aleatoriamente.
    if (!encontradoObstaculoAnteriormente) {
      randNum = random(1, 3);
      
      switch(randNum) {
        case 1:
          girarCocheIzq();
          break;
        case 2:
          girarCocheDcha();
          break;  
      }
      
      delay(delayGiro);
      pararCoche();
      encontradoObstaculoAnteriormente = true;
    } else {
      girarCocheIzq();
      delay(delayGiro * 2);
      encontradoObstaculoAnteriormente = false;
    }
    
    
  }
}

/* =========================*/
/* = Funciones de medición =*/
/* =========================*/

/**
 * Función para comprobar si se debe ejecutar un giroAbierto o un giroCerrado 
 */
boolean giroCerrado() {
  if(medirDistancia() < 0.20) {
    return true;
  } else {
    return false;
  }
}

/**
 * Función para determinar la distancia que hay entre el coche y un obstáculo.
 */
float medirDistancia() {
  //Inicializamos el sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(5);

  //Comenzamos las mediciones
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  distancia = pulseIn(echo, HIGH);
  distancia = distancia * 0.0001657;
  Serial.println(distancia);
  
  return distancia;
}

/* ======================*/
/* = Funciones de leds  =*/
/* ======================*/

/**
 * Función para encender el coche. Enciende el Led de Encendido.
 */
void encenderCoche() {
  digitalWrite(ledEncendido, HIGH);
}

/**
 * Función para apagar el coche. Apaga el Led de Encendido.
 */
void apagarCoche() {
  digitalWrite(ledEncendido, LOW);
}

/**
 * Función para encender led intermitente izquierdo.
 */
void encenderLedIzq() {
  digitalWrite(ledIzq, HIGH);
}

/**
 * Función para encender led intermitente derecho.
 */
void encenderLedDcha() {
  digitalWrite(ledDcha, HIGH);
}

/**
 * Función para apagar leds.
 */
void apagarLeds() {
  digitalWrite(ledIzq, LOW);
  digitalWrite(ledDcha, LOW);
}

/**
 * Función para encender los leds de parada.
 */
 void encenderLedsParada() {
  encenderLedIzq();
  encenderLedDcha();
 }

/* ==========================*/
/* = Funciones de sensores  =*/
/* ==========================*/

/**
 * Función para hacer que el claxon pite
 */
void pitar() {
  analogWrite(claxon, 100);
}  
