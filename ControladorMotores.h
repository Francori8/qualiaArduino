
class ControladorMotores{
  private:
    Motor* motores[4];
    EstrategiaDeMotor* estrategia;
    int edosDisponibles[4] = {5,12, 19, 31};
    
    int posicionEdo= 1;
  
  
  public:
     ControladorMotores(Motor& m1, Motor& m2, Motor& m3, Motor& m4, EstrategiaDeMotor& estra) {
        motores[0] = &m1;
        motores[1] = &m2;
        motores[2] = &m3;
        motores[3] = &m4;
        estrategia = &estra;
        

        cambiarEdoTodos(edosDisponibles[posicionEdo]);
    }

    void setEstrategia(EstrategiaDeMotor& estra){
      estrategia = &estra;
    }



    bool hayAlgunaNota(){
         int n = 0;
        while(n < 4 && motores[n]->estaTocando()){
            n++;
        }

        return n < 4 && !motores[n]->estaTocando();
    }

    void cambiarNota(int note){
      estrategia->asignarNota(note, motores);
      
    }
    void aplicarDesfaseATodos(int desfase){
      for(int n = 0 ; n < 4 ; n ++){
        motores[n]->aplicarDesfase(desfase);
      }
    }
    void tocarTodos(int desfase){
        aplicarDesfaseATodos(desfase);
        estrategia->tocarNotas(motores);  
    }

    void detenerNota(int note){
      estrategia->soltarNota(note, motores);
    }
    void cambiarEdoTodos(int edo){
        motores[0]->cambiarEdo(edo);
        motores[1]->cambiarEdo(edo);
        motores[2]->cambiarEdo(edo);
        motores[3]->cambiarEdo(edo);
    }

    void cambiarEdoCiclicoReloj(){
      posicionEdo++;
      if(posicionEdo == 4){
          posicionEdo = 0;
      }
      
      cambiarEdoTodos(edosDisponibles[posicionEdo]);
    }

    int edoActual(){
      return edosDisponibles[posicionEdo];
    }

    void cambiarEdoAntiReloj(){
      posicionEdo--;
      if(posicionEdo == -1){
          posicionEdo = 3;
      }
      cambiarEdoTodos(edosDisponibles[posicionEdo]);
    }
};