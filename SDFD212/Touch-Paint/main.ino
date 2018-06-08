#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <URTouch.h>

#define TFT_DC 9              
#define TFT_CS 10            
#define TFT_RST 8
#define TFT_MISO 12         
#define TFT_MOSI 11           
#define TFT_CLK 13          

#define t_SCK 3              
#define t_CS 4                
#define t_MOSI 5              
#define t_MISO 6             
#define t_IRQ 7  

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

int x, y;
int brushThickness = 4;
int brushColour = ILI9341_RED;

void setup() {
  Serial.begin(9600);
  
  // Initialise TFT + TS
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
   
  ts.InitTouch();
  ts.setPrecision(PREC_EXTREME);
}

void loop() {
  // Print reset button (red square)
  tft.fillRect(0, 220, 20, 20, brushColour);
  
  // Print colour toolbar
  tft.fillRect(100, 220, 20, 20, ILI9341_RED);
  tft.fillRect(120, 220, 20, 20, ILI9341_YELLOW);
  tft.fillRect(140, 220, 20, 20, ILI9341_GREEN);
  tft.fillRect(160, 220, 20, 20, ILI9341_CYAN);
  tft.fillRect(180, 220, 20, 20, ILI9341_BLUE);
  tft.fillRect(200, 220, 20, 20, ILI9341_MAGENTA);
  
  // While touch screen detects pressure
  while (ts.dataAvailable()) {
    ts.read();
    x = ts.getX();
    y = ts.getY();
    
    // If touch is within the reset button, clear screen
    if ((x > 0) && (x < 20) && (y > 220) && (y < 240)) {
      tft.setCursor(0, 120);
      tft.setTextColor(ILI9341_WHITE); tft.setTextSize(3);
      tft.println("Clearing Screen..");
      tft.fillScreen(ILI9341_BLACK);
    // If touch is within the red colour button
    } else if ((x > 100) && (x < 119) && (y > 220) && (y < 240)) {
      brushColour = ILI9341_RED;
    // If touch is within the yellow colour button
    } else if ((x > 120) && (x < 139) && (y > 220) && (y < 240)) {
      brushColour = ILI9341_YELLOW;
    // If touch is within the cyan colour button
    } else if ((x > 140) && (x < 159) && (y > 220) && (y < 240)) {
      brushColour = ILI9341_GREEN;
    // If touch is within the blue colour button
    } else if ((x > 160) && (x < 179) && (y > 220) && (y < 240)) {
      brushColour = ILI9341_CYAN;
    // If touch is within the magenta colour button
    } else if ((x > 180) && (x < 199) && (y > 220) && (y < 240)) {
      brushColour = ILI9341_BLUE;
    } else if ((x > 200) && (x < 219) && (y > 220) && (y < 240)) {
      brushColour = ILI9341_MAGENTA;
    // Otherwise paint
    } else if ((x != -1) && (y != -1)) {
      tft.fillCircle(x, y, brushThickness, brushColour);
    }
  }
}
  Serial.print("x:");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.println(y);
}
