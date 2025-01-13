#include <stdio.h>
#include "funciones.h"

int main() {
    int opcion;

    do {
        opcion = menu();

        switch (opcion) {
            case 1:
                createFactura();
                break;
            case 2:
                actualizarFactura();
                break;
            case 3:
                readFacturas();
                break;
            case 4:
                eliminarFactura();
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida\n");
        }
    } while (opcion != 5);

    return 0;
}
