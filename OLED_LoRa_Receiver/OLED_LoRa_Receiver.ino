/*
  This is a simple example show the Heltec.LoRa recived data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
//Bibliotecas
#include "heltec.h" 
#include "images.h"
//Define a variável BAND, que vai representar a frequência usada.
#define BAND    915E6  
//Declara rssi, que vai representar o RSSI do sinal.
String rssi = "RSSI --";
//Declara packSize, que vai representar o tamanho do pacote.
String packSize = "--";
//Declara packet, que vai representar o pacote.
String packet ;


byte localAddress = 0xBB;     // address of this device


/*
 * Declara um método para mostrar uma imagem na tela do ESP32
 */
void logo(){
  //Limpa o display
  Heltec.display->clear();
  //Xbm é o tipo de extensão. Acretdio que seja (coluna para iniciar, linha para iniciar,largura da imagem, altura da imagem, sequência hexadecimal)
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  //Escreve no Display a imagem.
  Heltec.display->display();
}

void LoRaData(){
  //Limpa o display
  Heltec.display->clear();
  //Seta o alinhamento do texto
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  //Seta a fonte 
  Heltec.display->setFont(ArialMT_Plain_10);// ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24. Or create one with the font tool at http://oleddisplay.squix.ch
  //'Pretende' escrever uma linha no Display. (coluna para iniciar, linha para iniciar,texto). Caso utrapasse o espaço da linha começa a escrever em cima do que já tinha
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  //'Pretende' escrever até o máximo passado, 128 neste caso. (coluna para iniciar, linha para iniciar, largura máxima, texto). Se o texto não couber na linha pula para as próximas linhas.
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
   //'Pretende' escrever uma linha no Display. (coluna para iniciar, linha para iniciar,texto). Caso utrapasse o espaço da linha começa a escrever em cima do que já tinha
  Heltec.display->drawString(0, 0, rssi); 
   //'Pretende' escrever uma linha no Display. (coluna para iniciar, linha para iniciar,texto). Caso utrapasse o espaço da linha começa a escrever em cima do que já tinha 
   //LoRa.packetSnr() pega o SNR do sinal
   Heltec.display->drawString(0, 40,"SNR: " + String(LoRa.packetSnr(), DEC));  
  //Escreve no Display os textos acima da forma definida
  Heltec.display->display();
}

void cbk(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length


  
  //Seta um valor nulo para packet
  packet ="";
  //Seta o valor de packSize
  //fornece a String que é a representação hexadecimal do valor de packetSize. 
  packSize = String(packetSize,DEC);
  //Enquanto i < packetSize - 4. -4 Por causa dos 4 bytes lidos acima, temos:
  for (int i = 0; i < packetSize - 4 ; i++) { 
    //Lê byte por byte e cria uma concaternação. No final terá todo o texto
    //O mesmo que packet = packet + (char) LoRa.read();
    packet += (char) LoRa.read(); 
    }


    

  //if (incomingLength != packet.length())
  //{   // check length for error
    //Serial.println("error: message length does not match length" + String(packet.length(), DEC));
    // skip rest of function
    //return;                             
  //}
  

  // if the recipient isn't this device or broadcast,
  //if (recipient != localAddress && recipient != 0xFF) {
    //Serial.println("This message is not for me.");
    // skip rest of function
    //return;                             
  //}



  // if message is for this device, or broadcast, print details:
  
  //Serial.println("Received from: 0x" + String(sender, DEC));
  //Serial.println("Sent to: 0x" + String(recipient, DEC));
  Serial.println("Received from: 0x" + String(sender, BIN));
  Serial.println("Sent to: 0x" + String(recipient, BIN));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + packet);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();





  
  //Define o valor de rssi
  //rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  //Chama o método LoRaData()
  //LoRaData();
}
//Executa quando inicia o programa
void setup() { 
  //Inicia o módulo 
  //O que seria Enable, Serial e PABOOST?
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  //Inicializa o display
  Heltec.display->init();
  //Mudou a orientação vertical. flip = virar.
  Heltec.display->flipScreenVertically();  
  //Seta a fonte 
  Heltec.display->setFont(ArialMT_Plain_10);
  //Chama o método logo()
  logo();
  //Espera 1500ms
  delay(1500);
  //Limpa o Display
  Heltec.display->clear();
  //'Pretende' escrever uma linha no Display. (coluna para iniciar, linha para iniciar,texto). Caso utrapasse o espaço da linha começa a escrever em cima do que já tinha 
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  //'Pretende' escrever uma linha no Display. (coluna para iniciar, linha para iniciar,texto). Caso utrapasse o espaço da linha começa a escrever em cima do que já tinha 
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  //Escreve no Display os textos acima da forma definida
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  //Coloca o rádio no modo de recepção contínua
  //LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  //Espera 10ms
  delay(10);
}
