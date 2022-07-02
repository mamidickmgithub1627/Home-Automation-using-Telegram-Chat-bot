#include<WiFi.h>
#include<WiFiClientSecure.h>
#include<UniversalTelegramBot.h>// Telegram chatbot library
#include<ArduinoJson.h>

const char* ssid="Mani_wifi"; //Wifi creditinals
const char* password="manikanta";

#define BOTtoken "2062725391:AAGAFHdjYK4hd-WKEfkDnam2bBo4ZgoTwV8"
//#define chatID 

const int GPIO_PIN=4;
bool GPIOState=LOW;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken,client);//defining bot

void handleNewMessages(int newMessagesCount){
  int i;
  for(i=0;i<newMessagesCount;i++){
    String chat_id=String(bot.messages[i].chat_id);
    /*if(chat_id !"){
      Serial.println("Unauthorized User!!!!!!");
      continue;
    }*/
    String messageText= bot.messages[i].text;
    String fromUser = bot.messages[i].from_name;
    String state ="";
    Serial.println("Message from "+fromUser+" : "+messageText);
    if(messageText=="/start"){
      String welcome = "Welcome "+fromUser+".\n";
      welcome+="/ON_LIGHT to ON BULB";
      welcome+="\n/OFF_LIGHT to OFF BULB";
      welcome+="\n/State to get current state of LED/n";
      bot.sendMessage(chat_id,welcome,"");
    }
    if(messageText=="/State"){
      String led_state=(GPIOState==HIGH?"ON":"OFF");
      state="The Bulb is "+led_state;
      bot.sendMessage(chat_id,state,"");
    }
    if(messageText=="/ON_LIGHT"){
      GPIOState=HIGH;
      digitalWrite(GPIO_PIN,GPIOState);
      state="The Bulb is turned ON";
      bot.sendMessage(chat_id,state,"");
    }
    if(messageText=="/OFF_LIGHT"){
      GPIOState=LOW;
      digitalWrite(GPIO_PIN,GPIOState);
      state="The Bulb is turned OFF";
      bot.sendMessage(chat_id,state,"");
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  pinMode(GPIO_PIN,OUTPUT);
  WiFi.begin(ssid,password);
  Serial.println("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status()==WL_CONNECTED)  {
    int newMessagesCount = bot.getUpdates(bot.last_message_received + 1);
    //Serial.println(newMessagesCount);
    while(newMessagesCount) {
      Serial.println("got response");
      handleNewMessages(newMessagesCount);
      newMessagesCount = bot.getUpdates(bot.last_message_received + 1);
    }
    //Serial.println("*");
    delay(500);
  }
}
