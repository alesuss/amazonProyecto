#pragma once

typedef unsigned int uint;

template <typename T>
class ListaDoble {
public:
    struct Nodo {
        T     elem;
        Nodo* sig;
        Nodo* ant;
        Nodo(T elem) : elem(elem), sig(nullptr), ant(nullptr) {}
        Nodo(T elem, Nodo* ant, Nodo* sig) : elem(elem), sig(sig), ant(ant) {}
    };

private:
    Nodo* ini;
    Nodo* fin;
    uint  lon;

public:
    ListaDoble() : ini(nullptr), fin(nullptr), lon(0) {}
    ~ListaDoble() {
        Nodo* aux;
        while (ini != nullptr) {
            aux = ini;
            ini = ini->sig;
            delete aux;
        }
        fin = nullptr;
        lon = 0;
    }

    uint  tam()       const { return lon; }
    bool  estaVacia() const { return lon == 0; }
    Nodo* inicio()    const { return ini; }
    Nodo* ultimo()    const { return fin; }

    void insertarInicio(T elem) {
        Nodo* nuevo = new Nodo(elem, nullptr, ini);
        if (ini != nullptr) ini->ant = nuevo;
        else fin = nuevo;
        ini = nuevo;
        lon++;
    }

    void insertarFinal(T elem) {
        if (fin == nullptr) { insertarInicio(elem); return; }
        Nodo* nuevo = new Nodo(elem, fin, nullptr);
        fin->sig = nuevo;
        fin = nuevo;
        lon++;
    }

    void insertar(uint pos, T elem) {
        if (pos > lon) return;
        if (pos == 0) { insertarInicio(elem); return; }
        if (pos == lon) { insertarFinal(elem);  return; }
        Nodo* aux;
        if (pos <= lon / 2) {
            aux = ini;
            for (uint i = 0; i < pos; i++) aux = aux->sig;
        }
        else {
            aux = fin;
            for (uint i = lon - 1; i > pos; i--) aux = aux->ant;
        }
        Nodo* nuevo = new Nodo(elem, aux->ant, aux);
        aux->ant->sig = nuevo;
        aux->ant = nuevo;
        lon++;
    }

    void modificar(uint pos, T elem) {
        if (pos >= lon) return;
        Nodo* aux;
        if (pos <= lon / 2) {
            aux = ini;
            for (uint i = 0; i < pos; i++) aux = aux->sig;
        }
        else {
            aux = fin;
            for (uint i = lon - 1; i > pos; i--) aux = aux->ant;
        }
        aux->elem = elem;
    }

    void eliminarInicio() {
        if (lon == 0) return;
        Nodo* aux = ini;
        ini = ini->sig;
        if (ini != nullptr) ini->ant = nullptr;
        else fin = nullptr;
        delete aux;
        lon--;
    }

    void eliminarFinal() {
        if (lon == 0) return;
        if (lon == 1) { eliminarInicio(); return; }
        Nodo* aux = fin;
        fin = fin->ant;
        fin->sig = nullptr;
        delete aux;
        lon--;
    }

    void eliminar(uint pos) {
        if (pos >= lon) return;
        if (pos == 0) { eliminarInicio(); return; }
        if (pos == lon - 1) { eliminarFinal(); return; }
        Nodo* borrar;
        if (pos <= lon / 2) {
            borrar = ini;
            for (uint i = 0; i < pos; i++) borrar = borrar->sig;
        }
        else {
            borrar = fin;
            for (uint i = lon - 1; i > pos; i--) borrar = borrar->ant;
        }
        borrar->ant->sig = borrar->sig;
        borrar->sig->ant = borrar->ant;
        delete borrar;
        lon--;
    }

    T obtener(uint pos) {
        Nodo* aux;
        if (pos <= lon / 2) {
            aux = ini;
            for (uint i = 0; i < pos; i++) aux = aux->sig;
        }
        else {
            aux = fin;
            for (uint i = lon - 1; i > pos; i--) aux = aux->ant;
        }
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
};
