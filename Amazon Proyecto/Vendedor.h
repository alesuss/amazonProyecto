#pragma once

#include "Usuario.h"
#include "Producto.h"
#include "ListaDoble.hpp"

class Vendedor : public Usuario {
private:
    ListaDoble<Producto*>* productosOfrecidos;
    float ratingVendedor;
public:
    Vendedor(int id, string n, string e, string p);
    ~Vendedor();
    string rol() override;
    void agregarProducto(Producto* p);
    void gestionarStock();
};