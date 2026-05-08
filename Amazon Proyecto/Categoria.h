#pragma once

#include <string>
#include "ListaDoble.hpp"
using namespace std;

class Categoria {
private:
    string nombreCat;
    ListaDoble<Categoria*>* subCategorias;
public:
    Categoria(string n);
    ~Categoria();
    void anadirSubcategoria(Categoria* c);
    string getNombre();
};