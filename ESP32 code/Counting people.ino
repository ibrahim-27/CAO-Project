/* Variables Used */

// pins used
int IR1 = 14;
int IR2 = 4;
int red = 15;
int green = 13;

// variables to store count and triggered IR
int enter = 0;
int ext = 0;
bool check1=false;
bool check2=false;

int max_ppl = 5;

// to store time
long t = 0;

// to store last occurrence (enter / exit)
String last="";   

void setup() {
  Serial.begin(115200);

  /* Setting pins mode */
  pinMode(IR1,INPUT);
  pinMode(IR2, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  // setup
  if(digitalRead(IR1)==HIGH && digitalRead(IR2)==HIGH){
    last="";
  }
  
  /* If the first IR is triggered */
  if(digitalRead(IR1) == LOW)
  {
    if(last!="enter")
      check1=true;
      
    t = millis();
    if(check2){
      enter++;
      last="enter";
      check1=false;
      check2=false;
    }
    digitalWrite(green, HIGH);
  }
  else
  {
    digitalWrite(green, LOW);
  }

  /* If the second IR is triggered */
  if(digitalRead(IR2) == LOW)
  {
    if(last!="ext")
      check2=true;
      
    t = millis();
    if(check1){
      ext++;
      last="ext";
      check1=false;
      check2=false;
    }
    digitalWrite(red, HIGH);
  }
  else
  {
    digitalWrite(red, LOW);
  }

  /* Printing details */
  if(enter - ext <= max_ppl)
  {
    Serial.print("People in the room are: ");
    Serial.println(enter - ext);
  }
  else
  {Serial.println("Max people in the room");}

  /* For debugging */
  if(check1){
    Serial.println("1 RECOGNIZED");
    }
   if(check2){
    Serial.println("2 RECOGNIZED");
   }

   /* Checks if an IR is left triggered -- reset after 2 sec */
   if(!(check1 && check2))
   {
      if(millis() - t > 2000)
      {
        check1 = false;
        check2 = false;
      }
   }
}
