#pragma once

#include <string>
using namespace std;

class Usuario {
protected:
    int id;
    string nombre;
    string email;
    string password;
public:
    Usuario(int id, string n, string e, string p);
    virtual ~Usuario();
    virtual string rol() = 0;
    virtual void mostrarPerfil();
    int getId();
    string getNombre();
    string getEmail();
    string getPassword();
};