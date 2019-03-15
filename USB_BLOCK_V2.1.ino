#include <SPI.h>        // Biblioteca do modulo SPI para comunicacao com o RC522
#include <MFRC522.h>  // Biblioteca para Mifare RC522

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Cria a instancia MFRC522.

#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON HIGH
#define LED_OFF LOW
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

//===== Intervalo================
unsigned long IntervaloLiberado = 480000;  //DEFINIDO O TEMPO QUE O USB VAI FICAR LIBERADO


 //480000   aqui é 8 minutos

//Cartões Liberados

String Card1 = "44 72 A0 EA";
String Card2 = "B4 C4 AC EA";
String Card3 = "76 6D 1F 07";
String Card4 = "34 7A A7 EA";
String Card5 = "54 C7 53 FF";
String Card6 = "75 FD 7F 63";
String Card7 = "75 DC 10 46";
String Card8 = "A6 DB 2B 07";
String Card9 = "F5 BE 82 45";
String Card10 = "96 35 2E 07";
String Card11 = "E6 BD 1A 07";
 
//================================
//===== Timer ===================
unsigned long TempoLido=0;              //VARIAVEL DE TESTE DE TEMPO


#define led 7    // Definicao do pino do LED

#define relay 8     // Definicao do pino do Rele

boolean unlocked = false;

int successRead;    //Variavel para verificar se um cartao foi lido com sucesso

String readCard= "";   // Armazena a ID lida do cartao pelo Modulo RFID



///////////////////////////////////////// Setup Inicial da placa ///////////////////////////////////
void setup() {

  //Configuracao dos pinos do Arduino
  pinMode(led, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);    // certificando que o USB esteja bloqueado
  digitalWrite(led, LED_OFF); // Certificando que o Led esteja desligado

  //Configuracao de Protocolos
  //Serial.begin(9600);  // Inicia a comunicacao Serial com o Computador
  SPI.begin();           // MFRC522 usa o protocolo SPI
  mfrc522.PCD_Init();    // inicializa o Modulo MFRC522

  //Seta a potencia da antena no maximo
 // mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

//   Serial.println("Aproxime o seu cartao do leitor...");
//  Serial.println();

}

///////////////////////////////////////// Loop Principal ///////////////////////////////////
void loop () {

  do {
    //Testa se o tempo de liberacao e maior que o definido , se for, bloqueia o USB
  unsigned long TempoAtual = millis();
 if ((unsigned long)(TempoAtual - TempoLido)>= IntervaloLiberado && unlocked == true) {
   bloquear();
   TempoLido=TempoAtual;
  }
    successRead = getID();  // A funcao GetID retorna um se um cartao foi lido com sucesso
  }
  while (!successRead);   //repete ate que um cartao seja lido com sucesso
  
if (readCard.substring(1) == Card1 || readCard.substring(1) == Card2 || readCard.substring(1) == Card3 || readCard.substring(1) == Card4 || readCard.substring(1) == Card5 || readCard.substring(1) == Card6 || readCard.substring(1) == Card7 || readCard.substring(1) == Card8 || readCard.substring(1) == Card9 || readCard.substring(1) == Card10  || readCard.substring(1) == Card11){
        granted();         // Libera o USB
      }
      else {      //Se nao for conhecido, nega acesso.
        denied();
      }
         delay (500); 
}


/////////////////////////////////////////  Access Granted    ///////////////////////////////////
void granted () {
  //iniciar o Timer para o tempo de acesso
  TempoLido = millis();
  digitalWrite(relay, HIGH);  // liga o rele para liberar o USB
  digitalWrite(led, LED_ON);   // Acende o led para mostrar que o USB esta liberado
  delay(100);
  digitalWrite(led, LED_OFF);
  delay(100);
  digitalWrite(led, LED_ON);
  unlocked = true;
 // Serial.print("Livre");
 // Serial.println();
  readCard = "";
  }

///////////////////////////////////////// Access Denied  ///////////////////////////////////
//Esta funcao pisca o led 3 vezes para mostrar que o acesso foi negado.
void denied() {
  digitalWrite(led, LED_ON);   // Liga o LEd
  delay(200);
  digitalWrite(led, LED_OFF);  // Desliga o Led
  delay(200);
  digitalWrite(led, LED_ON);  
  delay(200);
  digitalWrite(led, LED_OFF);  
  delay(200);
  digitalWrite(led, LED_ON);  
  delay(200);
  digitalWrite(led, LED_OFF);
  if ( unlocked == true ){
      digitalWrite(led, LED_ON);
  }
 // Serial.print("Negado");
 // Serial.println();
  readCard = "";
}


///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
int getID() {
 // mfrc522.PCD_Init();    // inicializa o Modulo MFRC522
  // Le o cartao
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //Se um cartao for lido, continua
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Assim que o cartao for lido, pega o serial e continua
    return 0;
  }

  //Mostra UID na serial
  //Serial.print("UID da tag :");
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     readCard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     readCard.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
 // Serial.println();
  readCard.toUpperCase();
  
 // mfrc522.PICC_HaltA(); // Para a leitura
return 1;
}

void bloquear(){
   digitalWrite(led, LED_OFF);
    digitalWrite(relay, LOW);
    unlocked = false;
 //   Serial.print("Bloqueado");
 //   Serial.println();
}
