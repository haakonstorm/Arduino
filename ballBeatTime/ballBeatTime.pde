import processing.serial.*;
int maxX = 1100; // For the screen
int maxY = 600;
int linefeed = 65;       // linefeed in ASCII

Serial myPort;


PFont font;

void setup(){
   size(maxX,maxY);
   background(0);
    font = loadFont("Serif-48.vlw"); 

  textFont(font); 
  text("Tiden mellom hver gang en ball lander:", 35, 50); 
  text("0 s.",1000,550);
  text("1 s.",1000,350);
  text("2 s.",1000,150);
   //set color white. For baselines of the three axis
  stroke(228,228,0);
  line(20,550,990,550);    
  line(20,550,20,150);    
 // line(990,550,990,150);    
  stroke(190,190,0);
  line(20,350,990,350);    
  line(20,150,990,150);    
  
 // line(20,150,990,150)
   
   println(Serial.list());
   myPort = new Serial(this, Serial.list()[0], 19200); // COM8, min serieport.
   myPort.bufferUntil(linefeed);
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
  //split string at comma
    String items[] = (split(serialString,' ')); 
    float T = float(items[1]);
   
    int x = int(T);
    if(x>400){
      x=400;
    }
    
  
  //set color white. For baselines of the three axis

//    line(0,yConst,width,yConst);    
 
    // increase timer, t.
    t = t+20;
    
  if (t>780)   //erase screen when graph reaches end 
  {
    t = 40;
    
    fill(0,0,0);
    stroke(0,0,0);
    
    rect(0,0,maxX,maxY);
    
    
    
   textFont(font); 
   stroke(255,255,255);
   fill(255,255,255);
  text("Tiden mellom hver gang en ball lander:", 35, 50); 
  text("0 s.",1000,550);
  text("1 s.",1000,350);
  text("2 s.",1000,150);
   //set color white. For baselines of the three axis
  stroke(228,228,0);
  line(20,550,990,550);    
  line(20,550,20,150);    
 // line(990,550,990,150);    
  stroke(190,190,0);
  line(20,350,990,350);    
  line(20,150,990,150); 
    
    
  }
  
      stroke(255,255,255);
  rect (t,550,8,-x);
  stroke(20,20,255);
  fill(20,20,255);
  rect(t+1,549,6,-(x-2));
}

void keyPressed() {
  exit(); // Stops the program
}


