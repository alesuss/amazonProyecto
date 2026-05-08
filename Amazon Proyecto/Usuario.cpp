#include "pch.h"
#include "Usuario.h"
#include <iostream>

Usuario::Usuario(int i, string n, string e, string p) : id(i), nombre(n), email(e), password(p) {}

Usuario::~Usuario() {}

void Usuario::mostrarPerfil() {
    cout << "[" << rol() << "] ID: " << id << " | " << nombre << " | " << email << "\n";
}

int    Usuario::getId() { return id; }
string Usuario::getNombre() { return nombre; }
string Usuario::getEmail() { return email; }
string Usuario::getPassword() { return password; }