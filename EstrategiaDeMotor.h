
class EstrategiaDeMotor {
    public:
    virtual void soltarNota(int note, Motor* motores[]);
    virtual void asignarNota(int note, Motor* motores[]);
    virtual void tocarNotas(Motor* motores[]);
};


class Coral : public EstrategiaDeMotor {
  private:
    int notaGuardada[2] = {-1, -1};

   bool sePuedeTocar(Motor* motores[]){
    int n = 0;
    while(n < 4 && motores[n]->estaTocando()){
        n++;
    }

    return n < 4 && !motores[n]->estaTocando();
  }

  Motor* motorDisponible(Motor* motores[]){
    int n = 0;
    while(n < 4 && motores[n]->estaTocando()){
        n++;
    }

    return motores[n];
  }

  Motor* estaTocandoNota(int note, Motor* motores[]){
      int n = 0;
    while(n < 4 && motores[n]->notaActual() != note){
        n++;
    }

    return motores[n];
  }

  Motor* primerMotorUsando(Motor* motores[]){
    Motor* moto = motores[0];
    for(int n = 0 ; n < 4  ; n++){
      if(moto->tiempo() > motores[n]->tiempo()){
        moto = motores[n];
      }
    }

    return moto;
  }
  
  bool notaSiendoTocada(int note, Motor* motores[]){
      int n = 0;
    while(n < 4 && motores[n]->notaActual() != note){
        n++;
    }

    return n < 4 && motores[n]->notaActual() == note;
  }

  public:
  
  void asignarNota(int note, Motor* motores[]){
    if(sePuedeTocar(motores)){
      motorDisponible(motores)->asignarNota(note);
    }else{
      Motor* moto = primerMotorUsando(motores);
      if(notaGuardada[0] == -1){
        notaGuardada[0] = moto->notaActual();  
      }else{
        notaGuardada[1] = moto->notaActual();  
      }
          
      moto->asignarNota(note);
    }
  }

  void tocarNotas(Motor* motores[]){
    for(int n = 0; n < 4 ; n++){
      motores[n]->tocar();
    }
  }

  void soltarNota(int note, Motor* motores[]){
      if(notaSiendoTocada(note, motores)){
        Motor* moto = estaTocandoNota(note, motores);
        moto->parar();
        if(notaGuardada[0] != -1){
          moto->asignarNota(notaGuardada[0]);
          notaGuardada[0] = notaGuardada[1];
          notaGuardada[1] =-1;
        }


      }else{
        if(notaGuardada[0] == note){
            notaGuardada[0] = notaGuardada[1];
        }
          notaGuardada[1] =-1;
      }
      
  }
};


class Melodica : public EstrategiaDeMotor {
  private:
    int motorTocando = -1;
    int motorProximo = 0;
    bool enCambio = false;
    bool solte = false;

  bool noHayMotoresConNotas(Motor* motores[]){
    int n = 0;
    while(n < 4 && !motores[n]->estaTocando()){
        n++;
    }

    return n < 4 && motores[n]->estaTocando();
  }

  
  bool notaSiendoTocada(int note, Motor* motores[]){
      int n = 0;
    while(n < 4 && motores[n]->notaActual() != note){
        n++;
    }

    return n < 4 && motores[n]->notaActual() == note;
  }

    Motor* estaTocandoNota(int note, Motor* motores[]){
      int n = 0;
    while(n < 4 && motores[n]->notaActual() != note){
        n++;
    }

    return motores[n];
  }

   float velocidadAActualizar(Motor* motores[]){
      int notaActual = motores[motorTocando]->notaActual();
      int notaProxima = motores[motorProximo]->notaActual();
      float velocidad;
      if(notaActual > notaProxima){
          velocidad = (notaActual - notaProxima)  *-0.01;
           
      }else{
          velocidad = (notaProxima - notaActual) * 0.01 ;
      }

      return velocidad;

   }

  public:
  void asignarNota(int note, Motor* motores[]){
    if(motorTocando == -1){
        motorTocando = motorProximo;
        motorProximo = (motorProximo + 1) % 4 ;
        motores[motorTocando ]->asignarNota(note);
    } else{
      enCambio = true;
      motores[motorProximo]->asignarNota(note);
    }
    
    
  }

  void tocarNotas(Motor* motores[]){
    if(enCambio){
        //float constante = (motores[motorProximo]->notaActual() - motores[motorTocando]->notaActual()) * 0.01 ;
        float constante = (motores[motorProximo]->velocidadOriginal() - motores[motorTocando]->velocidadOriginal() ) * 0.0007 ;
        bool condicion = motores[motorTocando]->velocidadActual() >= motores[motorProximo]->velocidadActual() ;
        if(motores[motorTocando]->notaActual() > motores[motorProximo]->notaActual()){
          
          condicion = !condicion;
        }
        motores[motorTocando]->cambiarVelocidad(motores[motorTocando]->velocidadActual() + constante);
        if(condicion){ 
            enCambio = false;
            motores[motorTocando]->parar();
            motorTocando = motorProximo;
            motorProximo = (motorProximo + 1) % 4;
        }
    }
    if(motorTocando != -1){
        motores[motorTocando]->tocar();
    }
    
  }

  void soltarNota(int note, Motor* motores[]){
    if(notaSiendoTocada(note, motores) && !enCambio){
         estaTocandoNota(note, motores)->parar();
         motorTocando = -1;
    }

    if(motores[motorTocando]->notaActual() == note && enCambio){
      solte = true;
    }
    

    if(motores[motorProximo]->notaActual() == note && enCambio){
        if(solte){
          motores[motorProximo]->parar();
          motores[motorTocando]->parar();
          motorTocando = -1;
        }else{
          motores[motorProximo]->parar();
        motores[motorTocando]->asignarNota(motores[motorTocando]->notaActual());
         
        }
        enCambio = false;
         solte = false;
             
    }
    
  }
};

