#pragma once

#include <string>
#include "menu.hpp"

using namespace std;

class Order {
private:
    int dishCode;
    double price;
    string notes;

public:
    Order();
    Order(int code, double price, const string& notes);

    // Getters
    int getDishCode() const;
    double getPrice() const;
    string getNotes() const;

    // Setters
    void setDishCode(int code);
    void setPrice(double price);
    void setNotes(const string& notes);
};

class OrderNode {
private:
    Order info;
    OrderNode* next;

    friend class Orders;
};

class Orders {
    typedef OrderNode* OrderPtr;

private:
    OrderPtr top;

public:
    Orders();
    ~Orders();

    bool isEmpty();
    bool isFull();
    bool push(int dishCode, const string& notes);
    bool push2(int dishCode, double price, const string& notes);

    bool pop(Order& value);

    OrderPtr getTop();
    void setTop(OrderPtr p);

    Order getInfo(OrderNode* p);
    void setInfo(OrderNode* p, const Order& value);

    double totalPrice();
    int size();
    
    //modificarPlato
    bool modifyDish(int oldCodeDish, int newCodeDish, const std::string& newNotes);
    
    // verifica si la pila contiene el plato
    bool containsDish(int code) const;


};

