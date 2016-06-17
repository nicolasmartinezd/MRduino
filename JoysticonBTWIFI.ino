




//Motor4 anda mas lento que motor 3

// the setup function runs once when you press reset or power the board
#include <AFMotor.h>
#include <LiquidCrystal.h>            
LiquidCrystal lcd(53, 51, 49, 47, 45, 43);
#define diff 6
#define top3 255
#define top4 (255-diff)
#define curva 80

AF_DCMotor Motor3(3);
AF_DCMotor Motor4(4);
int input=-1;
String ordenes[]=
    {  //"AT+RST",
       "AT+CWMODE=2",
       "AT+CWSAP=\"Arduino\",\"1234\",3,0",
       
       "AT+CIPMUX=1",
       "AT+CIPSERVER=1,80",
       //"AT+CIFSR",
       "END"        // Para reconocer el fin de los comandos AT
    };


void setup()
   {
      Serial2.begin(115200); //wifi Lee puerto serial2 pines 16 (TX2) 17 (RX2)
      
      Serial3.begin(115200); //BT Lee puerto serial3 pines 14 (TX3) 15 (RX3)
      Serial.begin(115200);
      Motor3.setSpeed(255);
      Motor4.setSpeed(255-diff);
      lcd.begin(16, 4);
      lcd.setCursor(0,0);
      lcd.print("   Control BT");
      

   }

void loop()
   {
    input = -1;
    delay (3);
    if (Serial2.available()){
       input = Serial2.read(); 
     }
     if (Serial3.available()){
       input = Serial3.read();
       if (input == 83){
         serverup();
        }    
        if(input == 114){
          Serial2.println("AT+RST");
          lcd.clear();
          lcd.print("RESET");
          delay(9000);
        }
        
     }
       lcd.setCursor(3, 1);
       //lcd.clear();
       lcd.print(" input=");
       lcd.print(input);
       Motor3.setSpeed(255 - (input == 81)*curva);
       Motor4.setSpeed(255-diff - (input == 87)*curva-10);
       Serial.println(input);
       Motor3.run(RELEASE - 2*(input>0) - (input == 66 || input == 76));
       Motor4.run(RELEASE - 2*(input>0) - (input == 82 || input == 66));
       Serial3.flush();
   }

  void serverup(){
    
    int index = 0;
      while(ordenes[index] != "END")
          {  Serial2.println(ordenes[index++]);
             lcd.setCursor(0,0);
             lcd.print(ordenes[index]);
             while ( true)
               {   String s = GetLineWIFI();
                   if ( s!= "") Serial.println(s);
                   lcd.clear();
                   lcd.setCursor(0,1);
                   lcd.print(s);
                   delay(1000);
                   if ( s.startsWith("no change"))   
                           break;
                   if ( s.startsWith("OK"))   
                           break;
                   if ( s.startsWith("ready"))   
                           break;
                  // if (millis()-T >10000) break;  
               }
            Serial.println("....................");
        } 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  BT / SERVER");
    }

   void wifi()
   {  
      if (Serial2.available())
         {  
          Serial3.write(Serial2.read()); //escribe lo que entra desde BT (EN FORMA DE COMANDOS!!). 
         }

      if (Serial.available())
         {  
            char c = Serial.read() ;     //envia desde consola a WiFi, comandos AT.
           Serial3.print(c);             
         }
     
  
      if (Serial3.available())
         { char c = Serial3.read() ;
           Serial.print(c);              //imprime en consola lo que entra por WiFi.
         Serial2.print(c);               //imprime en BT lo que entra por WiFi.
         }
   }

   String GetLineWIFI()
   {   String S = "" ;
       if (Serial2.available())
          {    char c = Serial2.read(); ;
                while ( c != '\n' )            //Hasta que el caracter sea intro
                  {     S = S + c ;
                        delay(25) ;
                        c = Serial2.read();
                   }
                 return( S ) ;
          }
   }
