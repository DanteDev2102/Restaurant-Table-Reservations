#pragma once

#include <string>
using namespace std;

class MenuItem {
private:
    int code;
    string name;
    double price;

public:
    MenuItem();
    MenuItem(int code, const string& name, double price);

    // Getters
    int getCode() const;
    string getName() const;
    double getPrice() const;

    // Setters
    void setCode(int code);
    void setName(const string& name);
    void setPrice(double price);
};

// Tamaño del menú
const int MENU_SIZE = 5;

// Arreglo estático de objetos MenuItem
const MenuItem MENU[MENU_SIZE] = {
    MenuItem(1, "Pabellón", 15.0),
    MenuItem(2, "Plato Navideño", 20.0),
    MenuItem(3, "Pollo Asado", 12.0),
    MenuItem(4, "Mondongo", 7.0),
    MenuItem(5, "Pasticho", 10.0)
};

// Función para buscar un plato por código
inline const MenuItem* findMenuItem(int code) {
    for (int i = 0; i < MENU_SIZE; ++i) {
        if (MENU[i].getCode() == code) {
            return &MENU[i];
        }
    }
    return nullptr;
}

