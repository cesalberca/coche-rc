int motoresIzquierdosAvanzar = 4;
int motoresIzquierdosRetroceder = 5;
int motoresDerechosAvanzar = 6;
int motoresDerechosRetroceder = 7;

int ledEncendido = 8;
int ledIzq = 9;
int ledDcha = 10;
int claxon = 11;
int trig = 12;
int echo = 13;
float distancia;

boolean modoAutomatico = false;

void setup() {
  Serial.begin(9600);
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
}

void loop() {
  //encenderCoche();
  //apagarLeds();
  girarCocheIzq();  
  //pararCoche();
  //  pitar();
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
    if(medirDistancia() > 0.20) {
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
    digitalWrite(motoresIzquierdosRetroceder, HIGH);
    digitalWrite(motoresDerechosRetroceder, HIGH);
  }

  /**
   * Función para hacer que el coche se pare.
   */
  void pararCoche() {
    encenderLedsParada();
    digitalWrite(motoresIzquierdosAvanzar, LOW);
    digitalWrite(motoresDerechosAvanzar, LOW);
    digitalWrite(motoresIzquierdosRetroceder, LOW);
    digitalWrite(motoresDerechosRetroceder, LOW);
  }

  /**
   * Función para hacer que el coche gire hacia la izquierda.
   */
  void girarCocheDcha() {
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
    encenderLedIzq();
    if (giroCerrado()) {
      digitalWrite(motoresDerechosAvanzar, HIGH);
      digitalWrite(motoresIzquierdosRetroceder, HIGH);  
    } else {
      digitalWrite(motoresDerechosAvanzar, HIGH);
      digitalWrite(motoresIzquierdosRetroceder, LOW);  
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

  void pitar() {
    analogWrite(claxon,100);
  }









   
