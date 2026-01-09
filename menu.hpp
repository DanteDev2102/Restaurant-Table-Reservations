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

// Tamano del menu
const int MENU_SIZE = 5;

// Arreglo estatico de objetos MenuItem
const MenuItem MENU[MENU_SIZE] = {
    MenuItem(1, "Pabellon", 15.0),
    MenuItem(2, "Plato Navideno", 20.0),
    MenuItem(3, "Pollo Asado", 12.0),
    MenuItem(4, "Mondongo", 7.0),
    MenuItem(5, "Pasticho", 10.0)
};

// Funcion para buscar un plato por codigo
inline const MenuItem* findMenuItem(int code) {
    for (int i = 0; i < MENU_SIZE; ++i) {
        if (MENU[i].getCode() == code) {
            return &MENU[i];
        }
    }
    return nullptr;
}

