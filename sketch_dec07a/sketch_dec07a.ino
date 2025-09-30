int spins[8] = {A0, A1, A2, A3, A4, A5, A6, A7 };
int smin[8];
int smax[8];
int threshold[8];
int reading[8];
int p[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int black = 0;

const int lpin = 5;
const int lpinf = 7;
//const int lpinb = 8;
const int rpin = 6;
const int rpinf = 9;
//const int rpinb = 10;


void calibrateSensors() {

  
    int sum = 0;
    Serial.println("Calibrating sensors...");
    digitalWrite(LED_BUILTIN, HIGH);
    for (int i = 0; i < 8; i++) {
        smin[i] = 1023;
        smax[i] = 0;
    }

    for (int j = 0; j < 100; j++) {
        for (int i = 0; i < 8; i++) {
            int value = analogRead(spins[i]);
            if (value < smin[i]) {
                smin[i] = value;
            }
            if (value > smax[i]) {
                smax[i] = value;
            }
        }
        delay(30);
    }
    Serial.println("Calibration complete.");
    digitalWrite(LED_BUILTIN, LOW);
    for (int i = 0; i < 8; i++) {
        threshold[i] = (smin[i] + smax[i]) / 2;
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" threshold: ");
        Serial.print(threshold[i]);
        sum += threshold[i];
    }
   
}

void mdrive(int lm ,int rm){
  if (lm>0){
    if(lm>255){
      lm=255;
     }
    digitalWrite(lpinf,HIGH);
    //digitalWrite(lpinb,LOW);
    analogWrite(lpin,lm);
  }else{
     if(lm<-255){
      lm=-255;
     }
    digitalWrite(lpinf,LOW);
    //digitalWrite(lpinb,HIGH);
    analogWrite(lpin,lm*-1);
    }

    if (rm>0){
    if(rm>255){
      rm=255;
     }
    digitalWrite(rpinf,HIGH);
    //digitalWrite(rpinb,LOW);
    analogWrite(rpin,rm);
  }else{
     if(rm<-255){
      rm=-255;
     }
    digitalWrite(rpinf,LOW);
    //digitalWrite(rpinb,HIGH);
    analogWrite(rpin,rm*-1);
    }
    
      
      
  }

void sread(){
   for (int i = 0; i < 8; i++) {
             reading[i] = analogRead(spins[i]);
   }


   for (int j = 0; j < 8; j++)
  {
    if (reading[j] <threshold[j] )
    {
      p[j] = 1;
    } else
    {
      p[j] = 0;
    }
  }

  if (p[0] == 1 && p[7] == 1 && (p[3] == 0 || p[4] == 0))
  {
    mdrive(255,255);
    delay(8);
    mdrive(0,0);
    delay(10);

    black = 0;
    
  }


  if (p[0] == 0 && p[7] == 0 && (p[3] == 1 || p[4] == 1 ) )
  {

 mdrive(255,255);
    delay(8);
 mdrive(0,0);
    delay(10);
    black = 1;
  }

  if (black == 1)
  {

    for (int j = 0; j < 8; j++)
    {
      if (reading[j] > threshold[j])
      {
        p[j] = 1;
      } else
      {
        p[j] = 0;
      }
    }

  }
  if (black == 0)
  {

    for (int j = 0; j < 8; j++)
    {
      if (reading[j] < threshold[j])
      {
        p[j] = 1;
      } else
      {
        p[j] = 0;
      }
    }
  }
    for (int i = 0; i < 8; i++) {
      Serial.print("   ");
      Serial.print(p[i]);
      Serial.print("   ");
      }    
      Serial.println("   ") ;
   }
  



void setup() {
 Serial.begin(115200);
    //pinMode(lpin, OUTPUT);
    pinMode(lpinf, OUTPUT);
    //pinMode(lpinb, OUTPUT);
    pinMode(rpin, OUTPUT);
    pinMode(rpinf, OUTPUT);
    //pinMode(rpinb, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    //calibrateSensors();
     for (int i = 0; i < 8; i++) {
        threshold[i] = 100;}
}

void loop() {
 

sread();


if (p[0] == 0 && p[7] == 0 && (p[3] == 0 && p[4] == 0))
    {

      mdrive(255,255);
      delay(2);    
      return;
    }
    if (p[7] == 0 )
    {
   mdrive(255,255);
      delay(5);
    mdrive(255,-255);
      delay(10);

      return;
    }
    if (p[5] == 0)
    {
      mdrive(255,255);
      delay(5);

     mdrive(255,0);
      delay(7);

      return;
    }

     if (p[4] == 0 && p[3] == 0)
    {

     mdrive(255,255);
      delay(2);   
      return;
    }

    if (p[2] == 0 )
    {

      mdrive(255,255);
      delay(5);
     mdrive(0,255);
      delay(7);

      return;

    } 
    
    if (p[0] == 0)
    {  mdrive(255,255);
      delay(5);

      mdrive(-240,255);
      delay(10);

      return;
    }
   
     

  }
