#pragma once

#include <string>
using namespace std;

class Envio {
private:
    string trackingId;
    string estadoEnvio;
    string fechaEstimada;
public:
    Envio(string t, string f);
    void actualizarEstado(string e);
};