#ifndef COLA_CPP
#define COLA_CPP

#include "cola.hpp"

template <typename T>
Cola<T>::Cola() {
    frente = NULL;
    final = NULL;
}

template <typename T>
bool Cola<T>::colaVacia() {
    return (frente == NULL);
}

template <typename T>
void Cola<T>::insertar(T valor) {
    NodoCola<T>* nuevo = new NodoCola<T>();
    nuevo->dato = valor;
    nuevo->next = NULL;

    if (colaVacia()) {
        frente = nuevo;
        final = nuevo;
    } else {
        final->next = nuevo;
        final = nuevo;
    }
}

template <typename T>
void Cola<T>::quitar(T &valor) {
    if (colaVacia()) {
        cout << "Cola vacia." << endl;
        return;
    }

    NodoCola<T>* p = frente;
    valor = p->dato;
    
    frente = frente->next;
    delete p;
    
    if (frente == NULL) {
        final = NULL;
    }
}

template <typename T>
void Cola<T>::borrarCola() {
    NodoCola<T>* p;
    while (!colaVacia()) {
        p = frente;
        frente = frente->next;
        delete p;
    }
    final = NULL;
}

template <typename T>
void Cola<T>::mostrarCola() {
    if (colaVacia()) {
        cout << "La cola de espera esta vacia." << endl;
        return;
    }

    NodoCola<T>* aux = frente;
    cout << "\n--- PERSONAS EN ESPERA ---" << endl;
    int i = 1;
    while (aux != NULL) {
        // Accedemos a los campos del struct DatosCola (dni, name)
        cout << i << ". [" << aux->dato.dni << "] " << aux->dato.name << endl;
        aux = aux->next;
        i++;
    }
    cout << "--------------------------" << endl;
}

#endif
