#pragma once

#include <string>
using namespace std;

class Promocion {
private:
    string codigo;
    float descuento;
public:
    Promocion(string c, float d);
    bool esValida();
    string getCodigo();
    float getDescuento();
};