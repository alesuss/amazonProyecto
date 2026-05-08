#pragma once

#include "ListaDoble.hpp"
#include "Producto.h"
#include <functional>
using namespace std;

class Inventario {
private:
    ListaDoble<Producto*>* listaProductos;
    double calcularValorTotalRecursivo(int pos, int total);
public:
    Inventario();
    ~Inventario();
    void agregar(Producto* p);
    ListaDoble<Producto*>* getLista();
    Producto* buscarProducto(function<bool(Producto*)> lambda);
    double obtenerValorInventario();
};