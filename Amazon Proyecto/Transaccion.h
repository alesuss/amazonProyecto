#pragma once

#include <string>
using namespace std;

class Transaccion {
private:
    string idTransaccion;
    string metodoPago;
public:
    Transaccion(string id, string m);
    bool procesarPago();
    string getId();
    string getMetodo();
};