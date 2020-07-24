#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <map>


// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion = actIDLE;
	// Estoy en el nivel 1

	actual.fila        = sensores.posF;
	actual.columna     = sensores.posC;
	actual.orientacion = sensores.sentido;

	cout << "Fila: " << actual.fila << endl;
	cout << "Col : " << actual.columna << endl;
	cout << "Ori : " << actual.orientacion << endl;


/*	destino.fila       = sensores.destinoF;
	destino.columna    = sensores.destinoC;
*/
 	int filaD = sensores.destinoF;
	int colD = sensores.destinoC;

	if (filaD != destino.fila || colD != destino.columna){
        hay_plan = false;
        destino.fila =  filaD;
        destino.columna = colD;

    }



    if(sensores.nivel != 4){ //estoy en el nivel 1

        if (!hay_plan){
            hay_plan = pathFinding (sensores.nivel, actual, destino, plan);
        }

        if (hay_plan && plan.size()>0){
            accion = plan.front();
            plan.erase(plan.begin());
        }

    } else { //estoy en el nivel 2


        if(charge && actual.fila == bateria.fila && actual.columna == bateria.columna){ //se esta cargando
            if(sensores.bateria < 1600){
                plan.push_front(actIDLE);
                charging = true;
            } else {
                charge = false;
                charging = false;
                hay_plan = false;
            }
        }



        if(!hay_plan && charge && !charging){ //busca camino hasta la bateria
           hay_plan = pathFinding (sensores.nivel, actual, bateria, plan);
           pathKnown = pasarPorDesconosido(actual, plan);
        } else if (!hay_plan && !charge) { // busca camino hasta destino
            hay_plan = pathFinding (sensores.nivel, actual, destino, plan);
         //proba
            stupki = 0;
            steps = 0;
            pathKnown = pasarPorDesconosido(actual, plan);
		    //proba

        }
         else if (plan.size()==0){
            hay_plan = pathFinding (sensores.nivel, actual, destino, plan);
        }


 /*       if(!tengoBikini){

         if(sensores.terreno[1] == 'K'){
            plan.clear();
            plan.push_back(actFORWARD);
            plan.push_back(actTURN_L);
            plan.push_back(actFORWARD);
            hay_plan = true;
            tengoBikini = true;
            stupki = 2;
            steps = 0;

        } else if(sensores.terreno[3] == 'K'){
            plan.clear();
            plan.push_back(actFORWARD);
            plan.push_back(actTURN_R);
            plan.push_back(actFORWARD);
            hay_plan = true;
            tengoBikini = true;
            stupki = 2;
            steps = 0;

        } else if(sensores.terreno[4] == 'K'){
            plan.clear();
            plan.push_front(actFORWARD);
            plan.push_front(actFORWARD);
            plan.push_front(actTURN_L);
            plan.push_front(actFORWARD);
            plan.push_front(actFORWARD);
            hay_plan = true;
            tengoBikini = true;
            stupki = 4;
            steps = 0;

        } else if(sensores.terreno[8] == 'K'){
            plan.clear();
            plan.push_front(actFORWARD);
            plan.push_front(actFORWARD);
            plan.push_front(actTURN_R);
            plan.push_front(actFORWARD);
            plan.push_front(actFORWARD);
            hay_plan = true;
            tengoBikini = true;
            stupki = 4;
            steps = 0;
        }

        }

*/
        if(mapaResultado[actual.fila][actual.columna] == 'K'){
            tengoBikini = true;
        }

        if(!tengoBikini && findBikini){ //si encuentra bikini con los sensores y no tiene los toma
            hay_plan = pathFinding (sensores.nivel, actual, bikiniPos, plan);
  //          tengoBikini = true;
            pathKnown = true;
        }

        if(mapaResultado[actual.fila][actual.columna] == 'D'){
            tengoZapatillas = true;
        }

        if(!tengoZapatillas && findZapatillas){ //si encuentra zapatillas con los sensores y no tiene los toma
            hay_plan = pathFinding (sensores.nivel, actual, zapatillasPos, plan);
  //          tengoZapatillas = true; //para hacerlo solo una vez
            pathKnown = true; //no es necesario recalcular
        }


        if(sensores.superficie[2] == 'a'){ //avoiding colisiones with aldeanos
            if(plan.front() == actFORWARD){
            plan.push_front(actIDLE);
            }
        }



        if (hay_plan && plan.size()>0){
            accion = plan.front();
            plan.erase(plan.begin());
            actualizarMapa(actual, sensores.terreno);
       }


        if(HayObstaculoDelante(actual)){ //if it cannot pass, it recalculates
            if(accion == actFORWARD){
                accion = actIDLE;
                hay_plan = false;
            }
        }



 /*   if (!pathKnown && steps >=3){ //recalcularing plan every 3 actions to avoid very expensive paths
            steps = 0;
                hay_plan = false;
        }
*/

         if(steps >= stupki && !pathKnown){ //recalculating when it reaches casilla '?'
            hay_plan = false;
            steps = 0;
         }


        if(sensores.bateria < 500 && bateria.fila != -1 && bateria.columna != -1 && !charge){ //se da cuenta de que se tiene que cargar y no está a camino de cargarse
            charge = true;
            hay_plan = false;
        }



        steps++;
    } // end nivel 2


    return accion;

}



// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			      return pathFinding_Profundidad(origen,destino,plan);
						break;
		case 2: cout << "Busqueda en Anchura\n";
			      // Incluir aqui la llamada al busqueda en anchura
			      return pathFinding_Anchura(origen,destino,plan);
						break;
		case 3: cout << "Busqueda Costo Uniforme\n";
						// Incluir aqui la llamada al busqueda de costo uniforme
                  return pathFinding_CostoUniforme(origen,destino,plan);
						break;
		case 4: cout << "Busqueda para el reto\n";
						// Incluir aqui la llamada al algoritmo de búsqueda usado en el nivel 2
						return pathFinding_CostoUniforme(origen,destino,plan);
						break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}


//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break; //norte
		case 1: col++; break; //este
		case 2: fil++; break; //sure
		case 3: col--; break; //oeste
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}




struct nodo{
	estado st;
	list<Action> secuencia;
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

//Implementacion de la busqueda en anchura

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	queue<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.front();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

//Implementacion de la busqueda con consto uniforme

struct nodoCosto{
	estado st;
	list<Action> secuencia;
	int costo;
};



int ComportamientoJugador::calculaCoste(unsigned char tipoTerreno){
    int costo;
    switch(tipoTerreno){
        case 'A':{
            if(bikini){
                costo = 10;
            } else {
                costo = 100;
            } break;
        }
        case 'B':{
            if(zapatillas){
                costo = 5;
            } else {
                costo = 50;
            } break;
        }
        case 'T': costo = 2; break;
        case '?':{
            if(zapatillas && bikini){
                costo = 4.3;
            } else if (!zapatillas && bikini){
                costo = 5;
            } else if (zapatillas && !bikini){
                costo = 5;
            } else {
                costo = 5;
            } break;
        }
        default: costo = 1; break;
    }
    return costo;
}




bool ComportamientoJugador::pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	multimap<int,nodoCosto> pila;										// Lista de Abiertos

  nodoCosto current;
	current.st = origen;
	current.secuencia.empty();
    current.costo = 0;
    pair<int, nodoCosto> casilla = {current.costo,current};

    if(mapaResultado[current.st.fila][current.st.columna]=='D'){
        zapatillas = true;
    }

    if(mapaResultado[current.st.fila][current.st.columna]=='K'){
        bikini = true;
    }


	pila.insert(casilla);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){





		pila.erase(pila.begin());
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodoCosto hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			hijoTurnR.costo += calculaCoste(mapaResultado[hijoTurnR.st.fila][hijoTurnR.st.columna]);
			casilla = {hijoTurnR.costo, hijoTurnR};

			bool existe = false;
			auto it = pila.begin();
			while (it != pila.end() && !existe){
                if(hijoTurnR.st.fila == (*it).second.st.fila &&
                   hijoTurnR.st.columna == (*it).second.st.columna &&
                   hijoTurnR.st.orientacion == (*it).second.st.orientacion){
                    existe = true;

                    if (hijoTurnR.costo < (*it).first){
                        pila.erase(it);
                        pila.insert(casilla);
                    }
                }
                it++;
			}
            if(!existe){
               pila.insert(casilla);
            }
		}

		// Generar descendiente de girar a la izquierda
		nodoCosto hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			hijoTurnL.costo += calculaCoste(mapaResultado[hijoTurnL.st.fila][hijoTurnL.st.columna]);
			casilla = {hijoTurnL.costo, hijoTurnL};

            bool existe = false;
			auto it = pila.begin();
			while (it != pila.end() && !existe){
                if(hijoTurnL.st.fila == (*it).second.st.fila &&
                   hijoTurnL.st.columna == (*it).second.st.columna &&
                   hijoTurnL.st.orientacion == (*it).second.st.orientacion){
                    existe = true;

                    if (hijoTurnL.costo < (*it).first){
                        pila.erase(it);
                        pila.insert(casilla);
                    }
                }
                it++;
			}
            if(!existe){
               pila.insert(casilla);
            }

		}




		// Generar descendiente de avanzar
		nodoCosto hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				hijoForward.costo += calculaCoste(mapaResultado[hijoForward.st.fila][hijoForward.st.columna]);
				casilla = {hijoForward.costo, hijoForward};

            bool existe = false;
			auto it = pila.begin();
			while (it != pila.end() && !existe){
                if(hijoForward.st.fila == (*it).second.st.fila &&
                   hijoForward.st.columna == (*it).second.st.columna &&
                   hijoForward.st.orientacion == (*it).second.st.orientacion){
                    existe = true;

                    if (hijoForward.costo < (*it).first){
                        pila.erase(it);
                        pila.insert(casilla);
                    }
                }
                it++;
			}
            if(!existe){
               pila.insert(casilla);
            }
			}

		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = (*pila.begin()).second;
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

