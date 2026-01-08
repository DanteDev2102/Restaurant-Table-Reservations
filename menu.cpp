#include "menu.hpp"

MenuItem::MenuItem() : code(0), name(""), price(0.0) {}

MenuItem::MenuItem(int code, const string& name, double price)
    : code(code), name(name), price(price) {}
    
int MenuItem::getCode() const {
    return code;
}

double MenuItem::getPrice() const {
    return price;
}

string MenuItem::getName() const {
    return name;
}

