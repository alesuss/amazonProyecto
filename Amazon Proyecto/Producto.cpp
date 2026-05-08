#include "pch.h"
#include "Producto.h"
#include <iostream>

Producto::Producto(string s, string n, float p, int st) : sku(s), nombre(n), precio(p), stock(st) {
    categoria = nullptr;
    resenas = new ListaDoble<Resena*>();
}

void Producto::actualizarStock(int cant) { stock += cant; }
string Producto::getSku() { return sku; }
string Producto::getNombre() { return nombre; }
float Producto::getPrecio() { return precio; }
int Producto::getStock() { return stock; }

void Producto::setCategoria(Categoria* c) { categoria = c; }
Categoria* Producto::getCategoria() { return categoria; }
void Producto::agregarResena(Resena* r) { resenas->insertarFinal(r); }
ListaDoble<Resena*>* Producto::getResenas() { return resenas; }

void Producto::mostrar() {
    string cat = (categoria != nullptr) ? categoria->getNombre() : "Sin Categoria";
    cout << "SKU: " << sku << " | " << nombre << " | S/. " << precio << " | Stock: " << stock << " | Cat: " << cat << " | Resenas: " << resenas->tam() << "\n";
}