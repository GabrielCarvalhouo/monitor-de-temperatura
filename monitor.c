const int LM35 = D3;
const int fan = D5;
float temp = 0; 
int ADClido = 0;
const int buzzer = D4;
const int LedRed = D1;
const int LedGreen = D2;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(fan, OUTPUT);
}

void loop() {
  ADClido = analogRead(LM35);
  temp = ((ADClido + 32) * 5 / 9);
  Serial.print("Temperatura = "); 
  Serial.print(temp);
  Serial.println(" *C");
  

  if(temp > 30){
    digitalWrite(buzzer, HIGH); 
    digitalWrite(LedRed, HIGH);  
    digitalWrite(LedGreen, LOW)
    digitalWrite(fan, LOW); // O valor da fan esta em LOW pois o transistor alterna a entrada de negativo e positivo
  }
  else if(temp < 5){
    digitalWrite(buzzer, HIGH);
    digitalWrite(LedGreen, HIGH);
    digitalWrite(LedRed, LOW); 
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(LedGreen, HIGH);
    digitalWrite(LedRed, HIGH);  
  }
  delay(500);
}
