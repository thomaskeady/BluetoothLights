void setup() {
  Serial.begin(9600);

  Serial.println("Starting");
}

int test = 10;

void loop() {
  Serial.println(test);
  delay(2000);
  
}

