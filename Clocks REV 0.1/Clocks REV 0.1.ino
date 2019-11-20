#include <FlexiTimer2.h>
#include <PinChangeInt.h>
#include <avr/power.h>

int MinL = 0, MinR= 0, HrsL = 0, HrsR = 0;
int rght = 3, lft = 4;
int ShwnTm = 240;
bool TmUpdt = false, TmChange = false, Chk = false;
int Disch = 0;
int TmShw = 0;




void setup() {
  ADCSRA &= ~(1 << ADEN); 
  power_usart0_disable();
  power_twi_disable();  
  pinMode(6, INPUT);     
  //digitalWrite(6, LOW);
  PCintPort::attachInterrupt(6, TimeShow,RISING); 
  
  FlexiTimer2::set(60500, 1.0/1000, timerInterupt);
  FlexiTimer2::start();
  TCCR0B = TCCR0B & 0b11111000 | 0x01;  
  pinMode(5, OUTPUT);  
  pinMode(rght, OUTPUT);
  pinMode(lft, OUTPUT);

  pinMode(15, OUTPUT);
  pinMode(17, OUTPUT); 
  pinMode(18, OUTPUT); 
  pinMode(16, OUTPUT); 
  pinMode(0, OUTPUT); 
}

void tabl(int numr = 0){ 
  switch(numr){
    case 0: 
      digitalWrite(15, LOW); 
      digitalWrite(17, LOW); 
      digitalWrite(18, LOW); 
      digitalWrite(16, LOW);      
    break;
    case 1:
      digitalWrite(15, HIGH); 
      digitalWrite(17, LOW); 
      digitalWrite(18, LOW); 
      digitalWrite(16, LOW);   
    break;
    case 2:
      digitalWrite(15, LOW); 
      digitalWrite(17, HIGH); 
      digitalWrite(18, LOW); 
      digitalWrite(16, LOW); 
    break;
    case 3:
      digitalWrite(15, HIGH); 
      digitalWrite(17, HIGH); 
      digitalWrite(18, LOW); 
      digitalWrite(16, LOW); 
    break;
    case 4:
      digitalWrite(15, LOW); 
      digitalWrite(17, LOW); 
      digitalWrite(18, HIGH); 
      digitalWrite(16, LOW); 
    break;
    case 5:
      digitalWrite(15, HIGH); 
      digitalWrite(17, LOW); 
      digitalWrite(18, HIGH); 
      digitalWrite(16, LOW); 
    break;
    case 6:
      digitalWrite(15, LOW); 
      digitalWrite(17, HIGH); 
      digitalWrite(18, HIGH); 
      digitalWrite(16, LOW); 
    break;
    case 7:
      digitalWrite(15, HIGH); 
      digitalWrite(17, HIGH); 
      digitalWrite(18, HIGH); 
      digitalWrite(16, LOW); 
    break;
    case 8:
      digitalWrite(15, LOW); 
      digitalWrite(17, LOW); 
      digitalWrite(18, LOW); 
      digitalWrite(16, HIGH); 
    break;
    case 9:
      digitalWrite(15, HIGH); 
      digitalWrite(17, LOW); 
      digitalWrite(18, LOW); 
      digitalWrite(16, HIGH); 
    break;   
  }  
}

void  timerInterupt() {    
  TmUpdt = true;
}

void TimeShow(){  
  noInterrupts();   
  TmShw++;  
  if(TmShw >= 3)
    Disch++;  
  //digitalWrite(6, LOW);
  delay(500);
  interrupts();  
} 

void HOURS(int a = 0){
  for(int i = 0; i < a; i++)
    {
      digitalWrite(lft, HIGH);
      tabl(HrsL);
      delay(12); 
      digitalWrite(lft, LOW);         
      digitalWrite(rght, HIGH);
      tabl(HrsR);
      delay(12); 
      digitalWrite(rght, LOW);      
    } 
}
void MINUTES(int a = 0){
  for(int i = 0; i < a; i++)
    {    
      digitalWrite(lft, HIGH);
      tabl(MinL);
      delay(12); 
      digitalWrite(lft, LOW);     
      digitalWrite(rght, HIGH);
      tabl(MinR);
      delay(12); 
      digitalWrite(rght, LOW);     
    }         
  }  


void loop() {  
   if(TmShw >= 1){
    if(Chk == false)
    {
      Chk = true;         
      digitalWrite(0, HIGH);          
      analogWrite(5, 165); // Переменная ШИМ
      delay(2000);       
      digitalWrite(rght, HIGH);
      for(int p = 0; p < 10; p++){
        tabl(p); 
        delay(500); 
      }    
      digitalWrite(rght, LOW);           
      digitalWrite(lft, HIGH);
      for(int p = 0; p < 10; p++){
        tabl(p); 
        delay(500); 
      }
      digitalWrite(lft, LOW); 
    }       
  
  if(TmShw >= 2){      
    switch(Disch){
      case 1:
        if(HrsL < 2)
          HrsL++;
        else
          HrsL = 0;
        HOURS(700);        
        break;
      case 2:
        if(HrsR < 3 && HrsL == 2 || HrsR < 9 && HrsL < 2)
          HrsR++;
        else
          HrsR = 0;
        HOURS(700); 
        break;
      case 3:
        if(MinL < 5)
          MinL++;
        else
          MinL = 0;
        MINUTES(700); 
        break;   
      case 4:
        if(MinR < 9)
          MinR++;
        else
          MinR = 0;
        MINUTES(700); 
        break;       
    }
    if(Disch == 5){  
      TmChange = false;
      Disch = 0;
      analogWrite(5, 0);
      digitalWrite(0, LOW);      
      TmShw = 0;  
      Chk = false;     
    }       
  }

  if(TmShw == 1){       
    delay(2000);          
    //Hours
    HOURS(ShwnTm);
    delay(2000);  
    //Minutes
    MINUTES(ShwnTm);
    analogWrite(5, 0);
    digitalWrite(0, LOW); 
    TmShw = 0; 
    Chk = false;        
  } 
}
  if(TmUpdt){
    MinR++;
    if(MinR == 10) {
      MinR = 0;
      MinL++;
    }
    if(MinL == 6) {
      MinL = 0;
      HrsR++;
    } 
    if(HrsR == 10) {
      HrsR = 0;
      HrsL++;
    } 
    if(HrsL == 2 && HrsR == 4) {
      HrsL = 0;
      HrsR = 0;
    }
    TmUpdt = false;
  }     
}
