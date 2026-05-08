#include "pch.h"
#include "Categoria.h"

Categoria::Categoria(string n) : nombreCat(n) {
    subCategorias = new ListaDoble<Categoria*>();
}

Categoria::~Categoria() {
    delete subCategorias;
}

void Categoria::anadirSubcategoria(Categoria* c) {
    subCategorias->insertarFinal(c);
}

string Categoria::getNombre() { return nombreCat; }