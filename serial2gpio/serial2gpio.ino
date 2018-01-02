int tab[25]= { -1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53};

bool pinisvalid (int n) {
  if ((n>0) && (n<25) && (tab[n]>0) && (tab[n]<=53))
    return true;
  else
    return false;
}

void pininit (int n) {
  if (pinisvalid(n)) {
    pinMode(tab[n], OUTPUT);
    digitalWrite(tab[n], HIGH);
  }
}

int pinstatus (int n, bool serial) {
  if (pinisvalid(n)) {
    int st;
    st = digitalRead(tab[n]);
    if (serial) {
      Serial1.print("Gpio[");
      Serial1.print(n);
      Serial1.print("] = ");
      Serial1.println(st);
    }
    return st;
  } else {
    if (serial) {
      Serial1.println("Invalid pin");
    }
    return -1;
  }
}

void pinhigh (int n) {
  if (pinisvalid(n)) {
      digitalWrite(tab[n],HIGH);
  }
}

void pinlow (int n) {
  if (pinisvalid(n)) {
      digitalWrite(tab[n],LOW);
  }
}

void pintoggle (int n) {
  if (pinisvalid(n)) {
      digitalWrite(tab[n],!(digitalRead(tab[n])));
  }
}


void setup() {
  int idx;
  
  Serial1.begin(9600);
  
  for (idx = 0; idx < 25 ; idx++)
  {
    if (pinisvalid(idx)) {
       pininit(idx);
       pinstatus(idx,true);
    }
  }
}

int c2i(char c) {
  if (c>=48 && c<=57)
    return (c - 48);
  return -1;
}

int ser2gpio(char c1,char c2) {
  int ret = -1;
  if ((c1 !=-1) && (c2 != -1)) {
    ret = c2i(c1) * 10 + c2i(c2);
  }
  return ret;
}

/* cmd : H -> set high
 *       L -> set low
 *       T -> toggle
 *       S -> return state
 */
char cmd = -1;
int c1 = -1;
int c2 = -1;

void printcmd () {
 Serial1.print("cmd = ");
 Serial1.print(cmd);
 Serial1.print(", c1 = ");
 Serial1.write(c1);
 Serial1.print(", c2 = ");
 Serial1.write(c2);
 Serial1.println(".");
}

void loop() {
  if (Serial1.available()) {
    if (cmd == -1) {
      char c = Serial1.read();
      if ((c == 'H') || (c == 'h') ||
          (c == 'L') || (c == 'l') ||
          (c == 'T') || (c == 't') ||
          (c == 'S') || (c == 's')) {
            cmd = c;
          } else if (
          else {
            Serial1.println("Invalid command");
          }
    } else if (c1 == -1) {
      c1 = Serial1.read();
    } else {
      c2 = Serial1.read();
    }
    printcmd();
    int n = ser2gpio(c1,c2);
    if (pinisvalid(n)) {
      switch (cmd) {
      case 'H':
      case 'h':
        pinhigh(n);
        pinstatus(n,true);
        break;
      case 'L':
      case 'l':
        pinlow(n);
        pinstatus(n,true);
      break;
      case 'T':
      case 't':
        pintoggle(n);
        pinstatus(n,true);
        break;
      case 'S':
      case 's':
        pinstatus(n,true);
        break;
      default:
        Serial1.println("Invalid command");
        break;
      }
      cmd = -1;
      c1 = -1;
      c2 = -1;
    }
    delay(500);
  }
}
