#include <algorithm>
#include <cctype>
#include "utility.hpp"
#include "../reservation.hpp"
#include <iostream>

using namespace std;

// Auxiliary functions
bool isAlphabetic(const string &cadena){
	for(int i = 0; i < cadena.length(); i++){
		if(!isalpha(cadena[i])) return false;
	}
	return !cadena.empty();
}

bool isNumeric(const string &cadena){
	for (int i = 0; i < cadena.length(); i++){
		if (!isdigit(cadena[i])) return false;
	}
	return !cadena.empty();
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Convierte un String la primera letra Mayuscula y el resto minuscula
string capitalize(const string& s) {
    if (s.empty()) return s;
    string out = s;
    out[0] = toupper(out[0]);
    for (size_t i = 1; i < out.size(); ++i)
        out[i] = tolower(out[i]);
    return out;
}


// Mostrar Reservas por dia
void mostrarReservasPorDia(Reservations& lista, const string& diaBuscado) {

    int contador = 0;
    Reservation* p = lista.getFirst();
    while (p != nullptr) {
        string dia = toLower(p->getDate());
        if (dia == toLower(diaBuscado)) {
            ++contador;
        }
        p = p->getNext();
    }

    cout << capitalize(diaBuscado) << ": " << contador << endl;

    if (contador > 0) {
        cout << "--- Reservas para " << capitalize(diaBuscado) << " ---" << endl;
        p = lista.getFirst();
        while (p != nullptr) {
            string dia = toLower(p->getDate());
            if (dia == toLower(diaBuscado)) {
                cout << "Numero de mesa: " << p->getTable() << endl;
                cout << "Nombre del cliente: " << p->getName() << endl;
                cout << "Cedula del cliente: " << p->getDni() << endl;
                cout << "Dia de la reserva: " << p->getDate() << endl;
                cout << "Cantidad de personas: " << p->getQty() << endl;
                cout << "------------------------------" << endl;
            }
            p = p->getNext();
        }
    }
}

