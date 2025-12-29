#include "orders.hpp"

Orders::Orders() {
    top = nullptr;
}

bool Orders::isEmpty() {
    return top == nullptr;
}

bool Orders::isFull() {
    OrderPtr p = new OrderNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Orders::push(int dishCode, const string& notes) {
    const MenuItem* item = findMenuItem(dishCode);
    if (item == nullptr) return false;

    Order newOrder(dishCode, item->price, notes);
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

void Orders::setInfo(OrderNode* p, Order value) {
    p->info = value;
}

double Orders::totalPrice() {
    double total = 0.0;
    OrderPtr current = top;
    while (current != nullptr) {
        total += current->info.price;
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

