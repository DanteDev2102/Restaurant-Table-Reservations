#pragma once

#include <string>
#include "orders.hpp"

using namespace std;

struct Client {
    string dni;
    string name;
    int table;
    string day;
    Orders orders;
    double total;

    Client() : table(0), total(0.0) {}
    Client(string dni, string name, int table, string day)
        : dni(dni), name(name), table(table), day(day), total(0.0) {}
};

class ClientNode {
    Client info;
    ClientNode* next;
    friend class Clients;
};

class Clients {
    typedef ClientNode* PtrClient;

private:
    PtrClient front;
    PtrClient rear;

public:
    Clients(); //contructor
    ~Clients(); //destructor

    bool isEmpty();
    bool isFull();
    bool enqueue(Client value);
    bool dequeue(Client& value);
    PtrClient getFront();
    PtrClient getRear();
    Client getInfo(ClientNode* p);
    void setInfo(ClientNode* p, Client value);
};

