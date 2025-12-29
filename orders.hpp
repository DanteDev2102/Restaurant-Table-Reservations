#pragma once

#include <string>
#include "menu.hpp"

using namespace std;

struct Order {
    int dishCode;
    double price;
    string notes;

    Order() : dishCode(0), price(0.0), notes("") {}
    Order(int code, double price, string notes)
        : dishCode(code), price(price), notes(notes) {}
};

class OrderNode {
public:
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

    bool isEmpty();
    bool isFull();
    bool push(int dishCode, const string& notes);
    bool pop(Order& value);
    OrderPtr getTop();
    void setTop(OrderPtr p);
    Order getInfo(OrderNode* p);
    void setInfo(OrderNode* p, Order value);
    double totalPrice();
    int size();
};

