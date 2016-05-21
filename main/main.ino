#include <rgb_lcd.h>

#define LEDS 7
#define PROTOCOLS 3
#define TAB_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define BUZZER_LIMIT 10

/*
 * Prototypes
 */
void led(String s); 
void buzzer(String s); 
void message(String s);
 

/*
 * LEDS 
 * 
 * 30 & 31 : Door A [Red, Green]
 * 40 & 41 : Door B [Red, Green]
 * 50 & 51 & 52 : Door Prison [Red, Green, Orange]
 */
int ledsBridge[] = {3, 4, 5};
int leds[LEDS] = {30, 31, 40, 41, 50, 51, 52};
int ledsState[LEDS] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW};



/*
 * BUZZER
 */
int buzzerModule = 4;
int buzzerState = LOW;
int buzzerCounter = 0;



/*
 * LCD
 */
rgb_lcd lcd;
const int red = 255;
const int green = 0;
const int blue = 0;




/*
 * Protocols
 * 
 * LED
 *  [LED DOOR STATUS]
 *  String "LED"
 *  Int DOOR : 1, 2, 3
 *  Int Status : 1, 2, 3, ...
 * 
 * --
 * BUZZER
 *  [BUZZER]
 *  String "BUZZER"
 * 
 * --
 * MESSAGE
 *  [MESSAGE STR]
 *  String "MESSAGE"
 *  String STR : String of the message
 */
 
struct s_protocols
{
   String  flag;
   void   (*p)(String s); // pointeur sur fonction retournant un int et en prenant 2 en parametres.
};
typedef struct s_protocols t_protocols;

t_protocols protocols[PROTOCOLS] = {
   {"LED", led},
   {"BUZZER", buzzer},
   {"MESSAGE", message},
};


String str;
String params;
String protocol;



void setup() {
  Serial.begin(9600);
  int i = 0;

  //leds
  for(i = 0; i < LEDS; i++) 
    pinMode(leds[i], OUTPUT);
    
  // buzzer
  pinMode(buzzerModule, OUTPUT);

  // screen
  lcd.begin(16, 2);
  lcd.setRGB(red, green, blue);
  lcd.print("Control loading...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait");
}

void loop() {
  for(int i = 0; i < LEDS; i++)
    digitalWrite(leds[i], ledsState[i]);
  digitalWrite(buzzerModule, buzzerState);
  
  // buzzer limit
  if(buzzerState == HIGH) buzzerCounter++;
  if(buzzerCounter == BUZZER_LIMIT) buzzerState = LOW;
  
  delay(100);
  
  for(int i = 0; i < LEDS; i++)
    digitalWrite(leds[i], LOW);
  digitalWrite(buzzerModule, LOW);
  
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    str = Serial.readString();
    protocol = str.substring(0, str.indexOf(' '));
    params = str.substring(str.indexOf(' ') + 1);
    
    int i = 0;
    if((i = isInPtrArray(protocols, PROTOCOLS, protocol)) > -1) {
        protocols[i].p(params);
    } else Serial.println("Le protocole n'existe pas.");      
  }
}

/* 
 * LED
 *  [LED DOOR STATUS]
 *  String "LED"
 *  Int DOOR : 1, 2, 3
 *  Boolean Status : true, false (1, 0)
 */
void led(String s) {
  if(s.length() == 0) {
    Serial.println("Aucun parametre n'a ete renseigne.");
    return;
  }
    
  int tmpDoor = s.substring(0, s.indexOf(' ')).toInt();
  int door = ledsBridge[tmpDoor - 1];
  int value = s.substring(s.indexOf(' ') + 1).toInt();

  int j = 0;
  
  if((j = isInArray(leds, LEDS, (door * 10) + value)) > -1) {
    (ledsState[j] == LOW) ? ledsState[j] = HIGH : ledsState[j] = LOW;
  } else Serial.println("Cette LED n'existe pas.");
}

void buzzer(String s) {
  buzzerCounter = 0;
  if(buzzerState == LOW)
    buzzerState = HIGH;
  else
    buzzerState = HIGH;
}

void message(String s) {
  if(s.length() == 0) {
    Serial.println("Aucun parametre n'a ete renseigne.");
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("###################");
  lcd.setCursor(0, 1);
  lcd.print("###################");

  delay(200);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(s);
}


/*
 * Tools
 */
int isInPtrArray(t_protocols arr[], int n, String str) {
  for(int i = 0; i < n; i++)  {
    if(arr[i].flag.compareTo(str) == 0)
      return i;
  }
  return -1;
}

int isInArray(int arr[], int n, int value) {
  for(int i = 0; i < n; i++)  {
    if(arr[i] == value) {
      return i;
    }
  }
  return -1;
}



