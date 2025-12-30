#pragma once

#include <string>
using namespace std;

// Arreglo estático con los 5 platos codigo, nombre y precio
struct MenuItem {
    int code;
    string name;
    double price;
};

// Valores fijos Arreglo estático con los 5 platos codigo, nombre y precio
const int MENU_SIZE = 5;
const MenuItem MENU[MENU_SIZE] = {
    {1, "Pabellón", 15.0},
    {2, "Plato Navideño", 20.0},
    {3, "Pollo Asado", 12.0},
    {4, "Mondongo", 7.0},
    {5, "Pasticho", 10.0}
};

// Función para buscar un plato por código
inline const MenuItem* findMenuItem(int code) {
    for (int i = 0; i < MENU_SIZE; ++i) {
        if (MENU[i].code == code) {
            return &MENU[i];
        }
    }
    return nullptr;
}

