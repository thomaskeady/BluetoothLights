// Arduino side code 

#define IN 2
#define LED 13

void setup() {
  // put your setup code here, to run once:
  pinMode(IN, INPUT);

  pinMode(LED, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Starting");
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {
    Serial.print(Serial.read());
    digitalWrite(LED, HIGH);
  }
  digitalWrite(LED, LOW);
  
}
