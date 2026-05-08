#pragma once

#include "ListaDoble.hpp"
#include "Producto.h"

class CarritoCompras {
private:
    ListaDoble<Producto*>* items;
    float totalTemporal;
public:
    CarritoCompras();
    ~CarritoCompras();
    void agregarItem(Producto* p);
    float calcularTotal();
    ListaDoble<Producto*>* getItems();
    void vaciar();
};