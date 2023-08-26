#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 13); // RX, TX

#define PIN_RX 2
#define PIN_TX 3

String inputString = "";         // donde recibo los datos del softwareSerial
String outputString = "";	       // donde mando los datos al softwareSerial


//--------caracteres entrantes y clave-----------------//
char inputInt="";
char outputInt="";
int clave=45;
 
unsigned long previousmillis = 0;


//---------ejecutamos una vez----------------//
void setup() {
  Serial.begin(9600);
  //Por defecto el ultimo puerto inicializado esta escuchando, como solo usamos uno, no es ncesario usar la funcion listen() de software serial
  mySerial.begin (4800);	//probar a poner velocidades diferentes en cada punto.
  // reserve 200 bytes for the inputString:
  outputString.reserve(200);	//Maximo de 200 caracteres que podemos trabajar con este programa
  pinMode(PIN_RX,OUTPUT);
  pinMode(PIN_TX,OUTPUT); 
}


//---------ejecutamos varias veces----------------//
void loop()
{
   
  int tiempoLoop = millis() - previousmillis;
  previousmillis = millis();
  if (tiempoLoop > 250) {	//como los loops pueden ser largos, imprimo por pantalla si es mayor de 250 ms.
    Serial.print("Tiempo de Loop: ");
    Serial.println(tiempoLoop);
  }

  //------------inicalizo los String en cada loop------------//
  inputString = "";	
  outputString = "";

  //------------ Leo del serie en cada loop------------------//
  if (mySerial.available()) {
    digitalWrite(PIN_RX,HIGH);  //Enciendo el led de recepción de paquetes por puerto SW serial
    while (mySerial.available()) {	//Lo que entra por el serie conectado al otro Arduino
      //leo el nuevo caracter
      char inChar = (char)mySerial.read();
      delay(20);
      if (inChar == '\n') break;	//salgo del while si detecto un salto de linea si solo quisiera leer lineas
      int a = inChar;
      inputInt = a^clave; 
      inputString += inputInt;
      if (inputString.length() > 100) break;  
    }
    
    //Consola vecina
    digitalWrite(PIN_RX,LOW);
    Serial.println("Recibido: ");
    Serial.println(inputString); //imprimo el texto decodificado que se recibe
    
  }
     
  //---------------------- Leo de la consola en cada loop------------//
  if (Serial.available()) {
    while (Serial.available()) {
      char outChar = (char)Serial.read();
      delay(5);
      if (outChar == '\n') break;	//salgo del while si detecto un salto de linea.
      int a = outChar;
      outputInt = a^clave;
      outputString += outputInt;
    }
    Serial.println("Enviado: ");
    Serial.println(outputString); //imprimo el texto codificado que se envia
  
  digitalWrite(PIN_TX,HIGH);
	mySerial.print(outputString);
	digitalWrite(PIN_TX,LOW);
}
}