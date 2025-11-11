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

bool checkReservationData(int table, int qty, string name, string dni, string date){
	// numero de mesa o cantidad de personas negativo o mayor a 8
	if(table < 1 || qty < 1 || qty > 8 ){
		return false;
	}
	
	//Verificar cadenas
	if(!isAlphabetic(name) || !isAlphabetic(date) || !isNumeric(dni)) {
		return false;
	}
	
	return true;
}

int readIntergers(const string &message, int min, int max){
	int value;
	while(true){
		cout << message;
		cin >>value;
		
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Entrada invalida. Debe ingresar un numero entero.\n";
		} else if (value < min || value > max  ){
			cout << "Valor fuera de rango (" << min << " - " << max << "). Intente de nuevo.\n";
		} else{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
	}
}

string readAlphaString(const string &message){
	string input;
	while(true){
		cout << message;
		getline(cin, input);
		if(isAlphabetic(input)) return input;
		cout << "Entrada invalida. Solo se permiten letras sin espacios ni simbolos.\n";
	}
}

string readDNI(const string &message){
	string input;
	while(true){
		cout << message;
		getline(cin, input);
		if(isNumeric(input) && input.length() == 8 ) return input;
		cout << "Entrada invalida. La cedula debe contener solo numeros y longitud de 8 digitos \n";
	}
}

bool isValidDay(const string &day){
	static const vector<string> validDays = {
		"lunes", "martes", "miercoles", "jueves", "viernes"
	};
	for(const string &d : validDays){
		if (toLower(day) == d) return true;
	}
	return false;
}

string readValidDay(const string &message){
	string input;
	while(true) {
		cout << message;
        getline(cin, input);
        if(isAlphabetic(input) && isValidDay(input)) return input;
        cout << "Entrada invalida. Debe ingresar un dia valido (Lunes a Viernes).\n";
	}
}
