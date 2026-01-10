#include "invoices.hpp"

// =======================
// Implementacion de Invoice
// =======================

Invoice::Invoice() : table(0), total(0.0) {}

Invoice::Invoice(string dni, string clientName, int table, string day, Orders orders, double total)
    : dni(dni), clientName(clientName), table(table), day(day), orders(orders), total(total) {}

string Invoice::getDni() const { return dni; }
string Invoice::getClientName() const { return clientName; }
int Invoice::getTable() const { return table; }
string Invoice::getDay() const { return day; }
Orders& Invoice::getOrders() { return orders; }
double Invoice::getTotal() const { return total; }

void Invoice::setDni(const string& dni) { this->dni = dni; }
void Invoice::setClientName(const string& name) { this->clientName = name; }
void Invoice::setTable(int table) { this->table = table; }
void Invoice::setDay(const string& day) { this->day = day; }
void Invoice::setOrders(const Orders& orders) { this->orders = orders; }
void Invoice::setTotal(double total) { this->total = total; }

// =======================
// Implementacion de Invoices
// =======================

Invoices::Invoices() {
    first = nullptr;
}

Invoices::~Invoices() {
    PtrInvoice p;
    while (!isEmpty()) {
        p = first->next;
        delete first;
        first = p;
    }
}

Invoices::PtrInvoice Invoices::getFirst() {
    return first;
}

void Invoices::setFirst(PtrInvoice p) {
    first = p;
}

bool Invoices::isEmpty() {
    return first == nullptr;
}

bool Invoices::isFull() {
    PtrInvoice p = new(nothrow) InvoiceNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Invoices::insertAtBeginning(const Invoice& value) {
    if (!isFull()) {
        PtrInvoice newNode = new InvoiceNode;
        newNode->info = value;
        newNode->next = first;
        first = newNode;
        return true;
    }
    return false;
}

bool Invoices::removeFromBeginning(Invoice& value) {
    if (!isEmpty()) {
        PtrInvoice oldFirst = first;
        value = oldFirst->info;
        first = oldFirst->next;
        delete oldFirst;
        return true;
    }
    return false;
}

bool Invoices::insertAfter(PtrInvoice p, const Invoice& value) {
    if (p == nullptr || isFull()) return false;

    PtrInvoice newNode = new InvoiceNode;
    newNode->info = value;
    newNode->next = p->next;
    p->next = newNode;
    return true;
}

bool Invoices::removeAfter(PtrInvoice p, Invoice& value) {
    if (p == nullptr || p->next == nullptr) return false;

    PtrInvoice temp = p->next;
    value = temp->info;
    p->next = temp->next;
    delete temp;
    return true;
}

Invoices::PtrInvoice Invoices::getNext(PtrInvoice p) {
    return p ? p->next : nullptr;
}

void Invoices::setNext(PtrInvoice p, PtrInvoice q) {
    if (p) p->next = q;
}

Invoice Invoices::getInfo(PtrInvoice p) {
    return p->info;
}

void Invoices::setInfo(PtrInvoice p, const Invoice& value) {
    if (p) p->info = value;
}

int Invoices::count() {
    int total = 0;
    PtrInvoice current = first;
    while (current != nullptr) {
        total++;
        current = current->next;
    }
    return total;
}

Invoices::PtrInvoice Invoices::searchByDni(const string& dni) {
    PtrInvoice current = first;
    while (current != nullptr) {
        if (current->info.getDni() == dni) return current;
        current = current->next;
    }
    return nullptr;
}


Invoice::Invoice(const Invoice& other)
    : dni(other.dni),
      clientName(other.clientName),
      table(other.table),
      day(other.day),
      orders(other.orders),   //  aquí se invoca el constructor de copia profundo de Orders
      total(other.total) {}
      
      
Invoice& Invoice::operator=(const Invoice& other) {
    if (this != &other) { // evitar auto-asignación
        dni = other.dni;
        clientName = other.clientName;
        table = other.table;
        day = other.day;
        orders = other.orders;   //  usa el operador profundo de Orders
        total = other.total;
    }
    return *this;
}

