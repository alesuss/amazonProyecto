#pragma once

#include <string>
using namespace std;

class Direccion {
private:
    string calle;
    string ciudad;
    string codigoPostal;
public:
    Direccion(string c, string ci, string cp);
    string getUbicacionFormateada();
    string getCalle();
    string getCiudad();
    string getCp();
};