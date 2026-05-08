#include "pch.h"
#include "Envio.h"

Envio::Envio(string t, string f) : trackingId(t), estadoEnvio("Procesando"), fechaEstimada(f) {}

void Envio::actualizarEstado(string e) {
    estadoEnvio = e;
}