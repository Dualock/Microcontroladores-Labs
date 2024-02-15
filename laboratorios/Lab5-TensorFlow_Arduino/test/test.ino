void setup() {
  // Inicializar la comunicación serial con una velocidad de 9600 baudios
  Serial.begin(9600);
}

void loop() {
  // Imprimir un mensaje en el "Serial Monitor"
  Serial.println("¡Hola desde Arduino!");

  // Esperar un breve periodo de tiempo
  delay(1000);
}