void ComportamientoJugador::actualizarMapa(estado &st, vector<unsigned char> vector){

	mapaResultado[st.fila][st.columna] = vector[0]; //casilla actual

        int i = 1;
        int nivel = 1;
        int veces = 3;
        while(i < 16){

            for(int j = 1; j <= veces; j++){
                switch(st.orientacion){
                    case 0:{
                        mapaResultado[st.fila-nivel][st.columna+j+nivel-veces] = vector[i];
                        if(vector[i] == 'X'){
                            bateria.fila = st.fila-nivel;
                            bateria.columna = st.columna+j+nivel-veces;
                        }
                         else if (vector[i] == 'K'){
                            findBikini = true;
                            bikiniPos.fila = st.fila-nivel;
                            bikiniPos.columna = st.columna+j+nivel-veces;
                        }
                        else if (vector[i] == 'K'){
                            findZapatillas = true;
                            zapatillasPos.fila = st.fila-nivel;
                            zapatillasPos.columna = st.columna+j+nivel-veces;
                        }
                        break;
                    }
                    case 1:{
                        mapaResultado[st.fila+j+nivel-veces][st.columna+nivel] = vector[i];
                        if(vector[i] == 'X'){
                            bateria.fila = st.fila+j+nivel-veces;
                            bateria.columna = st.columna+nivel;
                        }
                        else if (vector[i] == 'K'){
                            findBikini = true;
                            bikiniPos.fila = st.fila+j+nivel-veces;
                            bikiniPos.columna = st.columna+nivel;
                        }
                        else if (vector[i] == 'D'){
                            findZapatillas = true;
                            zapatillasPos.fila = st.fila+j+nivel-veces;
                            zapatillasPos.columna = st.columna+nivel;
                        }
                        break;
                    }
                    case 2:{
                        mapaResultado[st.fila+nivel][st.columna-(j+nivel-veces)] = vector[i];
                        if(vector[i] == 'X'){
                            bateria.fila = st.fila+nivel;
                            bateria.columna = st.columna-(j+nivel-veces);
                        }
                        else if (vector[i] == 'K'){
                            findBikini = true;
                            bikiniPos.fila = st.fila+nivel;
                            bikiniPos.columna = st.columna-(j+nivel-veces);
                        }
                        else if (vector[i] == 'D'){
                            findZapatillas = true;
                            zapatillasPos.fila = st.fila+nivel;
                            zapatillasPos.columna = st.columna-(j+nivel-veces);
                        }
                        break;
                    }
                    case 3:{
                        mapaResultado[st.fila-(j+nivel-veces)][st.columna-nivel] = vector[i];
                        if(vector[i] == 'X'){
                            bateria.fila = st.fila-(j+nivel-veces);
                            bateria.columna = st.columna-nivel;
                        }
                        else if (vector[i] == 'K'){
                            findBikini = true;
                            bikiniPos.fila = st.fila-(j+nivel-veces);
                            bikiniPos.columna = st.columna-nivel;
                        }
                        else if (vector[i] == 'D'){
                            findZapatillas = true;
                            zapatillasPos.fila = st.fila-(j+nivel-veces);
                            zapatillasPos.columna = st.columna-nivel;
                        }

                        break;
                    }
                }
                i++;
            }
            nivel++;
            veces += 2;

    }
}







// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

// Pinta sobre el mapa del juego el plan obtenido
bool ComportamientoJugador::pasarPorDesconosido(const estado &st, const list<Action> &plan){

	estado cst = st;


	bool proba = true;
    int i = 0;
	auto it = plan.begin();
	while (it!=plan.end() && proba){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}

        if(mapaResultado[cst.fila][cst.columna] == '?'){
            proba = false;
            stupki = i;
        }
     i++;
	 it++;
    }
    return proba;
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
