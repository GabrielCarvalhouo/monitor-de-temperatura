const int LM35 = D3;
float temp = 0; //Definindo a temperatura inicial.
int ADClido = 0;
const int buzzer = D4;
const int LedRed = D1;
const int LedGreen = D2;


void setup() {
Serial.begin(9600);
pinMode(buzzer, OUTPUT);
pinMode(LedRed, OUTPUT);
pinMode(LedGreen, OUTPUT);
}

void loop() {
ADClido = analogRead(LM35);
temp = ((ADClido + 32) * 5 / 9);
Serial.print("Temperatura = "); 
Serial.print(temp); //mostra valor na tela
Serial.println(" *C");
   
if(temp > 32){
digitalWrite(buzzer, HIGH); //aciona o buzzer
digitalWrite(LedRed, HIGH);  //aciona o led
}
else if(temp < 5){
digitalWrite(buzzer, HIGH); //aciona o buzzer
digitalWrite(LedGreen, HIGH); //aciona o led
} else {
digitalWrite(buzzer, LOW); //não aciona o buzzer
digitalWrite(LedGreen, HIGH); //aciona o led
digitalWrite(LedRed, HIGH);  //aciona o led
}
delay(500);
}