class Tutti : public EstrategiaDeMotor {
  private:
  int cantidadNotasTocando = 0;
  int notaGuardada = -1;
  Motor* estaTocandoNota(int note, Motor* motores[]){
      int n = 0;
    while(n < 4 && motores[n]->notaActual() != note){
        n++;
    }

    return motores[n];
  }

  int numeroDeMotorToca(int note, Motor* motores[]){
      int n = 0;
    while(n < 4 && motores[n]->notaActual() != note){
        n++;
    }

    return n;
  }

  void soltarCon4notas(int numeroDeMotor, Motor* motores[]){
      //por precondicion en este caso, la mas grave puede estar en la  posicion 1 o 2
          //Si suelto la 2 obligatoriamente la 1 es la mas grave, 
          //entonces tengo que poner nota de la 1 en al 2 tambien
          if(numeroDeMotor == 2){
            motores[2]->asignarNota(motores[1]->notaActual());
          }else if(numeroDeMotor == 1){
            // Si suelto la 1, cualquiera puede ser la mas grave, averiguar cual es mas grave, 
            //si es dos replico en 1, si es otra, la paso a la 1 y 2 y la 2 la paso a donde estaba antes
              int nota0 = motores[0]->notaActual();
              int nota2 = motores[2]->notaActual();
              int nota3 = motores[3]->notaActual();

              if(nota2 < nota0 && nota2 < nota3){
                motores[1]->asignarNota(nota2);
              }else if(nota0 < nota2 && nota0 < nota3){
                motores[0]->asignarNota(nota2);
                motores[1]->asignarNota(nota0);
                motores[2]->asignarNota(nota0);
              }else{
                  motores[3]->asignarNota(nota2);
                  motores[1]->asignarNota(nota3);
                  motores[2]->asignarNota(nota3);
              }
              


          }else{
              // Si suelto la 0 o 3 , la mas grave puede ser 1 o 2, averiguar cual es mas grave,  
              //replicarla en al otra y la nota que estaba antes colocarla en el motor que se solto
            int nota1 = motores[1]->notaActual();
            int nota2 = motores[2]->notaActual();

            if(nota1 < nota2){
                motores[2]->asignarNota(nota1);
                motores[numeroDeMotor]->asignarNota(nota2);
            }else{
                motores[1]->asignarNota(nota2);
                motores[numeroDeMotor]->asignarNota(nota1);
            }

          }

  }
  

  public:
  void asignarNota(int note, Motor* motores[]){
      if(cantidadNotasTocando == 0){
          for(int n = 0; n < 4 ; n++){
            motores[n]->asignarNota(note);
          }
          cantidadNotasTocando++ ;
      }else if(cantidadNotasTocando == 1){
          motores[1]->asignarNota(note);
          motores[3]->asignarNota(note);
          cantidadNotasTocando++ ;
      }else if(cantidadNotasTocando == 2){
          int nota0 = motores[0]->notaActual();
          int nota1 = motores[1]->notaActual();

          if(note < nota0 && note < nota1){
            motores[1]->asignarNota(note);
            motores[2]->asignarNota(note);   
          }
          
          if(nota0 < nota1 && nota0 < note){
            motores[0]->asignarNota(note);
            motores[1]->asignarNota(nota0); 
          }

          if(nota1 < nota0 && nota1 < note){
            motores[2]->asignarNota(nota1);
            motores[3]->asignarNota(note); 
          }
          cantidadNotasTocando++ ;
      }else if(cantidadNotasTocando == 3){
           motores[2]->asignarNota(note);
           cantidadNotasTocando++ ;
      }else if(cantidadNotasTocando == 4){
          notaGuardada = note;
          cantidadNotasTocando++ ;
          
      }
  }

  void tocarNotas(Motor* motores[]){
      for(int n = 0; n < 4 ; n++){
           motores[n]->tocar();
    }
  }

  void soltarNota(int note , Motor* motores[]){
    if(cantidadNotasTocando == 1){
         for(int n = 0; n < 4 ; n++){
           motores[n]->parar();
        }
        cantidadNotasTocando-- ;
    }else if(cantidadNotasTocando ==2){
        int nota0 = motores[0]->notaActual();
        int nota1 = motores[1]->notaActual(); 
        
        if(nota1 == note){
          motores[1]->asignarNota(nota0);
          motores[3]->asignarNota(nota0);
        }else{
          motores[0]->asignarNota(nota1);
          motores[2]->asignarNota(nota1);
        }


          cantidadNotasTocando-- ;
    }else if(cantidadNotasTocando == 3){
        int nota0 = motores[0]->notaActual();
        int notaGrave = motores[1]->notaActual();
        int nota3 = motores[3]->notaActual(); 
        
        if(nota0 == note){
          motores[0]->asignarNota(notaGrave);
          motores[1]->asignarNota(nota3);
        }else if(notaGrave == note){
          motores[1]->asignarNota(nota3);
          motores[2]->asignarNota(nota0);
        }else{
          motores[3]->asignarNota(notaGrave);
          motores[2]->asignarNota(nota0);
        }
        cantidadNotasTocando-- ;
    }else if(cantidadNotasTocando == 4){
          int numeroDeMotor = numeroDeMotorToca(note, motores) ;
          soltarCon4notas(numeroDeMotor, motores);
          cantidadNotasTocando-- ;
    }else if(cantidadNotasTocando == 5){
          if(note == notaGuardada){
            notaGuardada = -1;
          }else{
            estaTocandoNota( note, motores)->asignarNota(notaGuardada);
          }
          cantidadNotasTocando--;
    }
  }
};

