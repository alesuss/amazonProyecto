#pragma once

#include <string>
using namespace std;

class Resena {
private:
    int estrellas;
    string comentario;
public:
    Resena(int e, string c);
    bool validarResena();
    int getEstrellas();
    string getComentario();
};