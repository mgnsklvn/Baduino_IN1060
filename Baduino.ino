#include <LiquidCrystal.h> // importerer bibliotek for LCD-skjerm

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sensorPin = 0; //TR36, tempmåler på pin A0
float tempC; // Variabel for grader i Celcius

const int knapp = 6; // Knappen er koblet til pin-6
int knappState; // Variabel som holder 

int val;  // Variabler som senere tar vare på avlesning...
int val2; // ...av 'knapp'

int Mode = 0; // Variabel som holder oversikt over hvilke modus maskinen befinner seg i

float getTemperature(int pin) {         // Gjør om analogt signal fra TMP36 til ekvivalent verdi i Celsius
  float temperatur = analogRead(pin);   // Denne formelen er hentet fra bruker "GioMac" på https://create.arduino.cc/
  float voltage = temperatur * 5.0;
  voltage = voltage / 1024.0;
  return ((voltage - 0.5) * 100);
}

//######################################################################
//######################################################################

void setup() {
  pinMode(7, OUTPUT); //Relay-pin
  
  pinMode(10, OUTPUT); //Grønn LED
  pinMode(9, OUTPUT); //Gul LED
  pinMode(8, OUTPUT); //Rød LED
  
  pinMode(knapp, INPUT);              // Knapp tar input-signal
  knappState = digitalRead(knapp);    //Variabel for å lagre hvilken tilstand knappen er i
  
  lcd.begin(16, 2);                   // Setter LCDen sine kolonner og rader

  tempC = getTemperature(sensorPin);  //Gir Celciusverdi fra TMP36 til tempC
  lcd.setCursor(0,0);
  lcd.print("Skru pa maskinen");      //Skriver ut på første(0) linje av lcd
  lcd.setCursor(0,1);
  lcd.print ("for a varme opp:");     //Skriver ut på andre(1) linje av lcd
  digitalWrite(8, HIGH);              //Rød LED lyser
}

//######################################################################
//######################################################################

void loop() {
  val = digitalRead(knapp);   // 'val' og 'val2' holder samme verdi. Når brukeren trykker inn knappen...
  delay(10);                  // ...lagrer den verdien i begge val-variablene med et kort opphold...
  val2 = digitalRead(knapp);  // ...for å sikre at brukeren trykket 'godt nok'.

  if (val == val2) {          // Hvis begge val-verdiene er like, sjekk videre ->
    if (val != knappState) {  // Om 'val' ikke har samme verdi som knappState
      if (val == LOW) {       // Gjør ingenting om knappen ikke er trykket
        if (Mode == 0) {      // Hvis brukeren har trykket og vi befinner oss i "Mode 0"...
          Mode = 1;           // ...sett Mode-verdien til 1
        } else {              //
          if (Mode == 1) {    // Hvis brukeren har trykket og vi befinner oss i "Mode 1"...
            Mode = 2;         // ...sett Mode-verdien til 2
          } else {            //
            if (Mode == 2) {  // Hvis brukeren har trykket og vi befinner oss i "Mode 2"...
              Mode = 0;       // ...sett Mode-verdien til 0.
            }
          }
        }
      }
    }
    knappState = val; // gir knappState verdien til 'val' etter alle if-sjekkene er ferdig
  }
  
  if (Mode == 0) { //Når Mode har verdien '0', er den i 
    tempC = getTemperature(sensorPin);  //Gir Celciusverdi fra TMP36 til tempC
    lcd.setCursor(0,0);
    lcd.print("Skru pa maskinen");      //Skriver ut på første linje av lcd
    lcd.setCursor(0,1);
    lcd.print ("for a varme opp:");     //Skriver ut på andre linje av lcd
    digitalWrite(8, HIGH);              //Rød LED lyser
  }
  
  if (Mode == 1) { // Når Mode har verdi '1', skrur den på relayen som lar boksen varmes opp
    lcd.clear(); // Fjerner det som allerede er på skjermen
    tempC = getTemperature(sensorPin); //Gir Celciusverdi fra TMP36 til tempC
    lcd.setCursor(0,0);
    lcd.print("Varmer opp...");       //Skriver ut på første linje av lcd
    lcd.setCursor(0,1);
    lcd.print("Temp: "); lcd.print (tempC, 1); lcd.print(" "); lcd.print((char)223); lcd.print("C"); //Skriver ut på andre linje av lcd
    delay(200); // 2 millesekunder mellom hver lesing
  
      if (tempC <= 30) { // Hvis temperaturen er mindre/lik 30 grader..
        digitalWrite(10, LOW); 
        digitalWrite(9, HIGH);  //Gul pin lyser
        digitalWrite(8, LOW);
        digitalWrite(7, HIGH);  //Relay skrus på
      } else {
        digitalWrite(10, HIGH); //Grønn pin lyser
        digitalWrite(9, LOW);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);   //Relay skrus av
      }
  }
  
  if (Mode == 2) { // Knappen er trykket inn 2 ganger
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);    //Rød pin lyser
    digitalWrite(7, LOW);     //Realy skrus av

    lcd.clear(); // Fjerner det som allerede er på skjermen
    lcd.setCursor(0,0);
    lcd.print("Maskinen skrur"); //Skriver ut på første linje av lcd
    lcd.setCursor(0,1);
    lcd.print("seg av..."); // Skriver ut på andre linje av lcd
  }
}
