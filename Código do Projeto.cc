//Inclui as bibliotecas do SPI e do RC522
#include <SPI.h>
#include <MFRC522.h>
//Inclui a biblioteca do lcd
#include <LiquidCrystal.h>
/* 
  The circuit:
  * LCD RS pin to digital pin 8
  * LCD Enable pin to digital pin 7
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)
 *buzzer pin 6
*/
//Definição dos pinos de conexão do projeto
#define buzzer 8
#define PINO_SS 10
#define PINO_RST 9
//variável do lcd
LiquidCrystal lcd_1(7, 6, 5, 4, 3, 2);
//Cria o item para configurar o módulo RC522
MFRC522 mfrc522(PINO_SS, PINO_RST);
void setup()
{
  // Inicializa a serial
  Serial.begin(9600);
  // Inicializa a comunicação SPI
  SPI.begin();
  // Inicializa o módulo MFRC522
  mfrc522.PCD_Init();
//buzzer
pinMode(buzzer, OUTPUT);
lcd_1.begin(16,2);
 
  // Printa mensagem na serial
  Serial.println("Sistema Inicializado: Aproxime o token");
  lcd_1.print("APROXIME O TOKEN:");
}
void loop()
{
  // Aguarda a aproximação do token
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  // Seleciona qual token vai ser utilizado
  if (!mfrc522.PICC_ReadCardSerial()) return;
 
  // Mostra UID do token na serial
  Serial.print("UID da tag:");
  //Cria uma variável vazia, do tipo string
  String conteudo= "";
  //Cria uma variável do tipo byte
  byte letra;
 
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     if(mfrc522.uid.uidByte[i] < 0x10){
        Serial.print(" 0");
     }
     else{
        Serial.print(" ");
     }
     // Printa a mensagem convertida em hexadecimal
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     if(mfrc522.uid.uidByte[i] < 0x10){
        conteudo.concat(String(" 0"));
     }
     else{
        conteudo.concat(String(" "));
     }
     // Concatena a string convertida em hexadecimal
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
 
  Serial.println();
  Serial.print(" Mensagem : ");
 
  //Coloca todas as letras da string em maiúscula
  conteudo.toUpperCase();
 
  // Verifica se o conteúdo do cartão coincide
  if (conteudo.substring(1) == "D2 DF 74 1C"){   //cartão
    Serial.println("Acesso Permitido!");
    lcd_1.print("ACESSO PERMITIDO");
  tone(buzzer, 262);
delay(1000);
noTone(buzzer);
  } else if(conteudo.substring(1) == "01 DF 68 1D"){   //chaveiro
    Serial.println("Acesso Negado!");
    lcd_1.print("ACESSO NEGADO");
tone(buzzer, 494);
delay(1000);
noTone(buzzer);

  }
  delay(1000);
}
