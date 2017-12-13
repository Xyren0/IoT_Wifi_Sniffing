/*
This example creates a client object that connects and transfers
data using always SSL.

It is compatible with the methods normally related to plain
connections, like client.connect(host, port).

Written by Arturo Guadalupi
last revision November 2015

*/
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <SPI.h>
#include <WiFi101.h>

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

char ssid[] = "FRITZ!Box Fon WLAN 7390";        // your network SSID (name)
char pass[] = "3302665823231966";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.iot.blackreset.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

char buffer[512];
uint32_t timer = 0;
String s = "a";
int logo_r = -1;
int logo_g = -1;
int logo_b = -1;
String old_celsius;
int mode = 3;

void setup() {
  
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setCursor(0, 8);
  matrix.setRotation(2);
  matrix.setTextSize(2);
  matrix.setTextColor(matrix.Color444(15, 0, 0));
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  WiFi.setPins(53, 4, 3, -1);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to wifi");
  
  printWiFiStatus();
}

void loop() {
  updateText();
  delay(500);

  String s = String(buffer);
  s = filterHead(s);
  setMode(s);
  
  
  //Serial.println(s);
  if(mode == 0){ //Weather
      displayWeather(s);
  }
  if(mode  == 1){ //Notes
      displayNote(s); 
  }
  if(mode  == 2){ //Light
      displayColor(s);
  }
  if(mode == 3){ //Logo
    displayLogo(s);
  }
  if(mode == 4){
    displayChess(s);
  }
}


void updateText(){
  uint32_t now = millis();
  if (now > timer) {
    timer = now + 1000;
  } else {
    return;  
  }
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  
  WiFiSSLClient client;
  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /matrix.dev.php HTTP/1.1");
    client.println("Host: www.iot.blackreset.com");
    client.println("Connection: close");
    client.println();
    // if there are incoming bytes available
    // from the server, read them and print them:
    char *ptr = buffer;
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
        *(ptr++) = c;
      }
    }
    *ptr = '\0';
    
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
      
  
  } else {
    Serial.println("Connection failed");
  }
}

void setMode(String json){
  int j = json.indexOf(":");
  //Serial.println((int)json.charAt(j+1)-48);
  int oldmode = mode;
  mode = (int)json.charAt(j+1)-48;
  if(oldmode != mode){
    matrix.fillRect(0,0,32,32,matrix.Color444(0,0,0));
  }
  
}

String filterHead(String all){
  int j = all.indexOf("{");
  String res = all.substring(j);
  Serial.println(res);

  return res;
}

unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}


void displayColor(String co){
  //json 
    int j = co.indexOf("#");
    String  rs = (String)(((String)co.charAt(j+1)) + co.charAt(j+2));
    String  gs = (String)(((String)co.charAt(j+3)) + co.charAt(j+4));
    String  bs = (String)(((String)co.charAt(j+5)) + co.charAt(j+6));
    Serial.println(hexToDec(rs)/16);
    Serial.println(gs);
    Serial.println(bs);
    int r = hexToDec(rs)/16;
    int g = hexToDec(gs)/16;
    int b = hexToDec(bs)/16;
  matrix.fillScreen(matrix.Color444(r, g, b));
}

void displayWeather(String we){
   //json parsing here
    int j = we.indexOf(":");
    String res = we.substring(j+3);
    j = res.indexOf(":");
    int j2 = res.indexOf(",");
    Serial.println("celsius");
    String celsius = res.substring(j+1, j2);
    
    Serial.println(celsius);
    j = we.indexOf("#");
    String  rs = (String)(((String)we.charAt(j+1)) + we.charAt(j+2));
    String  gs = (String)(((String)we.charAt(j+3)) + we.charAt(j+4));
    String  bs = (String)(((String)we.charAt(j+5)) + we.charAt(j+6));
    Serial.println(hexToDec(rs)/16);
    Serial.println(gs);
    Serial.println(bs);
    int r = hexToDec(rs)/16;
    int g = hexToDec(gs)/16;
    int b = hexToDec(bs)/16;
    matrix.setCursor(6, 6);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color444(15, 15, 15));
    if(celsius!=old_celsius){
      old_celsius=celsius;
       matrix.fillRect(0,0,15,15, matrix.Color444(0, 0, 0));
    }
    matrix.print(celsius);
    matrix.drawLine(18, 6, 18, 8, matrix.Color444(r, g, b));
    matrix.drawLine(20, 6, 20, 8, matrix.Color444(r, g, b));
    matrix.drawPixel(19, 6, matrix.Color444(r, g, b));
    matrix.drawPixel(19, 8, matrix.Color444(r, g, b));
    matrix.setCursor(22, 6);
    matrix.print("C");
    matrix.fillRect(0,17,32,32, matrix.Color444(r, g, b));
}

