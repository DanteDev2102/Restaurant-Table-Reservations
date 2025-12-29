#pragma once

#include <string>
#include "orders.hpp"

using namespace std;

struct Invoice {
    string dni;
    string clientName;
    int table;
    string day;
    Orders orders;
    double total;

    Invoice() : table(0), total(0.0) {}
    Invoice(string dni, string clientName, int table, string day, Orders orders, double total)
        : dni(dni), clientName(clientName), table(table), day(day), orders(orders), total(total) {}
};

class InvoiceNode {
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

    PtrInvoice getFirst();
    void setFirst(PtrInvoice p);

    bool isEmpty();
    bool isFull();
    bool insertAtBeginning(Invoice value);
    bool removeFromBeginning(Invoice& value);
    bool insertAfter(PtrInvoice p, Invoice value);
    bool removeAfter(PtrInvoice p, Invoice& value);
    PtrInvoice getNext(PtrInvoice p);
    void setNext(PtrInvoice p, PtrInvoice q);
    Invoice getInfo(PtrInvoice p);
    void setInfo(PtrInvoice p, Invoice value);
    int count();
    PtrInvoice searchByDni(const string& dni);
};

