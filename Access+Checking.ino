#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>  
#include<EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(13,12); // RX,TX
#define Password_Length 5
#define a 0;


Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

String presentValue = " ";
String changeValue = " ";
int pos = 0;
int redPin = 10;
int greenPin = 11;
int BTP = 8;

char Data[Password_Length];
char Master[Password_Length];
byte data_count = 0, master_count = 0;


bool door = false;
char pressed;


/*---preparing keypad---*/

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/*--- Main Action ---*/

void setup()
{
  pinMode(redPin,OUTPUT);
 // myservo.attach(9);  
  Serial.begin(115200);
  mySerial.begin(9600);
  
  myservo.attach(9, 2000, 2400);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(BTP, INPUT);
  ServoClose();
 // lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.print("    Welcome  ");
  lcd.setCursor(0, 1);
  lcd.print(" To Secure Door ");
  delay(2000);
  lcd.clear();
  loading(" Loading");
  lcd.clear();
  initialpassword();
}


void loop()
{  
  String msg = mySerial.readStringUntil('\r');
  bool BTPstate = digitalRead(BTP);

  if (BTPstate == true || (Actionnage(msg) && reboucle() )  ) {
      myservo.attach(9);
      lcd.clear();
      lcd.print("  Door is Open ");
      digitalWrite(greenPin, HIGH);
      digitalWrite(redPin, LOW);
      ServoOpen();
      delay(3000);
      lcd.clear();
      lcd.print(" Door is Closed "); 
      ServoClose();
      delay(2000);
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      lcd.clear();
  } else {

    if (door == true)
    {
      pressed = keypad.getKey();

      if (pressed == '#')
      {
        lcd.clear();
        ServoClose();
        door = false;
      }
    }
  }
    
     if (pressed == 'C'){
      change();
    }
    else if (pressed != '*') {
      Open();
    } else {
      lcd.clear();
      lcd.print("     Reset    ");
      delay(1000);
     Data[Password_Length] =  0;
     data_count = 0;
      
      lcd.clear();
      Open();
    }
      }












      

void loading (char msg[]) {

  lcd.setCursor(0, 1);
  lcd.print(msg);

  for (int i = 0; i < 9; i++) {
    delay(500);
    lcd.print(".");
  }
}

void clearData()
{
  while (data_count != 0)
  {
    Data[data_count--] = 0;
  }
  return;
}


void ServoOpen()
{
  for (pos = 0; pos <= 90; pos += 10) {
    myservo.write(pos); 
  } 
}

void ServoClose()
{
  for (pos = 90; pos >= 0; pos -= 10) {
    myservo.write(pos);
  }
}

void Open()
{

   myservo.detach();
  digitalWrite(redPin, HIGH);
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password ");
  pressed = 0;
  pressed = keypad.getKey();
  if (pressed)
  {
    Data[data_count] = pressed;
    lcd.setCursor(data_count, 1);
    lcd.print('*');
    data_count++;
  }
  if (data_count == Password_Length - 1)
  {
    if (!strcmp(Data, Master))
    {
      myservo.attach(9);
      lcd.clear();
      lcd.print("  Door is Open ");
      digitalWrite(greenPin, HIGH);
      ServoOpen();
      digitalWrite(redPin, LOW);
      door = true;
      delay(4000);
      loading(" Waiting");
      lcd.clear();
      lcd.print("  Time is up! ");
      lcd.setCursor(0, 1);
      lcd.print(" Door is Closed ");
      delay(2000);
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);

      ServoClose();
      door = false;
    }
    else if (strcmp(Data, Master) && pressed != '*')
    {
      lcd.clear();
      lcd.print(" Wrong Password ");
      door = false;
    }
    delay(1000);
    lcd.clear();
    clearData();
  }
}
void change()

{
 lcd.clear();
  int j=0;
  lcd.print("Change Pass");
  delay(1000);
  
  lcd.clear();

  lcd.print("Current Password");

  lcd.setCursor(0,1);

  while(j<4)

  {

    pressed =keypad.getKey();

    if(pressed)

    {

      Data[j++]=pressed;

      lcd.print(pressed);

       

    }

    pressed=0;

  }

  delay(500);




  if(strncmp(Data, Master, 4))
  {

    lcd.clear();

    lcd.print("Wrong Password");

    lcd.setCursor(0,1);

    lcd.print("Try Again");

    delay(1000);

  }

  else

  {

    j=0;

    lcd.clear();

    lcd.print("New Password:");

    lcd.setCursor(0,1);

    while(j<4)

    {

      pressed=keypad.getKey();

      if(pressed)

      {

        Master[j]=pressed;

        lcd.print(pressed);

        EEPROM.write(j,pressed);

        j++;

     

      }

    }
    
    lcd.clear();
    lcd.print("Pass Changed");

    delay(1000);

  }

 lcd.clear();
   clearData();
//pressed = 0;
Open();
  //lcd.print("Enter Password");

  //lcd.setCursor(0,1);

  
  

}

void initialpassword(){

  // for(int j=0;j<4;j++)

  // EEPROM.write(j, j+49);

  for(int j=0;j<4;j++)

    Master[j]=EEPROM.read(j);

} 

void ConnectToArduino(){
    String msg = mySerial.readStringUntil('\r');
     
  do{
    
    presentValue = msg;
    myservo.attach(9);
    
  }while(msg != presentValue);
    
    if (presentValue != changeValue)
  {
    Serial.println(msg);
    changeValue = presentValue;
    if(presentValue == "1")                   // Password
    {
    digitalWrite(redPin , LOW);
    digitalWrite( greenPin, HIGH);
   myservo.write(180);
   

   //  myservo.detach();
    changeValue = " ";
    }
    else if(presentValue == "0")
    {
      digitalWrite(redPin , LOW);
      digitalWrite(greenPin , LOW);
        
        myservo.write(0);
        
      changeValue = " ";
      
    }
  }
}




int Actionnage (String msg){
  if(msg  == "1")                   // Password
    {
   /* digitalWrite(redPin , LOW);
    digitalWrite( greenPin, HIGH);
    myservo.write(180); */
    return a+1; }
  else if ( msg  == "0")
    {
     /* digitalWrite(redPin , LOW);
      digitalWrite(greenPin , LOW);   
      myservo.write(0); */
      return a;
    }
  }


  

int reboucle () {
  String msg;
  if ( Actionnage(msg)){return a+1;}
   else return a;
  
  }