void displayNote(String n){
  //json
  int j = n.indexOf(":");
  String res = n.substring(j+3);
  j = res.indexOf(":");
  String note = res.substring(j+2);
  j = res.indexOf("}");
  note = note.substring(0,j-6);
  note.trim();
  Serial.println(note);
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color444(15, 15, 15));
  int len = note.length();
  Serial.println(len);
  int x = 0;
  while(mode == 1){
    delay(200);
    matrix.fillRect(0,0,32,32, matrix.Color444(0, 0, 0));
    matrix.setCursor(31-x, 12);
    matrix.print(note);
    x = x+6;
    if(x>(len * 6)+31){
      Serial.print("len: ");
      Serial.println(len);
      Serial.println("overflow");
      Serial.println(x);
      x = 0;
      mode = 0;
      
      //s = checkWeb();
      //s = filterHead(s);
      //setMode(s);
      //json
      //int j = s.indexOf(":");
      //String res = s.substring(j+3);
      //j = res.indexOf(":");
      //String note = res.substring(j+2);
      //j = res.indexOf("}");
      //note = note.substring(j-2);
      //len = s.length();
    }
  }
}

void displayLogo(String co){
  int j = co.indexOf("#");
    String  rs = (String)(((String)co.charAt(j+1)) + co.charAt(j+2));
    String  gs = (String)(((String)co.charAt(j+3)) + co.charAt(j+4));
    String  bs = (String)(((String)co.charAt(j+5)) + co.charAt(j+6));
    Serial.println(hexToDec(rs)/16);
    Serial.println(gs);
    Serial.println(bs);
    int r = hexToDec(rs)/16;
    int g = hexToDec(gs)/16;
    int b = hexToDec(bs)/16;
    if(r!=logo_r || g != logo_g ||b!= logo_b){
      logo_r = r;
      logo_g = g;
      logo_b = b;
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.drawLine(2, 2, 29, 29, matrix.Color444(r, g, b));
      matrix.drawLine(29, 2, 2, 29, matrix.Color444(r, g, b));
  
  
      matrix.drawLine(0, 10, 31, 10, matrix.Color444(r, g, b));
      matrix.drawLine(10, 0, 10, 31, matrix.Color444(r, g, b));
  
      matrix.drawLine(0, 21, 31, 21, matrix.Color444(r, g, b));
      matrix.drawLine(21, 0, 21, 31, matrix.Color444(r, g, b));
  
  
      matrix.drawLine(13, 2, 29, 18, matrix.Color444(r, g, b));
      matrix.drawLine(2, 13, 18, 29, matrix.Color444(r, g, b));
  
      matrix.drawLine(2, 18, 18, 2, matrix.Color444(r, g, b));
      matrix.drawLine(29, 13, 13, 29, matrix.Color444(r, g, b));
  
  
      matrix.drawPixel(10, 10, matrix.Color333(0, 0, 0));
  
      matrix.drawPixel(10, 21, matrix.Color333(0, 0, 0));
  
      matrix.drawPixel(21, 21, matrix.Color333(0, 0, 0));
  
      matrix.drawPixel(21, 10, matrix.Color333(0, 0, 0));
      matrix.drawPixel(15, 15, matrix.Color444(r, g, b));
      matrix.drawPixel(15, 16, matrix.Color444(r, g, b));
      matrix.drawPixel(16, 16, matrix.Color444(r, g, b));
      matrix.drawPixel(16, 15, matrix.Color444(r, g, b));
      }

}

void displayChess(String co){
  int j = co.indexOf("#");
  String res = co.substring(j+1);
  drawField(res);
}

