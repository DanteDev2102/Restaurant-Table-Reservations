#pragma once
#include <string>
#include "../reservation.hpp"

using namespace std;

bool isAlphabetic(const string &cadena);
bool isNumeric(const string &cadena);

// Convierte un string a minúsculas
string toLower(const string& str);

// Convierte un String la primera letra Mayuscula y el resto minuscula
string capitalize(const string& str);

// Mostrar Reservas por dia
void mostrarReservasPorDia(Reservations& lista, const string& diaBuscado);




