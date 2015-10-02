#include <FrequencyTimer2.h>
const int cols[8]={2,7,A5,5,11,A4,10,A2};
const int rows[8]={9,8,4,A3,3,A1,A0,6};

int display_pos=0;
int pattern_pos=0;
int pattern_num=0;

int pattern_size=164;

int slide_delay=110;
int advance_state=HIGH;

byte happy_birthday_austin[164]={0,126,8,8,8,126,0,124,18,18,18,124,0,126,18,18,18,12,0,126,18,18,18,12,0,2,12,112,12,2,0,0,0,126,74,74,74,52,0,66,126,66,0,126,26,26,42,76,0,2,2,126,2,2,0,126,8,8,8,126,0,126,66,66,66,60,0,124,18,18,18,124,0,2,12,112,12,2,0,0,0,124,18,18,18,124,0,62,64,64,64,62,0,36,74,82,82,36,0,2,2,126,2,2,0,66,126,66,0,126,4,8,16,126,0,0,94,0,0,0,60,114,225,225,225,225,225,225,225,225,225,114,60,0,0,0,246,247,0,0,246,247,0,0,246,247,0,0,246,247,0,0,246,247,0,0,246,247,0,0,246,247,0,0};
byte love_science[151]={0,12,30,62,124,248,124,62,30,12,0,0,36,74,82,82,36,0,60,66,66,66,0,66,126,66,0,126,74,74,66,0,126,4,8,16,126,0,60,66,66,66,0,126,74,74,66,0,0,64,160,144,143,128,143,144,160,64,0,0,0,0,0,0,0,0,0,0,0,12,30,62,124,248,124,62,30,12,0,0,126,4,8,16,8,4,126,0,124,18,18,18,124,0,2,2,126,2,2,0,126,8,8,8,126,0,0,0,0,64,128,112,14,1,17,17,1,137,253,129,1,1,0,0,108,146,146,146,108,0,0,130,198,170,146,130,198,0,0,8,132,132,124,4,4,124,132,132,68,0,0};
byte Austin_DOB[127]={0,124,18,18,18,124,0,62,64,64,64,62,0,36,74,82,82,36,0,2,2,126,2,2,0,66,126,66,0,126,4,8,16,126,0,0,0,0,126,4,8,16,8,4,126,0,124,18,18,18,124,0,66,36,24,36,66,0,126,32,16,8,16,32,126,0,126,74,74,66,0,126,64,64,64,0,126,64,64,64,0,0,0,0,2,12,112,12,2,0,126,74,74,66,0,126,8,8,8,126,0,0,0,0,0,0,68,126,64,0,60,66,66,60,0,16,16,0,68,98,82,82,76,0,0,0,0};

byte* pattern;
const int num_patterns=3;
int pattern_sizes[num_patterns];
byte* patterns[num_patterns];

void set_pattern_num(int num)
{
  pattern_num=num;
  pattern=(byte*)patterns[num];
  pattern_size=pattern_sizes[num];
  display_pos=0;
  pattern_pos=0;
}

void setup() {
  for(int idx=0;idx<8;idx++)
  {
    pinMode(rows[idx],OUTPUT);
    pinMode(cols[idx],OUTPUT);
    digitalWrite(rows[idx],HIGH);
    digitalWrite(cols[idx],LOW);  
  }

  pinMode(12,INPUT_PULLUP);
  FrequencyTimer2::disable();
  // Set refresh rate (interrupt timeout period)
  FrequencyTimer2::setPeriod(1000);
  // Set interrupt routine to be called
  FrequencyTimer2::setOnOverflow(display);

//  pattern=(byte*)&happy_birthday_austin;
  patterns[0]=(byte*)&happy_birthday_austin;
  pattern_sizes[0]=164;
  
  patterns[1]=(byte*)&love_science;
  pattern_sizes[1]=151;

  patterns[2]=(byte*)&Austin_DOB;
  pattern_sizes[2]=127;

  pattern_num=1;
  set_pattern_num(pattern_num);
  
}


void loop() {
  delay(slide_delay);
  pattern_pos++;
  pattern_pos=pattern_pos % pattern_size;
  int advance=digitalRead(12);
  if(advance!=advance_state)
  {
    if(advance==LOW)
    {
      pattern_num++;
      pattern_num = pattern_num % num_patterns;
      set_pattern_num(pattern_num);
    }
    advance_state=advance;
  }
}

void display()
{
  display_pos = display_pos % 8;
  int pattern_display_pos=(display_pos+pattern_pos) % pattern_size;
  byte pattern_data=pattern[pattern_display_pos];
  if((display_pos<3) && (pattern_data==0))
  {
    digitalWrite(13,HIGH);    
  }
  else
  {
    digitalWrite(13,LOW);
  }
  digitalWrite(cols[display_pos],HIGH);  
  for (int row=0;row < 8; row++)
  {
    if((pattern_data & 0b00000001)==1)
    {
        digitalWrite(rows[row],LOW);
        delayMicroseconds(50);
    }
    digitalWrite(rows[row],HIGH);
    pattern_data = pattern_data >> 1;       
  }
  digitalWrite(cols[display_pos], LOW); 
  display_pos++;
}

