#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
Adafruit_SSD1306 display(128, 64);  // Create display
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

int ThermistorPin1 = 1;
int ThermistorPin2 = 2;
int ThermistorPin3 = 3;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int set = 0;
float vSet;
float setOut;

int rel1 = 2;
int rel2 = 3;
int rel3 = 4;
int rel4 = 5;

char data = 0;

void setup() {
Serial.begin(9600);
pinMode(rel1, OUTPUT);
digitalWrite(rel1, HIGH);
pinMode(rel2, OUTPUT);
digitalWrite(rel2, HIGH);
pinMode(rel3, OUTPUT);
digitalWrite(rel3, HIGH);
pinMode(rel4, OUTPUT);
digitalWrite(rel4, HIGH);

  //display setup 
  delay(100);  // This delay is needed to let the display to initialize
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
                               // To override this behavior (so text will run off the right side of the display - useful for
                               // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                               // with setTextWrap(true).

  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)


}

void loop() {

  //bluetooth stuff 
  if(Serial.available() > 0)  // Send data only when you receive data:
    {
    data = Serial.read();      //Read the incoming data and store it into variable data
    Serial.print(data);        //Print Value inside data in Serial monitor
    Serial.print("\n");        //New line 
    }
    if(data == '1')            //Checks whether value of data is equal to 1 
      {
        digitalWrite(rel1, LOW);//switch relay on
        Serial.print("Relay On for 1 Second\n");
        delay(2000);
        digitalWrite(rel1, HIGH);//switch relay off
        Serial.print("Relay Off Again\n");
        
      }
    if(data == '2')       //Checks whether value of data is equal to 0
      {
        digitalWrite(rel2, LOW);//switch relay on
        Serial.print("Relay On for 2 Second\n");
        delay(2000);
        digitalWrite(rel2, HIGH);//switch relay off
        Serial.print("Relay Off Again\n");
      }

    if(data == '3')       //Checks whether value of data is equal to 0
      {
        digitalWrite(rel3, LOW);//switch relay on
        Serial.print("Relay On for 2 Second\n");
        delay(2000);
        digitalWrite(rel3, HIGH);//switch relay off
        Serial.print("Relay Off Again\n");
      }
    
    if(data == '4')       //Checks whether value of data is equal to 0
      {
        digitalWrite(rel4, LOW);//switch relay on
        Serial.print("Relay On for 2 Second\n");
        delay(2000);
        digitalWrite(rel4, HIGH);//switch relay off
        Serial.print("Relay Off Again\n");
      }


  vSet = analogRead(set);
  setOut = (vSet /1023)*100;

  Vo = analogRead(ThermistorPin1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0; 


//Start of screen loop - - - - - - - 
display.clearDisplay();  // Clear the display so we can refresh
display.setFont(&FreeMono9pt7b);  // Set a custom font
display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

//display setting value
display.setCursor(0, 11);
display.println("Set :");
display.setCursor(60, 11);
display.println(setOut); 
//Display Tmp1 value
display.setCursor(0, 26);
display.println("Tmp1:");
display.setCursor(60, 26);
display.println(Tc); 
//Display Tmp2 value
display.setCursor(0, 41);
display.println("Tmp2:");
display.setCursor(60, 41);
display.println(Tc); 

//Display Tmp3 value
display.setCursor(0, 56);
display.println("Tmp3:");
display.setCursor(60, 56);
display.println(Tc); 

// Print everything we set previously
display.display();  


//Serial Print Set value 
Serial.print("Set: ");
Serial.print(setOut);
Serial.print("\n");
//Serial print Tmp Values 
Serial.print("Tmp1: ");
Serial.print(Tc);
Serial.print("\n");
Serial.print("Tmp2: ");
Serial.print(Tc);
Serial.print("\n");
Serial.print("Tmp3: ");
Serial.print(Tc);
Serial.print("\n");

//Check if Tmp exceeds set value and turn on water is required 
    if (Tc > setOut){

      digitalWrite(rel1, LOW);//switch relay on 
      Serial.print("Cooling on 3 seconds \n");

      //clear and reset display
      display.clearDisplay();  // Clear the display so we can refresh
      display.setFont(&FreeMono9pt7b);  // Set a custom font
      display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

      display.setCursor(0, 35);
      display.println("-- Cooling");
      display.display();  // Print everything we set previously

      Serial.print("Cooling\n");
      
      delay(3000);
      //switch cooling water off rel1
      digitalWrite(rel1, HIGH);//switch relay off 

      //clear and reset display
      display.clearDisplay();  // Clear the display so we can refresh
      display.setFont(&FreeMono9pt7b);  // Set a custom font
      display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

      display.setCursor(0, 35);
      display.println("-- Hold");
      display.display();  // Print everything we set previously

      Serial.print("Holding\n");
      Serial.print("Delay then check again! \n");
      
      delay(3000);
      
    }

  delay(2000);
}
