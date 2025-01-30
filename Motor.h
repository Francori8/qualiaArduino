
#include <AccelStepper.h>

class Motor {
private:
    AccelStepper& stepper;  // Motor paso a paso controlado por AccelStepper
    int nota;               // Nota MIDI asociada al motor
    int velocidadBase;
    float edos;
    float tiempoTocar; 
    int desfase;   // Velocidad máxima de movimiento del motor

public:
    // Constructor que toma un objeto AccelStepper por referencia
    Motor(AccelStepper& motor, int velocidadOriginal = 0) : stepper(motor), nota(-1), velocidadBase(velocidadOriginal), edos(12) , desfase(512) {}

    // Función para asignar una nota MIDI (velocidad en la que el motor debe moverse)
    void asignarNota(int notaMidi) {
        if(nota == 1){
          return;
        }
        nota = notaMidi;
        tiempoTocar = millis();
        // Aquí asignamos la velocidad del motor en función de la nota
        // Por ejemplo, podemos mapear la nota MIDI a un rango de velocidades
        velocidadBase = calcularFrecuencia(notaMidi);  
        
        stepper.setMaxSpeed(velocidadBase); 
        stepper.setSpeed(velocidadBase);    // 
    }

    float valorLogaritmicoDeNota(){
      return log(calcularFrecuencia(nota));
    }


    // Función para mover el motor (correspondiente a la "nota")
    void tocar() {
        if (nota != -1) {  // Si se ha asignado una nota
            stepper.runSpeed();  // Ejecuta el movimiento del motor
        }
    }

    float calcularFrecuencia(int notaMidi) {
        return 440.0 * pow(2.0, (notaMidi - 69) / edos);
    }
    void aplicarDesfase(int desf){
      if(desf > desfase + 10 ||  desf < desfase - 10 ){
          desfase = desf;
          int frecuenciaAlta = calcularFrecuencia(nota + 1);
          int frecuenciaActual = calcularFrecuencia(nota);
          int frecuenciaBaja = calcularFrecuencia(nota-1);
          int velocidad ;
          if(desfase > 550){
              velocidad = frecuenciaActual + (((frecuenciaAlta - frecuenciaActual) * 0.01) * desf);
          }else{
               velocidad = frecuenciaActual - (((frecuenciaActual - frecuenciaBaja) * 0.01) * desf);
          } 
          
          cambiarVelocidad(velocidad);
      }
        
    }
  
    void parar() {
        nota = -1;
        tiempoTocar =-1;
        stepper.setSpeed(0);  
    }
    
    int velocidadOriginal(){
      return velocidadBase;
    }

    bool estaTocando() {
        return nota != -1;  
    }

    int notaActual(){
      return nota;
    }

    float tiempo(){
      return tiempoTocar;
    }

    float velocidadActual(){
      return stepper.speed();
    }

    void cambiarVelocidad(float velocidad){
      stepper.setMaxSpeed(velocidad);
      stepper.setSpeed(velocidad);   
    }

    void cambiarEdo(float edo){
      edos = edo;
    }
};