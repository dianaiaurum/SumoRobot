//\ibraries
#include <LiquidCrystal_I2C.h>
#include <Sabertooth.h>

//config modul telecomanda
#define rc_pin 3

//config LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//config joystick
#define in_x A3
#define in_sw 9

//config driver
Sabertooth ST(128);

//config senzor linie
#define in_qtr_st A1
#define in_qtr_dr A2
int qrt_dr;
int qrt_st;

//config senzori inamici
#define in_s1 10
#define in_s2 11
#define in_s3 12

int sw_state, s1, s2, s3;
int m_st = 1;
int m_dr = 2;
int rc;
int* s;
void setup() {
//remote
pinMode(rc_pin, INPUT);

//lcd
lcd.init();
lcd.clear();
lcd.backlight();

//joystick
pinMode(in_x, INPUT);
pinMode(in_sw, INPUT_PULLUP);

//driver
SabertoothTXPinSerial.begin(115200);

//senzori linie
pinMode(in_qtr_st, INPUT);
pinMode(in_qtr_dr, INPUT);

//senzori inamici
pinMode(in_s1, INPUT);
pinMode(in_s2, INPUT);
pinMode(in_s3, INPUT);

}

void loop() {

  rc = digitalRead(rc_pin);
  while (rc == 0) {
    ST.motor(1, 0);
    ST.motor(2, 0);
    rc = digitalRead(rc_pin);
  }   
  while (rc == 1){
    ST.motor(1, 127);
    ST.motor(2, 127);
    rc = digitalRead(rc_pin);
    }
 
}

/*
//interfata principala
void menu(){
  lcd.clear();

  int x = analogRead(in_x);
  x = map(x, 0, 1023, -512, 512);
  sw_state = digitalRead(in_sw);

  lcd.setCursor(1, 0);
  lcd.print("Test");
  lcd.setCursor(10, 0);
  lcd.print("Fight mode");

  int opt = 0;

  lcd.setCursor(opt, 0);
  lcd.print(">");

  while(sw_state == 1){
    x = analogRead(in_x);
    x = map(x, 0, 1023, -512, 512);
    sw_state = digitalRead(in_sw);
    if (x > 200){
      lcd.setCursor(0, 0);
      lcd.print(" ");
      opt = 9;
    }
    if (x < -200){
      lcd.setCursor(9, 0);
      lcd.print(" ");
      opt = 0;
    }
    lcd.setCursor(opt, 0);
    lcd.print(">");
  }
/*
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (opt == 0){
    test_mode();
  }
  else{
    remote_start();
    fight_mode();
  }
}

void test_mode(){
  sw_state = digitalRead(in_sw);
  int x = analogRead(in_x);
  x = map(x, 0, 1023, -512, 512);
  while (sw_state == 1){
    //test senzori linie - sub 100 -> alb, peste 600 -> negru
    lcd.clear();
    qrt_dr = analogRead(A2);
    qrt_st = analogRead(A1);
    lcd.setCursor(0, 0);
    lcd.print("dr:");
    lcd.print(qrt_dr);
    lcd.setCursor(8, 0);
    lcd.print("st:");
    lcd.print(qrt_st);
    //test senzori inamici
    int s1 = digitalRead(in_s1);
    int s2 = digitalRead(in_s2);
    int s3 = digitalRead(in_s3);
    lcd.setCursor(0,1);
    lcd.print("s1:");
    lcd.print(s1);
    lcd.setCursor(5,1);
    lcd.print("s2:");
    lcd.print(s2);
    lcd.setCursor(10,1);
    lcd.print("s3:");
    lcd.print(s3);
    //test motoare
    int x = analogRead(in_x);
    x = map(x, 0, 1023, -127, 127);
    ST.motor(1, x);
    ST.motor(2, x);
    sw_state = digitalRead(in_sw);
    delay(100);
    }

  menu();
  
}

void remote_start(){
  rc = digitalRead(rc_pin);
  while (rc == 0){
    ST.motor(1, 0);
    ST.motor(2, 0);
    rc = digitalRead(rc_pin);
  }
  }
*/
void remote_stop(){
  rc = digitalRead(rc_pin);
  while (rc == 0){
    exit(0);
    rc = digitalRead(rc_pin);
  }
  }
/*
void fight_mode(){
  while(true){
    s = s_inamici();
    unsigned long StartTime = millis();
    while(s[0] == 0 && s[1] == 0 && s[2] == 0){
      unsigned long CurrentTime = millis();
      unsigned long ElapsedTime = CurrentTime - StartTime;
      linie_alba();
      remote_stop();
      if (ElapsedTime > 100){
        spin(30, 1);//1 - cw, 0 -ccw
      }
      else{
        patrol(60);
      }
      }
    target(90);
    bull();
    remote_stop();
  }
  }

void forward(int p){    //mergi drept inainte
  ST.motor(1, p);
  ST.motor(2, p);
  remote_stop();
}
*/
void arc(int p, int fact){  //curba - fact neg/poz in functie de directie
  ST.motor(1, p + fact);
  ST.motor(2, p - fact);
  remote_stop();
}
/*
void spin(int p, bool clockwise){
  if (clockwise){
    ST.motor(1, p);
    ST.motor(2, -p);
  }
  else{
    ST.motor(1, -p);
    ST.motor(2, p);
  }
  remote_stop();
}

void rotate(int p, bool cw){
  if (cw){
    ST.motor(1, p);
    ST.motor(2, -p);
  }
  else{
    ST.motor(1, -p);
    ST.motor(2, p);
  //sa vedem cat timp ii ia sa se invarta un anumit nr de grade
  }
  remote_stop();
}
   */
void intoarcere_2miscari(int p, int fact){
    arc(p, fact);
    arc(p, -fact);
    remote_stop();
}
/*
void stop(){
    ST.motor(1, 0);
    ST.motor(2, 0);
    remote_stop();
    }

void patrol(int p){
  ST.motor(1, p);
  ST.motor(2, p);
  remote_stop();
}

//senzori*/
void linie_alba(){  //Cand se ajunge la linie alba - under100 -white; over600 - black ()
  qrt_dr = analogRead(A2);
  qrt_st = analogRead(A1);
  int position = qrt_dr + qrt_st; 
  //am atins linia alba
  while(position < 200){
      //mergi inapoi forta majora
      qrt_dr = analogRead(A2);
      qrt_st = analogRead(A1);
      position = qrt_dr + qrt_st; 
      
      remote_stop();
      }
 }

/*
int s_inamici(){   //senzori inamici
  s1 = digitalRead(in_s1);
  s2 = digitalRead(in_s2);
  s3 = digitalRead(in_s3);
  int* s[3] = {s1, s2, s3};
  return s;
}

//functii
void target(int p){
  s = s_inamici();
  while (s[0] == 1 && s[1] == 0 && s[2] == 0){ //inamicul e in dreapta, roteste-te pana ajungi cu fata la el
    rotate(p, 1);
    s = s_inamici();
    remote_stop();
  }
  while (s[0] == 0 && s[1] == 0 && s[2] == 1){ //inamicul e in stanga, roteste-te pana ajungi cu fata la el
    rotate(p, -1);
    s = s_inamici();
    remote_stop();
  }
  while (s[1] == 1 && (s[0] == 0 || s[2] == 0)){ //inamicul e in fata, du-te in el
    forward(p);
    s = s_inamici();
    remote_stop();
  }
}

void bull(){
  while (s[0] == 1 && s[1] == 1 && s[2] == 1){
    ST.motor(1, 127);
    ST.motor(2, 127);
    s = s_inamici();
    remote_stop();
  }
}
*/
