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

bool checkReservationData(int table, int qty, string name, string dni, string date);

// Para lectura de datos de entrada
int readIntergers(const string &message, int min, int max);
string readAlphaString(const string &message);
string readDNI(const string &message);
bool isValidDay(const string &day);
string readValidDay(const string &message);



