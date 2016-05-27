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
int ledEncendido= 1;
int laser = 8;
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
int delayLaser = 1000;
char orden;
int randNum;
int delayGiro = 350;
boolean encontradoObstaculoAnteriormente = false;
boolean laserEncendido = false;

//Constantes notas
#define  C0      16.35
#define Db0 17.32
#define D0  18.35
#define Eb0 19.45
#define E0  20.60
#define F0  21.83
#define Gb0 23.12
#define G0  24.50
#define Ab0 25.96
#define LA0 27.50
#define Bb0 29.14
#define B0  30.87
#define C1  32.70
#define Db1 34.65
#define D1  36.71
#define Eb1 38.89
#define E1  41.20
#define F1  43.65
#define Gb1 46.25
#define G1  49.00
#define Ab1 51.91
#define LA1 55.00
#define Bb1 58.27
#define B1  61.74
#define C2  65.41
#define Db2 69.30
#define D2  73.42
#define Eb2 77.78
#define E2  82.41
#define F2  87.31
#define Gb2 92.50
#define G2  98.00
#define Ab2 103.83
#define LA2 110.00
#define Bb2 116.54
#define B2  123.47
#define C3  130.81
#define Db3 138.59
#define D3  146.83
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Gb3 185.00
#define G3  196.00
#define Ab3 207.65
#define LA3 220.00
#define Bb3 233.08
#define B3  246.94
#define C4  261.63
#define Db4 277.18
#define D4  293.66
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Gb4 369.99
#define G4  392.00
#define Ab4 415.30
#define LA4 440.00
#define Bb4 466.16
#define B4  493.88
#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32
#define Eb7 2489.02
#define E7  2637.02
#define F7  2793.83
#define Gb7 2959.96
#define G7  3135.96
#define Ab7 3322.44
#define LA7 3520.01
#define Bb7 3729.31
#define B7  3951.07
#define C8  4186.01
#define Db8 4434.92
#define D8  4698.64
#define Eb8 4978.03
// DURATION OF THE NOTES 
#define BPM 120     // you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

SoftwareSerial BT(txd, rxd);

void setup() {
  BT.begin(9600);
  pinMode(trig, HIGH);
  pinMode(echo, INPUT);
  pinMode(laser, OUTPUT);
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
      if (laserEncendido) {
        apagarLaser();
        laserEncendido = false;
      } else {
        encenderLaser();  
        laserEncendido = true;
      }
    } else if (orden == '6') {
      pitar();
    } else if (orden == '7') {
      sonar();
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
    moverAI();
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
    digitalWrite(laser, HIGH);
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
  modoAutomatico = true;
  
  // Avanzar coche en caso que no haya obstaculos.
  if(medirDistancia() > distanciaMinima) {
    avanzarCoche();
    apagarLaser();
  } else {
    encenderLaser();
    pararCoche();
    // Encontrado obstáculo. Decide aleatoriamente en qué dirección girar.
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

      // Girar al lado contrario durante el doble de tiempo.
      switch(randNum) {
        case 1:
          girarCocheDcha();
          break;
        case 2:
          girarCocheIzq();
          break;  
      }
      
      delay(delayGiro * 2);
      pararCoche();
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
  if (!modoAutomatico) {
    if(medirDistancia() < distanciaMinima) {
      return true;
    } else {
      return false;
    }  
  } else {
    return true;
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

void encenderLaser() {
  digitalWrite(laser, HIGH);
}

void apagarLaser() {
  digitalWrite(laser, HIGH);
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

void sonar(){
  tone(11,LA3,Q); 
  delay(1+Q);
  tone(11,LA3,Q);
  delay(1+Q);
  tone(11,LA3,Q);
  delay(1+Q);
  tone(11,F3,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  
  tone(11,LA3,Q);
  delay(1+Q);
  tone(11,F3,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  tone(11,LA3,H);
  delay(1+H);
  
  tone(11,E4,Q); 
  delay(1+Q); 
  tone(11,E4,Q);
  delay(1+Q);
  tone(11,E4,Q);
  delay(1+Q);
  tone(11,F4,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  
  tone(11,Ab3,Q);
  delay(1+Q);
  tone(11,F3,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  tone(11,LA3,H);
  delay(1+H);
  
  tone(11,LA4,Q);
  delay(1+Q);
  tone(11,LA3,E+S);
  delay(1+E+S);
  tone(11,LA3,S);
  delay(1+S);
  tone(11,LA4,Q);
  delay(1+Q);
  tone(11,Ab4,E+S);
  delay(1+E+S);
  tone(11,G4,S);
  delay(1+S);
  
  tone(11,Gb4,S);
  delay(1+S);
  tone(11,E4,S);
  delay(1+S);
  tone(11,F4,E);
  delay(1+E);
  delay(1+E);
  tone(11,Bb3,E);
  delay(1+E);
  tone(11,Eb4,Q);
  delay(1+Q);
  tone(11,D4,E+S);
  delay(1+E+S);
  tone(11,Db4,S);
  delay(1+S);
  
  tone(11,C4,S);
  delay(1+S);
  tone(11,B3,S);
  delay(1+S);
  tone(11,C4,E);
  delay(1+E);
  delay(1+E);
  tone(11,F3,E);
  delay(1+E);
  tone(11,Ab3,Q);
  delay(1+Q);
  tone(11,F3,E+S);
  delay(1+E+S);
  tone(11,LA3,S);
  delay(1+S);
  
  tone(11,C4,Q);
  delay(1+Q);
   tone(11,LA3,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  tone(11,E4,H);
  delay(1+H);
  
   tone(11,LA4,Q);
  delay(1+Q);
  tone(11,LA3,E+S);
  delay(1+E+S);
  tone(11,LA3,S);
  delay(1+S);
  tone(11,LA4,Q);
  delay(1+Q);
  tone(11,Ab4,E+S);
  delay(1+E+S);
  tone(11,G4,S);
  delay(1+S);
  
  tone(11,Gb4,S);
  delay(1+S);
  tone(11,E4,S);
  delay(1+S);
  tone(11,F4,E);
  delay(1+E);
  delay(1+E);//PAUSE
  tone(11,Bb3,E);
  delay(1+E);
  tone(11,Eb4,Q);
  delay(1+Q);
  tone(11,D4,E+S);
  delay(1+E+S);
  tone(11,Db4,S);
  delay(1+S);
  
  tone(11,C4,S);
  delay(1+S);
  tone(11,B3,S);
  delay(1+S);
  tone(11,C4,E);
  delay(1+E);
  delay(1+E);
  tone(11,F3,E);
  delay(1+E);
  tone(11,Ab3,Q);
  delay(1+Q);
  tone(11,F3,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  
  tone(11,LA3,Q);
  delay(1+Q);
   tone(11,F3,E+S);
  delay(1+E+S);
  tone(11,C4,S);
  delay(1+S);
  tone(11,LA3,H);
  delay(1+H);
  
  delay(2*H); 
}
