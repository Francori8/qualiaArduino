#include <MIDI.h>
#include <math.h>  // Incluir para la función pow()
#include <AccelStepper.h>
#include "Motor.h"
#include "EstrategiaDeMotor.h"
#include "ControladorMotores.h"


MIDI_CREATE_DEFAULT_INSTANCE();

AccelStepper stepper1(1,2,5);
AccelStepper stepper2(1,3,6);
AccelStepper stepper3(1,4,7);
AccelStepper stepper4(1,12,13);

Motor motor1(stepper1);
Motor motor2(stepper2);
Motor motor3(stepper3);
Motor motor4(stepper4);

Coral cora;
Melodica melo;
Tutti tuti;


const int pinY = A0;  
const int pinX = A1;    
const int pinBoton = 11;  // Pin digital para el botón (opcional)

float tiempo;
float tiempo2;
int valorX;
int valorY;
int botton;

ControladorMotores controladorMotores(motor1, motor2, motor3, motor4,cora);


void setup() {
  // put your setup code here, to run once:
  tiempo = millis();
  tiempo2 = millis() ;
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
 
  stepper1.setMaxSpeed(2000);  
  stepper1.setSpeed(0);
  stepper4.setMaxSpeed(2000);  
  stepper4.setSpeed(0);
  stepper2.setMaxSpeed(2000);  
  stepper2.setSpeed(0);
  stepper3.setMaxSpeed(2000);  
  stepper3.setSpeed(0);
  valorY = 512;
  valorX = 512;
  botton = HIGH;
  pinMode(pinBoton, INPUT_PULLUP);
}



/*
MIDI.getType(): Identifica el tipo de mensaje MIDI (por ejemplo, si es una "nota encendida" o "nota apagada").
MIDI.getData1(): Devuelve el número de la nota MIDI (por ejemplo, 60 para la nota C4).
MIDI.getData2(): Devuelve la velocidad de la nota, es decir, cuán fuerte se tocó la tecla.
*/

void loop() {

  
 if(millis() - tiempo >= 20){
    valorX = analogRead(pinX);
    valorY = analogRead(pinY);
    botton = digitalRead(pinBoton);
    
    if(millis() - tiempo2 >= 500){
         if(valorX > 1000){
        controladorMotores.cambiarEdoCiclicoReloj();
        tiempo2 = millis();
      }
      if(valorX < 50){
          controladorMotores.cambiarEdoAntiReloj();
          tiempo2 = millis();
      }
       if(botton == LOW){
        
        controladorMotores.setEstrategia(melo);
        tiempo2 = millis();
      }
    
    }
      tiempo = millis();
  
  }

  controladorMotores.tocarTodos(valorY);
  
  
  if (MIDI.read()) {
    if (MIDI.getType() == midi::NoteOn) {
      int note = MIDI.getData1();     
      controladorMotores.cambiarNota(note);
    }
    else if (MIDI.getType() == midi::NoteOff) {
      int note = MIDI.getData1();
      controladorMotores.detenerNota(note); 
    } 
  }
}
