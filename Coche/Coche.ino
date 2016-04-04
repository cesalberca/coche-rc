int motorDelanteIzq = 4;
int motorDelanteDcha = 5;
int motorTrasIzq = 6;
int motorTrasDcha = 7;
int ledEncendido = 8;
int trig = 10;
int echo = 11;
float distancia;


void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledEncendido, OUTPUT);
}

void loop() {
  encenderCoche();
  avanzarCoche();  
}

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
   * Función para hacer que el coche se mueva hacia delante.
   */
  void avanzarCoche() {
    digitalWrite(motorDelanteIzq, HIGH);
    delay(1000);
    digitalWrite(motorDelanteIzq, LOW);
  }
  
  /**
   * Función para determinar la distancia que hay entre el coche y un obstáculo.
   */
  void medirDistancia() {
    //Inicializamos el sensor
    digitalWrite(trig, LOW);
    delayMicroseconds(5);

    //Comenzamos las mediciones
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    distancia = pulseIn(echo, HIGH);
    distancia = distancia * 0.0001657;

    if (distancia < 0.10)
    {
      // parar();
      Serial.println("Parar");
    }
  }
