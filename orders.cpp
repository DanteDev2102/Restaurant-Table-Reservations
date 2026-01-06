#include "orders.hpp"

// =======================
// Implementación de Order
// =======================

Order::Order() : dishCode(0), price(0.0), notes("") {}

Order::Order(int code, double price, const string& notes)
    : dishCode(code), price(price), notes(notes) {}

int Order::getDishCode() const { return dishCode; }
double Order::getPrice() const { return price; }
string Order::getNotes() const { return notes; }

void Order::setDishCode(int code) { dishCode = code; }
void Order::setPrice(double price) { this->price = price; }
void Order::setNotes(const string& notes) { this->notes = notes; }

// =======================
// Implementación de Orders
// =======================

Orders::Orders() {
    top = nullptr;
}

Orders::~Orders() {
    OrderPtr current = top;
    while (current != nullptr) {
        OrderPtr temp = current;
        current = current->next;
        delete temp;
    }
}

bool Orders::isEmpty() {
    return top == nullptr;
}

bool Orders::isFull() {
    OrderPtr p = new(nothrow) OrderNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Orders::push(int dishCode, const string& notes) {
    const MenuItem* item = findMenuItem(dishCode);
    if (item == nullptr) return false;

    Order newOrder(dishCode, item->getPrice(), notes);
    OrderPtr newNode = new OrderNode;
    newNode->info = newOrder;
    newNode->next = top;
    top = newNode;
    return true;
}

bool Orders::pop(Order& value) {
    if (!isEmpty()) {
        OrderPtr oldTop = top;
        value = oldTop->info;
        top = top->next;
        delete oldTop;
        return true;
    }
    return false;
}

Orders::OrderPtr Orders::getTop() {
    return top;
}

void Orders::setTop(OrderPtr p) {
    top = p;
}

Order Orders::getInfo(OrderNode* p) {
    return p->info;
}

void Orders::setInfo(OrderNode* p, const Order& value) {
    if (p != nullptr)
        p->info = value;
}

double Orders::totalPrice() {
    double total = 0.0;
    OrderPtr current = top;
    while (current != nullptr) {
        total += current->info.getPrice();
        current = current->next;
    }
    return total;
}

int Orders::size() {
    int count = 0;
    OrderPtr current = top;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}
