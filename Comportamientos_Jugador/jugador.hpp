#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
    bateria.fila = -1;
    bateria.columna = -1;
    bateria.orientacion = -1;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
    bateria.fila = -1;
    bateria.columna = -1;
    bateria.orientacion = -1;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    estado actual, destino;
    list<Action> plan;

    bool hay_plan = false;
    bool zapatillas = false;
    bool bikini = false;
    int steps = 0;

    //variables for batteria
    estado bateria;
    bool charge = false;
    bool charging = false;

    //checking when to recalculate
    bool pathKnown = false;
    int stupki = 0;

    //variables for bikini
    bool findBikini = false;
    estado bikiniPos;
    bool tengoBikini = false;

    //variables for zapatillas
    bool findZapatillas = false;
    estado zapatillasPos;
    bool tengoZapatillas = false;



    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan);



    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);

    int calculaCoste(unsigned char tipoTerreno);
    void actualizarMapa(estado &st, vector<unsigned char> vector);
    bool pasarPorDesconosido(const estado &st, const list<Action> &plan);
    bool pathFinding_PK(int pos, list<Action> &plan);

};

#endif
