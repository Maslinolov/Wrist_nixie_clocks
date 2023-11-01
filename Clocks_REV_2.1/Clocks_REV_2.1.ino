#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

int MinL = 0, MinR= 0, HrsL = 0, HrsR = 0;
int rght = 3, lft = 4;
int ShwnTm = 120;
bool TmUpdt = false, TmChange = false, Chk = false;
int Disch = 0;
int TmShw = 0;
int val=0;                      
boolean flag=0; 
int btn = 0;
int sec = 0; 
bool A = true;
int prval = 2;
bool slp = false;



void setup() {
  TCCR2A = 0x00; 
  TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
  TIMSK2 = 0x01;   
  ASSR = (1<<EXCLK);
  ASSR = (1<<AS2);

  /*cli();
  TIMSK2 &= ~((1<<OCIE2A)|(1<<OCIE2B)|(1<<TOIE2));
  ASSR = (1<<AS2);
  delay(2000);
  TCNT2 = 0;
  TCCR2B |= (1<<CS22) | (1<<CS20);
  while(ASSR & 0x1F);
  TIFR2 |= ((1<<OCF2A)|(1<<OCF2B)|(1<<TOV2));
  TIMSK2 |= (1<<TOIE2);
  sei();*/

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

  pinMode(13, INPUT); 
  digitalWrite(13, LOW);  
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
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
    sec  += 400;
    if(sec >= 6000){       
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
      sec -= 5990;
    }    
      if(val == 0 && btn == 0){
        slp = true;
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
  if(slp){     
    sleep_mode();
    slp = false;
  }  
   if(btn == 1){
    if(digitalRead(13) == 1 && A == true){
      val++;
      A = false;
      delay(2000);
    }   
    val++;  
    btn = 0;     
  } 
  if(val >= 2){
    if(digitalRead(2) == 1)
      val++;
    }
  if(prval < val){
    prval = val;
    delay(1000);
    }
  
  if(val == 1){
    TmShw = 1;
    val = 0;
    btn = 0;    
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
        delay(200); 
      }    
      digitalWrite(rght, LOW);           
      digitalWrite(lft, HIGH);
      for(int p = 0; p < 10; p++){
        tabl(p); 
        delay(200); 
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
      A = true;
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
