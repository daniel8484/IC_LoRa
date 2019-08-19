/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

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
#include "heltec.h"
#include "images.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

//Declara localAddress que identifica o endereço deste dispositivo
byte localAddress = 0xBB;
//Declara destination que identifica o destino.
byte destination = 0xFD;      
//Declara msgCount, que será usado para guardar o tamanho da mensagem
//byte msgCount = 0;       
//Mensagem de saída     
String outgoing;
//Tempo para mandar 
long lastSendTime = 0;  
//Declara temporario que será usado para guardar valor temporario do tempo estimado para transmistir            
long temporario = 0;   

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.println("Iniciando...");
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();
  
  outgoing = "Hello World " + String(counter,DEC);
  //Retorna o número de milissegundos passados desde que a placa Arduino começou a executar o programa atual.
  sendMessage(outgoing);
 
  //Teste com LED
  //digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);                       // wait for a second
  //digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);                       // wait for a second
}
void sendMessage(String outgoing)
{
    // Manda o pacote
    LoRa.beginPacket();


  
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/



  //Adicionar endereço de destino
  LoRa.write(destination);  
  //Adicionar endereço do remetente            
  LoRa.write(localAddress);    
  //Adicionar o contador de bytes da mensagem         
  LoRa.write(counter);
  //Comprimento de msg de entrada              
  LoRa.write(outgoing.length());   

  
  //LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);


  LoRa.print(outgoing);
  
  LoRa.endPacket();
   //Tempo para mandar uma mensagem?
  if(counter==0){
   temporario = lastSendTime = millis();
  }
  else{
  lastSendTime =  millis() - temporario; 
  temporario = millis();
  }
  Serial.println("Tempo gasto " + String(lastSendTime, DEC ));
  Serial.println("mensagem" + outgoing);
  //Contador que aparece na mensagem, útil pra ver se todas estão chegando
  counter++;
  }
