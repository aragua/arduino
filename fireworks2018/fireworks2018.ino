#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define STARTBTN 2
#define NEXTBTN 3

typedef struct {
  int io;
  int duration;
} Line;

typedef Line * Table;
#define ENDL {-1,-1}

/* Write firework config here */
Line table0[] = {{22,0}, {23,12}, {24,48}, {25,8}, {26,7}, {27,10}, {28,17}, {29,26}, {30,17}, {31,26}, {32,31}, {33,29}, ENDL };
Line table1[] = {{42,0}, {43,18}, {44,43}, {45,15}, {46,20}, {47,26}, {48,6}, {49,15}, {50,20}, ENDL };
Table tables[] = {table0, table1};
/*******************************/
unsigned int tabsz = sizeof(tables)/sizeof(Line *);
int countdown = 0;
int nextlineidx = 0;
int lasttablescount = 1;
unsigned int table = 0;
int started = 0;

void display_status() {
  display.clearDisplay();
  display.setCursor(0,0);

  if (table >= tabsz) {
    display.println("Enjoy!!");
  } else  if ((started == 0) || (table < 0)) {
    display.println("Ready");
  } else {
    display.setTextSize(3);
    display.print("Next ");
    if (nextlineidx+lasttablescount < 10)
      display.print(" ");
    display.println(nextlineidx+lasttablescount);
  }
  display.setTextSize(1);
  display.println("");
  display.println("");
  
  display.setTextSize(3);
  display.setTextColor(WHITE);
  if ((table >= tabsz) || (started == 0) || (table < 0)) {
    display.println("");
  } else {
    display.print("T");
    display.print(table + 1);

    if ((countdown < 10) && (countdown >= 0))
      display.print("    ");
    else if (((countdown < 100) && (countdown >= 0)) || ((countdown < 0) && (countdown > (-10))))
      display.print("   ");
    else
      display.print("  ");
    display.println(countdown);
  }
  display.display();
}
  
void setup() {
  int idx, t;
  Serial.begin(9600);

  pinMode(STARTBTN, INPUT);
  pinMode(NEXTBTN, INPUT);

  for (t = 0 ; t < tabsz ; t++) {
    for (idx = 0; tables[t][idx].io != -1 ; idx++) {
      pinMode(tables[t][idx].io, OUTPUT);
      digitalWrite(tables[t][idx].io, HIGH);
      Serial.println(tables[t][idx].io);
    }
  }
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_status();
}

void loop() {
  static int delayed = 0, counter = 0;
  int startState, nextState;
  startState = digitalRead(STARTBTN);
  nextState = digitalRead(NEXTBTN);

  if (nextState == HIGH) {
    if ((started != 0) && (table >= 0) && (table < tabsz)) {
      delayed = 1;
    }
    display_status();
  } else {
    if (started == 0) {
      if (startState == HIGH) {
        lasttablescount += nextlineidx;
        nextlineidx = 0;
        started = 1;
        countdown = tables[table][nextlineidx].duration;
      }
    } else if ((table >= 0) && (table < tabsz)) {
      if (delayed == 1) {
        delayed = 0;
        countdown = 0;
      }
      if (tables[table][nextlineidx].io != -1) {
        if (countdown <= 0 ) {
          digitalWrite(tables[table][nextlineidx].io,LOW);
          delay(250);
          digitalWrite(tables[table][nextlineidx].io,HIGH);
          nextlineidx++;
          countdown = tables[table][nextlineidx].duration;
          display_status();
        }
      } else {
        table++;
        started = 0;
      }
    } else {
      Serial.println("Finished");
    }
  }
  if (counter % 4 == 0) {
    countdown--;
    display_status();
  }
  counter++;
  delay(250);
}