void drawField(String field){
  //int fieldint[8][8][3];
  Serial.println("field:");
  Serial.println(field);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {  
      /*String rs = (String)(((String)field.charAt(i*3+j*3*8) + field.charAt(i*3+j*3*8)));
      String gs = (String)(((String)field.charAt((i*3+j*3*8)+1) + field.charAt((i*3+j*3*8)+1)));
      String bs = (String)(((String)field.charAt((i*3+j*3*8)+2) + field.charAt((i*3+j*3*8)+2)));*/
      int r= (int)field.charAt((i*4)+(j*4*8))-48;
      int g= (int)field.charAt(((i*4)+(j*4*8))+1)-48;
      int blue= (int)field.charAt(((i*4)+(j*4*8))+2)-48;
      int side= (int)field.charAt(((i*4)+(j*4*8))+3)-48; 
      if(side >0){
        

        matrix.drawPixel(i*4+1,j*4+1,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+2,j*4+1,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+1,j*4+2,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+2,j*4+2,matrix.Color444(r,g,blue));
      }else{
        matrix.drawPixel(i*4+1,j*4+1,matrix.Color444(0,0,0));
        matrix.drawPixel(i*4+2,j*4+1,matrix.Color444(0,0,0));
        matrix.drawPixel(i*4+1,j*4+2,matrix.Color444(0,0,0));
        matrix.drawPixel(i*4+2,j*4+2,matrix.Color444(0,0,0));
      }
      //Serial.println(r);
      //Serial.println(g);
      //Serial.println(blue);
      matrix.drawPixel(i*4+1,j*4,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+2,j*4,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4,j*4+1,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4,j*4+2,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+3,j*4+1,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+3,j*4+2,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+1,j*4+3,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4+2,j*4+3,matrix.Color444(r,g,blue));
      matrix.drawPixel(i*4,j*4,matrix.Color444(r,g,blue));
        matrix.drawPixel(i*4,j*4+3,matrix.Color444(r,g,blue));
        matrix.drawPixel(i*4+3,j*4,matrix.Color444(r,g,blue));
        matrix.drawPixel(i*4+3,j*4+3,matrix.Color444(r,g,blue));
      //Serial.println("x,y: "+i+" "+j+" "+(String)hexToDec(rs)/16+" "+(String)hexToDec(gs)/16+" "+(String)hexToDec(bs)/16);
      /*matrix.drawPixel(i*4+1,j*4+1,matrix.Color444(hexToDec(rs)/16,hexToDec(gs)/16,hexToDec(bs)/16));
      matrix.drawPixel(i*4+2,j*4+1,matrix.Color444(hexToDec(rs)/16,hexToDec(gs)/16,hexToDec(bs)/16));
      matrix.drawPixel(i*4+1,j*4+2,matrix.Color444(hexToDec(rs)/16,hexToDec(gs)/16,hexToDec(bs)/16));
      matrix.drawPixel(i*4+2,j*4+2,matrix.Color444(hexToDec(rs)/16,hexToDec(gs)/16,hexToDec(bs)/16));*/
      //matrix.fillRect(i*4,j*4,4,4,matrix.Color444(hexToDec(rs)/16,hexToDec(gs)/16,hexToDec(bs)/16));
    }
  }
}
void printWiFiStatus() {
  
  uint32_t x = WiFi.localIP();
  uint8_t *p = reinterpret_cast<uint8_t*>(&x);
  sprintf(buffer, "%d.%d.%d.%d\n", *p, *(p+1), *(p+2), *(p+3));
  Serial.print("IP Address: ");
  Serial.write(buffer, strlen(buffer));
  
  x = WiFi.gatewayIP();
  p = reinterpret_cast<uint8_t*>(&x);
  sprintf(buffer, "%d.%d.%d.%d\n", *p, *(p+1), *(p+2), *(p+3));
  Serial.print("Gateway: ");
  Serial.write(buffer, strlen(buffer));
  
  x = WiFi.subnetMask();
  p = reinterpret_cast<uint8_t*>(&x);
  sprintf(buffer, "%d.%d.%d.%d\n", *p, *(p+1), *(p+2), *(p+3));
  Serial.print("Subnet Mask: ");
  Serial.write(buffer, strlen(buffer));
  
  long y = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(y);
  Serial.println("dBm");
}