/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>

// datos de tu red Wifi 
const char* ssid     = "Guzman";
const char* password = "guzman321";



//variables que les debes asignar un valor para que se muestren en la pagina web

//ip que debes entrar cuando te conectes al esp32 = 192.168.4.1
int frecuenciaCardiaca;
int spo;
float temperatura;

WiFiServer server(80);//puerto de escucha del servidor

void setup()
{
    Serial.begin(9600);
    pinMode(5, OUTPUT);   

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.softAP("paul", "12345678"); // Nombre y contraseña del punto de acceso del esp32
    WiFi.begin(ssid, password);//nombre y contra del wifi de mi casa
    

    while (WiFi.status() != WL_CONNECTED) {//salimos del cilo hasta que se aya conectado al wifi
        delay(50);
        Serial.print(".");
    }

    Serial.println("\nWifi conectado.");
    Serial.println("Direccion ip: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   //declaramos el objecto cliente para escuchar

  if (client) {                             // if you get a client,
    Serial.println("Cliente nuevo");           // print a message out the serial port
    String currentLine = "";                // guarda la peticion del cliente
    
    while (client.connected()) { //si el cliente esta conectado entonces realizamos la escucha de sus peticiones
      if (client.available()) {// si el clinente realizo una peticion entonces la procesamos
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          if (currentLine.length() == 0) {// es para la peticion directa a la puerta de enlace (ip que le dio el router al esp32 o la ip local del esp32)
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            String html = "";
            html += "<!DOCTYPE html>";
            html += "<html lang=\"en\">";
            html += "<head>";
            html += "    <meta charset=\"UTF - 8\">";
            html += "    <meta http-equiv=\"X - UA - Compatible\" content=\"IE = edge\">";
            html += "    <meta name=\"viewport\" content=\"width = device - width, initial - scale = 1.0\">";
            html += "    <title>Servidor signos vitales</title>";

            html += "<style>";
            html+="html,body{width:100%;height:100%; display:flex; background:black;}";
            html+=".contenedor{";
            html+="width:500px;";
            html+="height:500px;";
            html+="background:#397aab;";
            html+="left:0; right:0; top:0; bottom:0; margin:auto;";
            html+="color:#ffffff;";
            html+="display:flex; justify-content:center; align-items:center; flex-direction: column;";
            html+="font-size:25px;";
            html+="border-radius: 10px;";
            html+="font-family: 'Roboto', sans-serif;";
            html+="";
            html+="}";
            html+="@import url('https://fonts.googleapis.com/css2?family=Roboto:wght@100&display=swap');";
            html += " </style>";
            html += "</head>";
            html += "<body id='body1'>";

            html+="<div class='contenedor'>";
            html+="<h2>Signos Vitales</h2>";
            html+="<h3>Frecuencia Cardiaca:"+(String)frecuenciaCardiaca+"</h3>";
            html+="<h3>SPO2:"+(String)spo+"</h3>";
            html+="<h3>Temperatura:"+(String)temperatura+"</h3>";
            html+="<h4>IP en red:"+WiFi.localIP().toString()+"</h4>";
            html+="</div>";
            
            
            html += "</body> ";
            html += "</html> ";

            // the content of the HTTP response follows the header:
            client.print(html);

            //terminamos la respuesta hacia el cliente con una linea en blanco
            client.println();
            break;//salimos del while
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // si el ususario en el path nos pidio
        //ip/'path'
        //www.google.com <-- es la ip
        // ip/carros <-- 'carros es el pat'
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
