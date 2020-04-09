int out1 = 9;
int potentiometerPin = A2;


void setup() {
  Serial.begin(9600); 
  pinMode(out1,OUTPUT);

}

void loop() {
//analogWrite(out1, 0) ; 

int potValue = potentiometerValue();
int fadeValue = map(potValue, 0, 1023, 0, 255);   // sets the value (range from 0 to 255):

analogWrite(out1, fadeValue);

delay(30);      

}

//function to calculate potentiometer value
int potentiometerValue()
   {
    int val = analogRead(potentiometerPin);
    return val;
   } 
