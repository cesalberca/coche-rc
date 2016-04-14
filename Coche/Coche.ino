int motoresIzquierdosAvanzar = 4;
int motoresIzquierdosRetroceder = 5;
int motoresDerechosAvanzar = 6;
int motoresDerechosRetroceder = 7;

int ledEncendido = 8;
int trig = 10;
int echo = 11;
float distancia;


void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledEncendido, OUTPUT);
  pinMode(motoresIzquierdosAvanzar, OUTPUT);
  pinMode(motoresIzquierdosRetroceder, OUTPUT);
  pinMode(motoresDerechosAvanzar, OUTPUT);
  pinMode(motoresDerechosRetroceder, OUTPUT);
}

void loop() {
  encenderCoche();
  girarCocheDcha();  
  //avanzarCoche();
  //digitalWrite(motoresIzquierdosRetroceder, HIGH);
  //digitalWrite(motoresIzquierdosAvanzar, HIGH);
  //digitalWrite(motoresDerechosRetro, HIGH);
 }
 
  /* ============================*/
  /* = Funciones de movimiento  =*/
  /* ============================*/

  /**
   * Función para hacer que el coche se mueva hacia delante.
   */
  void avanzarCoche() {
    digitalWrite(motoresIzquierdosAvanzar, HIGH);
    digitalWrite(motoresDerechosAvanzar, HIGH);
  }

  /**
   * Función para hacer que el coche se mueva hacia delante.
   */
  void retrocederCoche() {
    digitalWrite(motoresIzquierdosRetroceder, HIGH);
    digitalWrite(motoresDerechosRetroceder, HIGH);
  }

  /**
   * Función para hacer que el coche se pare.
   */
  void pararCoche() {
    digitalWrite(motoresIzquierdosAvanzar, LOW);
    digitalWrite(motoresDerechosAvanzar, LOW);
    digitalWrite(motoresIzquierdosRetroceder, LOW);
    digitalWrite(motoresDerechosRetroceder, LOW);
  }

  /**
   * Función para hacer que el coche gire hacia la izquierda.
   */
  void girarCocheDcha() {
    if (giroCerrado()) {
      digitalWrite(motoresIzquierdosAvanzar, HIGH);
      digitalWrite(motoresDerechosRetroceder, HIGH);
    } else {
      digitalWrite(motoresIzquierdosAvanzar, HIGH);  
    }
  }

  /**
   * Función para hacer que el coche gire hacia la izquierda.
   */
  void girarCocheIzq() {
    if (giroCerrado()) {
      digitalWrite(motoresDerechosAvanzar, HIGH);
      digitalWrite(motoresIzquierdosRetroceder, HIGH);  
    } else {
      digitalWrite(motoresDerechosAvanzar, HIGH);
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
  float  medirDistancia() {
    //Inicializamos el sensor
    digitalWrite(trig, LOW);
    delayMicroseconds(5);

    //Comenzamos las mediciones
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    distancia = pulseIn(echo, HIGH);
    distancia = distancia * 0.0001657;
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
   * Función para encender los leds de parada.
   */
   void encenderLedsParar() {
    encenderLedIzq();
    encenderLedDcha();
   }









   
