#pragma once

#include <string>
#include "orders.hpp"

using namespace std;

class ClientNode; // NUEVO 
typedef ClientNode* PtrClient; // SUBI ESTO

class Client {
private:
    string dni;
    string name;
    int table;
    string day;
    Orders orders;
    double total;

public:
    Client();
    Client(string dni, string name, int table, string day);

    // Getters
    string getDni() const;
    string getName() const;
    int getTable() const;
    string getDay() const;
    Orders& getOrders();
    double getTotal() const;

    // Setters
    void setDni(const string& dni);
    void setName(const string& name);
    void setTable(int table);
    void setDay(const string& day);
    void setOrders(const Orders& orders);
    void setTotal(double total);
};

class ClientNode {
    Client info;
    ClientNode* next;
    friend class Clients;
};

class Clients {
    //typedef ClientNode* PtrClient;

private:
    PtrClient front;
    PtrClient rear;

public:
    Clients();
    ~Clients();

    bool isEmpty();
    bool isFull();
    bool enqueue(const Client& value);
    bool dequeue(Client& value);
    PtrClient getFront();
    PtrClient getRear();
    Client& getInfo(ClientNode* p);
    void setInfo(ClientNode* p, const Client& value);
    void showQueue(bool isWaitingList);
    bool isTableOccupied(int tableNum);
    bool isClientInList(string dniCheck);
    ClientNode* getNext(ClientNode* p) const;
    void setNext(ClientNode* p, ClientNode* nextNode);
};

