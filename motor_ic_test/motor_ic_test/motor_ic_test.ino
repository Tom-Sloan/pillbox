int nenable = 27;
int stepb = 30;
int dir = 31;
int m0 = 15;
int m1 = 7;
int nsleep = 16;
int configPin = 11;
boolean dir_toggle = false;

void setup() {
  Serial.begin(115200);
  Serial.println("IC motor test");
  Serial.println("---------------------------\n");
  pinMode(nenable, OUTPUT);
  pinMode(stepb, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(m0, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(nsleep, OUTPUT);
  pinMode(configPin, OUTPUT);

  digitalWrite(nenable, LOW);
  digitalWrite(m0, LOW);
  digitalWrite(m1, HIGH);
  
  digitalWrite(configPin, HIGH);
  digitalWrite(dir, LOW);
  digitalWrite(nsleep, LOW);
  delay(100);
  digitalWrite(nsleep, HIGH);
//  digitalWrite(stepb, LOW);
}

void loop() {
  Serial.println("Loop");
//  if (dir_toggle)
//  {
//    digitalWrite(dir, LOW);
//    dir_toggle = false;
//  } else
//  {z
//    digitalWrite(dir, HIGH);
//    dir_toggle = true;
//  }
//  delay(100);
                                                                               
  for (int i = 0; i <= 1000; i++)
  {
    Serial.println("FOR");
    digitalWrite(stepb, HIGH);
    int a = digitalRead(stepb);
    Serial.println(a);
    delay(10);
    digitalWrite(stepb, LOW);
    a = digitalRead(stepb);
    Serial.println(a);
    delay(10);
    
  }
  delay(1000);
}
