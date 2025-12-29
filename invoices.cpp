#include "invoices.hpp"

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
    PtrInvoice p = new InvoiceNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Invoices::insertAtBeginning(Invoice value) {
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

bool Invoices::insertAfter(PtrInvoice p, Invoice value) {
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

void Invoices::setInfo(PtrInvoice p, Invoice value) {
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
        if (current->info.dni == dni) return current;
        current = current->next;
    }
    return nullptr;
}

