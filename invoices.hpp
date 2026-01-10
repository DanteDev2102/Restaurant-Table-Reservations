#pragma once

#include <string>
#include "orders.hpp"

using namespace std;

class Invoice {
private:
    string dni;
    string clientName;
    int table;
    string day;
    Orders orders;
    double total;

public:
    Invoice();
    Invoice(string dni, string clientName, int table, string day, Orders orders, double total);
    
    Invoice(const Invoice& other);              // Constructor de copia
    Invoice& operator=(const Invoice& other);   // Operador de asignación

    // Getters
    string getDni() const;
    string getClientName() const;
    int getTable() const;
    string getDay() const;
    Orders& getOrders();
    double getTotal() const;

    // Setters
    void setDni(const string& dni);
    void setClientName(const string& name);
    void setTable(int table);
    void setDay(const string& day);
    void setOrders(const Orders& orders);
    void setTotal(double total);
};

class InvoiceNode {
private:
    Invoice info;
    InvoiceNode* next;

    friend class Invoices;
};

class Invoices {
    typedef InvoiceNode* PtrInvoice;

private:
    PtrInvoice first;

public:
    Invoices();
    ~Invoices();

    // Acceso a la lista
    PtrInvoice getFirst();
    void setFirst(PtrInvoice p);

    // Operaciones basicas
    bool isEmpty();
    bool isFull();
    bool insertAtBeginning(const Invoice& value);
    bool removeFromBeginning(Invoice& value);
    bool insertAfter(PtrInvoice p, const Invoice& value);
    bool removeAfter(PtrInvoice p, Invoice& value);

    // Acceso a nodos
    PtrInvoice getNext(PtrInvoice p);
    void setNext(PtrInvoice p, PtrInvoice q);
    Invoice getInfo(PtrInvoice p);
    void setInfo(PtrInvoice p, const Invoice& value);

    // Utilidades
    int count();
    PtrInvoice searchByDni(const string& dni);
};

