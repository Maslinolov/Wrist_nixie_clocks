#include <avr/power.h>
#include <avr/sleep.h>

int MinL = 0, MinR= 0, HrsL = 0, HrsR = 0;
int rght = 3, lft = 4;
int ShwnTm = 120;
bool TmUpdt = false, TmChange = false, Chk = false;
int Disch = 0;
int TmShw = 0;
int val=0;                      
boolean flag=0; 
int btn = 0;
int twosec = 0; 




void setup() {
  TCCR2A = 0x00; 
  TCCR2B = 0x05; 
  TIMSK2 = 0x01; 
  ASSR = 0x20; 
  ADCSRA &= ~(1 << ADEN); 
  power_usart0_disable();
  power_twi_disable();  
  attachInterrupt(0, ButInter, RISING); 
  
  TCCR0B = TCCR0B & 0b11111000 | 0x02;  
  pinMode(5, OUTPUT);  
  pinMode(rght, OUTPUT);
  pinMode(lft, OUTPUT);

  pinMode(15, OUTPUT);
  pinMode(17, OUTPUT); 
  pinMode(18, OUTPUT); 
  pinMode(16, OUTPUT); 
  pinMode(0, OUTPUT);  
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_mode();
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

ISR(TIMER2_OVF_vect){
    twosec  += 1;
    if(twosec == 60){  
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
      twosec = 0;
    }  
}

void HOURS(int a = 0){
  for(int i = 0; i < a; i++)
    {
      digitalWrite(lft, HIGH);
      tabl(HrsL);
      delay(4); 
      digitalWrite(lft, LOW);
      delay(4);         
      digitalWrite(rght, HIGH);
      tabl(HrsR);
      delay(4); 
      digitalWrite(rght, LOW);  
      delay(4);    
    } 
}
void MINUTES(int a = 0){
  for(int i = 0; i < a; i++)
    {    
      digitalWrite(lft, HIGH);
      tabl(MinL);
      delay(4); 
      digitalWrite(lft, LOW);
      delay(4);      
      digitalWrite(rght, HIGH);
      tabl(MinR);
      delay(4); 
      digitalWrite(rght, LOW); 
      delay(4);     
    }         
  }  

void ButInter(){
    btn++;
  }

void loop() {
  if(btn > 0 && btn <= 2){
    btn = 0;
    val++;    
    delay(7500);     
  }
  if(btn >= 3 && btn <= 50){
    val++;  
    btn = 0;   
  }
  
  if(val == 1){
    TmShw = 1;
    val = 0;
  }
  if(val >= 2){
    TmShw = 2;
    Disch = val;    
  }  
    
   if(TmShw >= 1){
    if(Chk == false)
    {
      Chk = true;         
      digitalWrite(0, HIGH);          
      analogWrite(5, 155); // Переменная ШИМ
      delay(500);       
      digitalWrite(rght, HIGH);
      for(int p = 0; p < 10; p++){
        tabl(p); 
        delay(250); 
      }    
      digitalWrite(rght, LOW);           
      digitalWrite(lft, HIGH);
      for(int p = 0; p < 10; p++){
        tabl(p); 
        delay(250); 
      }
      digitalWrite(lft, LOW);       
    }       
  
  if(TmShw >= 2){      
    switch(Disch){
      case 2:
        if(HrsL < 2)
          HrsL++;
        else
          HrsL = 0;
        HOURS(400);        
        break;
      case 3:
        if(HrsR < 3 && HrsL == 2 || HrsR < 9 && HrsL < 2)
          HrsR++;
        else
          HrsR = 0;
        HOURS(400); 
        break;
      case 4:
        if(MinL < 5)
          MinL++;
        else
          MinL = 0;
        MINUTES(400); 
        break;   
      case 5:
        if(MinR < 9)
          MinR++;
        else
          MinR = 0;
        MINUTES(400); 
        break;       
    }
    if(Disch >= 6){  
      TmChange = false;
      Disch = 0;
      analogWrite(5, 0);
      digitalWrite(0, LOW);      
      TmShw = 0;  
      Chk = false;  
      val = 0;  
      btn = 0; 
      sleep_mode();     
    }       
  } 

  if(TmShw == 1){       
    delay(500);          
    //Hours
    HOURS(ShwnTm);
    delay(500);  
    //Minutes
    MINUTES(ShwnTm);
    analogWrite(5, 0);
    digitalWrite(0, LOW); 
    TmShw = 0; 
    Chk = false; 
    btn = 0;
    sleep_mode();         
    }    
  }
} 
