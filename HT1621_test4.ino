// http://arduinolearning.com/code/ht1621-6-digit-7-segment-display-example.php
// random clock and temperature with decimal point by Nicu FLORICA (niq_ro) - http://www.arduinotehniq.com
// ver.4 - Craiova, 14.10.2018

#define CS   13  //Pin 2 as chip selection output
#define WR   12  //Pin 3 as read clock output
#define DATA 7  //Pin 4 as Serial data output
 
#define CS1    digitalWrite(CS, HIGH) 
#define CS0    digitalWrite(CS, LOW)
#define WR1    digitalWrite(WR, HIGH) 
#define WR0    digitalWrite(WR, LOW)
#define DATA1  digitalWrite(DATA, HIGH) 
#define DATA0  digitalWrite(DATA, LOW)
 
#define sbi(x, y)  (x |= (1 << y))  
#define cbi(x, y)  (x &= ~(1 <<y ))      
#define uchar   unsigned char 
#define uint   unsigned int 
 
#define  ComMode    0x52  
#define  RCosc      0x30  
#define  LCD_on     0x06 
#define  LCD_off    0x04 
#define  Sys_en     0x02 
#define  CTRl_cmd   0x80
#define  Data_cmd   0xa0   
 
/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,H,h,L,n,N,o,P,r,t,U,- (26), (27),degree (28),comma */
const char num[]={0x7D,0x60,0x3E,0x7A,0x63,0x5B,0x5F,0x70,0x7F,0x7B,0x77,0x4F,0x1D,0x0E,0x6E,0x1F,0x17,0x67,0x47,0x0D,0x46,0x75,0x37,0x06,0x0F,0x6D,0x02,0x00,0x33,0x80};
//char dispnum[6]={0x00,0x00,0x00,0x00,0x00,0x00};
 
 
void SendBit_1621(uchar sdata,uchar cnt) 
{ 
  uchar i; 
  for(i=0;i<cnt;i++) 
  { 
    WR0;
    if(sdata&0x80) DATA1; 
    else DATA0;
    WR1;
    sdata<<=1; 
  } 
}
 
void SendCmd_1621(uchar command)
{ 
  CS0; 
  SendBit_1621(0x80,4);   
  SendBit_1621(command,8);
  CS1;                     
}
 
void Write_1621(uchar addr,uchar sdata)
{ 
  addr<<=2; 
  CS0; 
  SendBit_1621(0xa0,3);    
  SendBit_1621(addr,6);     
  SendBit_1621(sdata,8);    
  CS1; 
} 
 
void HT1621_all_off(uchar num)
{
  uchar i; 
  uchar addr=0; 
  for(i=0;i<num;i++) 
  { 
    Write_1621(addr,0x00); 
    addr+=2; 
  } 
}
 
void HT1621_all_on(uchar num)
{
  uchar i; 
  uchar addr=0; 
  for(i=0;i<num;i++) 
  {
    Write_1621(addr,0xff); 
    addr+=2; 
  } 
}
 
void Init_1621(void)
{
  SendCmd_1621(Sys_en);
  SendCmd_1621(RCosc);    
  SendCmd_1621(ComMode);  
  SendCmd_1621(LCD_on);
}    

int ora, zora, uora;
int minut, zmin, umin;
int temperatura, temp, ztemp, utemp, stemp;
byte semn = 0;  
 
void setup() {
  Serial.begin(9600);
  Serial.println(" ");
  pinMode(CS, OUTPUT); // 
  pinMode(WR, OUTPUT); // 
  pinMode(DATA, OUTPUT); //
  CS1;
  DATA1;
  WR1;
  delay(50);
  Init_1621();
  HT1621_all_on(16); 
  delay(1000);
}
 
void loop() {

  ora = random(24);
  zora = ora/10;
  uora = ora%10;
Serial.print(ora);
Serial.print(" = ");
Serial.print(zora);
Serial.print(uora);
Serial.println(" ?");
  minut = random(60);
  zmin = minut/10;
  umin = minut%10;
Serial.print(minut);
Serial.print(" = ");
Serial.print(zmin);
Serial.print(umin);
Serial.println(" ?");

  temperatura = random(-390,390);
  if (temperatura < 0)
  {
    temp = - temperatura;
    semn = 2 ;  // negative
  }
else
   {
   temp = temperatura;
   semn = 0 ;  // negative
   }
   
   ztemp = temp/100;
   temp = temp%100;
   utemp = temp/10;
   stemp = temp%10;
Serial.print(temperatura);
Serial.print(" = ");
if (semn ==2) Serial.print("-");
else Serial.print("+");
Serial.print(ztemp);
Serial.print(utemp);
Serial.print(",");
Serial.print(stemp);
Serial.println(" ?");
  
HT1621_all_off(16);

  Write_1621(8,num[zora]); 
  Write_1621(6,num[uora]);
  Write_1621(2,num[zmin]); 
  Write_1621(0,num[umin]);  

for (int i=0; i <= 3; i++)
{
  Write_1621(4,num[26]);  // -
  delay(1000);
  Write_1621(4,num[27]);  // empty space
  delay(1000);
} 
  
   HT1621_all_off(16);
   delay(1000);

if (ztemp != 0)
{
if (semn == 2) Write_1621(10,num[26]); // -
  Write_1621(8,num[ztemp]);
}
else
if (semn == 2) Write_1621(8,num[26]); // -
  Write_1621(6,num[utemp]); 
  Write_1621(4,num[stemp]+num[29]);  // number with decimal points
  Write_1621(2,num[28]);  // degree
  Write_1621(0,num[12]);  // C
  delay(3000);
 
}
