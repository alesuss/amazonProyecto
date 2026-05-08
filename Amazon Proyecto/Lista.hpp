#pragma once

#include <functional>

using namespace std;

typedef unsigned int uint;

// Nodo anidado dentro de la clase para evitar colision al incluir
// Lista, ListaDoble y ListaCircular en el mismo proyecto.
template <typename T>
class Lista {
public:
    struct Nodo {
        T     elem;
        Nodo* sig;
        Nodo(T elem) : elem(elem), sig(nullptr) {}
        Nodo(T elem, Nodo* sig) : elem(elem), sig(sig) {}
    };

private:
    Nodo* ini;
    uint  lon;
    

public:
    Lista() : ini(nullptr), lon(0) {}
    ~Lista() {
        Nodo* aux;
        while (ini != nullptr) {
            aux = ini;
            ini = ini->sig;
            delete aux;
        }
        lon = 0;
    }

    uint  tam()       const { return lon; }
    bool  estaVacia() const { return lon == 0; }
    Nodo* inicio()    const { return ini; }

    void insertarInicio(T elem) {
        ini = new Nodo(elem, ini);
        lon++;
    }

    void insertarFinal(T elem) {
        if (ini == nullptr) { insertarInicio(elem); return; }
        Nodo* aux = ini;
        while (aux->sig != nullptr) aux = aux->sig;
        aux->sig = new Nodo(elem);
        lon++;
    }

    void insertar(uint pos, T elem) {
        if (pos > lon) return;
        if (pos == 0) { insertarInicio(elem); return; }
        Nodo* aux = ini;
        for (uint i = 1; i < pos; i++) aux = aux->sig;
        aux->sig = new Nodo(elem, aux->sig);
        lon++;
    }

    void modificar(uint pos, T elem) {
        if (pos >= lon) return;
        Nodo* aux = ini;
        for (uint i = 0; i < pos; i++) aux = aux->sig;
        aux->elem = elem;
    }

    void eliminarInicio() {
        if (lon == 0) return;
        Nodo* aux = ini;
        ini = ini->sig;
        delete aux;
        lon--;
    }

    void eliminar(uint pos) {
        if (pos >= lon) return;
        if (pos == 0) { eliminarInicio(); return; }
        Nodo* aux = ini;
        for (uint i = 1; i < pos; i++) aux = aux->sig;
        Nodo* borrar = aux->sig;
        aux->sig = borrar->sig;
        delete borrar;
        lon--;
    }

    void eliminarFinal() {
        if (lon == 0) return;
        if (lon == 1) { eliminarInicio(); return; }
        Nodo* aux = ini;
        while (aux->sig->sig != nullptr) aux = aux->sig;
        delete aux->sig;
        aux->sig = nullptr;
        lon--;
    }

    T obtener(uint pos) {
        Nodo* aux = ini;
        for (uint i = 0; i < pos; i++) aux = aux->sig;
        return aux->elem;
    }

    Nodo* buscar(T elem) {
        Nodo* aux = ini;
        while (aux != nullptr) {
            if (aux->elem == elem) return aux;
            aux = aux->sig;
        }
        return nullptr;
    }

    // Metodo nuevo: cuenta cuantos elementos cumplen un criterio (lambda).
    int contarSi(function<bool(T)> pred) {
        int total = 0;
        Nodo* aux = ini;
        while (aux != nullptr) {
            if (pred(aux->elem)) total++;
            aux = aux->sig;
        }
        return total;
    }
};
