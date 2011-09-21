import processing.serial.*;
int maxX = 1100;  // For the screen
int maxY = 600;
int linefeed = 65;       // linefeed in ASCII
Serial myPort;

PFont font;
// The font must be located in the sketch's 
// "data" directory to load successfully






void setup(){
   size(maxX,maxY);
   background(0);
   font = loadFont("Serif-48.vlw"); 
  textFont(font); 
  text("Kraften som virker på ballen:", 35, 50); 
  text("0 g.",1000,550);
  text("1 g.",1000,445);
  
   println(Serial.list());
   myPort = new Serial(this, Serial.list()[0], 19200); // COM8, min serieport.
   myPort.bufferUntil(linefeed);
   //set color white. For baselines of the three axis
  stroke(228,228,0);
  line(20,550,990,550);    
  line(20,550,20,150);    
 // line(990,550,990,150);    
  stroke(190,190,0);
  line(20,445,990,445);    
 // line(20,150,990,150);    
}

void draw(){}

void serialEvent(Serial myPort) { 
  // read the serial buffer:
  String myString = myPort.readStringUntil(linefeed);
  // if you got any bytes other than the linefeed:
  if (myString != null) {
 
  parseString(myString); 
  }
} 

 int t = 20; 
void parseString(String serialString)
{
  font = loadFont("Serif-48.vlw"); 
  textFont(font); 
  //split string at comma
    String items[] = (split(serialString,' ')); 
    float T = float(items[1]);
   
    int x = int(T);
    if(x>400){
      x=400;
    }
   
    // increase timer, t.
    t = t+4;
    
  if (t > 960)   //erase screen when graph reaches end 
  {
    t = 40;
     //set color white. For baselines of the three axis
  stroke(0,0,0);
  fill(0,0,0);
  rect(0,0,maxX,maxY);
  stroke(255,255,255);
  fill(255,255,255);
    text("0 g.",1000,550);
  text("1 g",1000,445);
  text("Kraften som virker på ballen:", 35, 50); 
  stroke(228,228,0);
   line(20,550,990,550);    
  line(20,550,20,150);    
//  line(990,550,990,150);    
  stroke(190,190,0);
  line(20,445,990,445);    
 // line(20,150,990,150);    
  }
  
 // stroke(255,255,255);
 // rect (t,550,8,-x);
  stroke(50,50,255);
  fill(50,50,255);
  rect(t+1,549,2,-(x-2));
}

void keyPressed() {
  exit(); // Stops the program
}


