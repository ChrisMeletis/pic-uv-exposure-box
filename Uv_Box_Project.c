// LCD module connections
sbit LCD_RS at RC4_bit;
sbit LCD_EN at RC5_bit;
sbit LCD_D4 at RC0_bit;
sbit LCD_D5 at RC1_bit;
sbit LCD_D6 at RC2_bit;
sbit LCD_D7 at RC3_bit;

sbit LCD_RS_Direction at TRISC4_bit;
sbit LCD_EN_Direction at TRISC5_bit;
sbit LCD_D4_Direction at TRISC0_bit;
sbit LCD_D5_Direction at TRISC1_bit;
sbit LCD_D6_Direction at TRISC2_bit;
sbit LCD_D7_Direction at TRISC3_bit;
// End LCD module connections


//Rotery Encoder
sbit En_enter at RA2_bit;
//-------------------

//Shows time in minutes and seconds.
void Show_time(double value){
int min,sec;
char text[8],text2[8];

min=sec=0; 
if (value>59){
     min=value/60;
     sec=value-(60*min);
 }
 else
      sec=value;

 IntToStr(min,text);
 IntToStr(sec,text2);

 Lcd_Out(2,2,Ltrim(text));
 Lcd_Out_CP(":");
 Lcd_Out_CP(Ltrim(text2));
 Lcd_Out_CP(" ");
}


//Simple Countdown (uses "Show_time" func).
void Countdown(int value){
int i=0;

    PORTB.f1 = 1;
    Lcd_Cmd(_Lcd_Clear);
    Lcd_Out(1,1,"Time to shutdown");
    Delay_ms(10);
    for(i=value;i>=0;i--){
            Show_time(i);
            Delay_ms(990);
    }
    Lcd_Cmd(_Lcd_Clear);
    PORTB.f1 = 0;
    Lcd_Out(1,1,"Done!");
}


//User choses exposure times using a rotary encoder knob.
double Chose_exp_time(){
double value=0;
char new_encoder, last_encoder;

 Lcd_Out(1,1,"Exposure time:");
 while (En_enter==0){
  new_encoder = (PORTA & 0x03);      // keep only 2 bits
   if(new_encoder != last_encoder){      // if encoder has changed
      if(new_encoder.F1 == last_encoder.F0) value+=0.25;
      else value-=0.25;
      last_encoder = new_encoder;   // save for next time
   }

   if(value<0)
     value=0;
     
   Show_time(value);

 }
 return value;
}



//Buzzer lets the user know that cooking procedure done.
void Buzz(){
 int tms,i,j=255;

 TRISB = 0x00;
 for(tms=0;tms<2;tms++){
    for(i=0;i<60;i++){ // from off to on fading
        PORTB.f0=1;
        delay_cyc(40);
        PORTB.f0=0;
        delay_cyc(40);
    }
    delay_ms(180);
    for(i=0;i<60;i++){ // from off to on fading
        porta.f0=1;
        delay_cyc(40);
        porta.f0=0;
        delay_cyc(40);
    }
  delay_ms(500);
 }
 PORTB = 0;
}


void main() { 
 double value=0;

 ADCON1 = 0b00000110; // all PORTA pins as digital I/O
 TRISB = 0xFF;           //Set PORTB as Input
 TRISC = 0x00;           //Set PORTC as Output
 
 for(;;){
 Lcd_Init();
 Delay_ms(10);
 Lcd_Cmd (_Lcd_CURSOR_OFF);
 Delay_ms(10);
 Lcd_Cmd(_Lcd_Clear);

 value=Chose_exp_time();
 Countdown(value);
 Buzz();
 }
}