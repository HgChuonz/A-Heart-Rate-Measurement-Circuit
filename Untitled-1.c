#INCLUDE <16F887.H>       //
#FUSES  HS                 // cho phep su dung thach anh ngoai       // intrc
#USE DELAY(CLOCK=20M)      // thach anh 20M

#define LCD_ENABLE_PIN PIN_b2    // dinh nghia theo lcd (phan cung)
#define LCD_RS_PIN PIN_b4
#define LCD_RW_PIN PIN_b3

#define LCD_DATA4 PIN_D7
#define LCD_DATA5 PIN_D6
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D4

#include <lcd.c>

#define        cb       pin_d2
#define        led      pin_c0
#define        nn       pin_a0

signed int32 xung,luu,bmp,bdn;
int1     tt;

#int_timer1    // tran thi ngat 1 lan. khi tran la dat gia tri 65535
void ngat_timer()
{
   if(tt==1)         // co nhan
   {
      bdn++;         // sau 100ms no tang len 1 don vi
      set_timer1(3036);       // gia tri bat dau
      if(input(cb)==1)  
      {
         output_high(led);
         delay_ms(200);       // chong nhieu cam bien
         
         xung++;
         while(input(cb)==1);       // cho xung xuong muc 0
      }
      else              output_low(led);
      
      if(bdn>=150)       // 200*100ms = 20000ms = 20s
      {
         bdn=0;
         setup_timer_1(T1_DISABLED);
         
         luu=xung;
         xung=0;
         tt=0;
         bmp=luu*3;
         output_low(led);
         setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
      }   
   }
}

void nn_start()
{
   if(input(nn)==0)        // co nhan khong
   {
      delay_ms(20);        // chong doi
      if(input(nn)==0)     // con nhan
      {
         tt=1;             // bat dau dem
         xung=0;
         bdn=0;
         while(input(nn)==0);       // cho nha
      }
   }   
}

VOID MAIN()
{
   set_tris_b(0x01);    // 0000 0001 = b0
   set_tris_d(0x04);      //0000 0100
   set_tris_a(0x01);
    
   bdn=0;
   luu=0;
   xung=0;
   lcd_init();       // khoi tao lcd
   setup_timer_1(t1_internal|t1_div_by_8);         // khoi tao timer
   set_timer1(3036);
   
   enable_interrupts(global);
   enable_interrupts(int_timer1);            // khoi tao ngat timer
   
   lcd_gotoxy(1,1);
   lcd_putc("                ");
   lcd_gotoxy(1,2);
   lcd_putc("                ");
   
   output_low(led);
   tt=0;
   bmp=0;
   WHILE(TRUE)
   {
      nn_start();
      if(tt==0)
      {
         output_low(led);
         lcd_gotoxy(1,1);              
         lcd_putc("    NHIP TIM    ");
         lcd_gotoxy(1,2);        // cot 1 hang 2
         lcd_putc("    ");
         lcd_putc(bmp/100+0x30);
         lcd_putc(bmp/10%10+0x30);
         lcd_putc(bmp%10+0x30);
         lcd_putc("(BMP)    ");   
      }
      else
      {
         
         lcd_gotoxy(1,1);              
         lcd_putc("BAT DAU DO......");
         lcd_gotoxy(1,2);        // cot 1 hang 2
         lcd_putc("KHONG BO TAY RA "); 
      }
      delay_ms(100);
   }
}
