#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
Adafruit_SSD1306 display(128, 64);  // Create display
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int set = 1;
float vSet;
float setOut;

int rel = 3;

char data = 0;

void setup() {
Serial.begin(9600);
pinMode(rel, OUTPUT);

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
        digitalWrite(rel, HIGH);//switch relay on
        Serial.print("Relay On for 1 Second\n");
        delay(1000);
        digitalWrite(rel, LOW);//switch relay off
        Serial.print("Relay Off Again\n");
        
      }
        else if(data == '2')       //Checks whether value of data is equal to 0
        {
        digitalWrite(rel, HIGH);//switch relay on
        Serial.print("Relay On for 2 Second\n");
        delay(2000);
        digitalWrite(rel, LOW);//switch relay off
        Serial.print("Relay Off Again\n");
        }


//screen loop
display.clearDisplay();  // Clear the display so we can refresh
display.setFont(&FreeMono9pt7b);  // Set a custom font
display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

// Print text:
display.setCursor(0, 10);  // (x,y)
display.println("Check Temp");  // Text or value to print
display.drawLine(0, 15, 120, 15, WHITE);  // Draw line (x0,y0,x1,y1,color)
display.setCursor(0, 35);
display.println("Set:");
display.setCursor(0, 55);
display.println("Tmp:");


  vSet = analogRead(set);
  setOut = (vSet /1023)*100;

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0; 



    if (Tc > setOut){

      digitalWrite(rel, HIGH);//switch relay on 
      Serial.print("Cooling on 3 seconds \n");

      //clear and reset display
      display.clearDisplay();  // Clear the display so we can refresh
      display.setFont(&FreeMono9pt7b);  // Set a custom font
      display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

      display.setCursor(0, 35);
      display.println("-- Cooling");
      display.setCursor(0, 55);
      display.println("-- Cooling");
      Serial.print("Cooling\n");
      

      display.display();  // Print everything we set previously
      delay(3000);

      //clear and reset display
      display.clearDisplay();  // Clear the display so we can refresh
      display.setFont(&FreeMono9pt7b);  // Set a custom font
      display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

      display.setCursor(0, 35);
      display.println("-- Hold");
      display.setCursor(0, 55);
      display.println("-- Hold");
      Serial.print("Holding\n");

      display.display();  // Print everything we set previously
      
      Serial.print("Delay 3 seconds then check again! \n");
      digitalWrite(rel, LOW);//switch relay off 
      delay(3000);

      //clear and reset display
      display.clearDisplay();  // Clear the display so we can refresh
      display.setFont(&FreeMono9pt7b);  // Set a custom font
      display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

      
    }



display.setCursor(0, 35);
display.println("Set:");
display.setCursor(45, 35);
display.println(setOut); 
Serial.print("Set: ");
Serial.print(setOut);
Serial.print("\n");

display.setCursor(0, 55);
display.println("Tmp:");
display.setCursor(45, 55);
display.println(Tc); 
Serial.print("Temp: ");
Serial.print(Tc);
Serial.print("\n");


display.display();  // Print everything we set previously


    

  delay(2000);
}
