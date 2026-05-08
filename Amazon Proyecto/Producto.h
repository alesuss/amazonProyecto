#pragma once

#include <string>
#include "Categoria.h"
#include "Resena.h"
#include "ListaDoble.hpp"
using namespace std;

class Producto {
private:
    string sku;
    string nombre;
    float precio;
    int stock;
    Categoria* categoria;
    ListaDoble<Resena*>* resenas;
public:
    Producto(string s, string n, float p, int st);
    void actualizarStock(int cant);
    string getSku();
    string getNombre();
    float getPrecio();
    int getStock();
    void setCategoria(Categoria* c);
    Categoria* getCategoria();
    void agregarResena(Resena* r);
    ListaDoble<Resena*>* getResenas();
    void mostrar();
};