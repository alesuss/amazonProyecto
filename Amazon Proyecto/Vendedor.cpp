#include "pch.h"
#include "Vendedor.h"
#include <iostream>

Vendedor::Vendedor(int id, string n, string e, string p) : Usuario(id, n, e, p), ratingVendedor(5.0) {
    productosOfrecidos = new ListaDoble<Producto*>();
}

Vendedor::~Vendedor() {
    delete productosOfrecidos;
}

string Vendedor::rol() { return "Vendedor"; }

void Vendedor::agregarProducto(Producto* p) { productosOfrecidos->insertarFinal(p); }
void Vendedor::gestionarStock() { cout << "Gestionando inventario...\n"; }