#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu() {
    int opcion;
    printf("Menu\n");
    printf("1. Crear factura\n");
    printf("2. Actualizar factura\n");
    printf("3. Leer facturas\n");
    printf("4. Eliminar factura\n");
    printf("5. Salir\n");
    printf("Ingrese una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void saveFactura(struct Factura *factura) {
    FILE *archivo;
    archivo = fopen("factura.dat", "ab+");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    } else {
        fwrite(factura, sizeof(struct Factura), 1, archivo);
        printf("Factura guardada con exito\n");
    }

    fclose(archivo);
}

void updateFactura(struct Factura *factura, int posicion) {
    FILE *archivo;
    archivo = fopen("factura.dat", "rb+");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    } else {
        fseek(archivo, posicion, 0);
        fwrite(factura, sizeof(struct Factura), 1, archivo);
        printf("Factura actualizada con exito\n");
    }

    fclose(archivo);
}

int findByCedula(int cedula) {
    int posicion = 0, f = 0;
    FILE *archivo;
    struct Factura factura;
    archivo = fopen("factura.dat", "rb");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return -1;
    } else {
        while (fread(&factura, sizeof(struct Factura), 1, archivo)) {
            if (factura.cedula == cedula) {
                f = 1;
                printf("Factura encontrada\n");
                printf("Nombre\t\tCedula\n");
                printf("%s\t\t%d\n", factura.nombre, factura.cedula);
                break;
            }
            posicion = ftell(archivo);
        }

        if (f == 0) {
            printf("No existe la factura\n");
            posicion = -1;
        }
        fclose(archivo);
        return posicion;
    }
}

void leerCadena(char *cadena, int longitud) {
    fflush(stdin);
    fgets(cadena, longitud, stdin);
    int len = strlen(cadena) - 1;
    cadena[len] = '\0';
}

int validarMayorCero(float valor, const char *mensaje) {
    if (valor <= 0) {
        printf("%s debe ser mayor a 0.\n", mensaje);
        return 0;
    }
    return 1;
}

void actualizarFactura() {
    int cedula;
    printf("Ingrese la cedula a buscar: ");
    scanf("%d", &cedula);

    int posicion = findByCedula(cedula);
    struct Factura factura;
    if (posicion != -1) {
        printf("Ingrese el Nombre del cliente: ");
        leerCadena(factura.nombre, 20);
        printf("Ingrese la Cedula del cliente: ");
        scanf("%d", &factura.cedula);

        do {
            printf("Ingrese el numero de productos: ");
            scanf("%d", &factura.nProductos);
        } while (!validarMayorCero(factura.nProductos, "El numero de productos"));

        factura.total = 0;
        for (int i = 0; i < factura.nProductos; i++) {
            printf("Ingrese el Nombre del producto %d: ", i + 1);
            leerCadena(factura.productos[i].nombre, 50);

            do {
                printf("Ingrese el Precio del producto %d: ", i + 1);
                scanf("%f", &factura.productos[i].precio);
            } while (!validarMayorCero(factura.productos[i].precio, "El precio"));

            do {
                printf("Ingrese la Cantidad del producto %d: ", i + 1);
                scanf("%d", &factura.productos[i].cantidad);
            } while (!validarMayorCero(factura.productos[i].cantidad, "La cantidad"));

            factura.total += factura.productos[i].precio * factura.productos[i].cantidad;
        }

        updateFactura(&factura, posicion);
    }
}

void createFactura() {
    struct Factura factura;
    printf("Ingrese el Nombre del cliente: ");
    leerCadena(factura.nombre, 20);

    printf("Ingrese la Cedula del cliente: ");
    scanf("%d", &factura.cedula);

    do {
        printf("Ingrese el numero de productos: ");
        scanf("%d", &factura.nProductos);
    } while (!validarMayorCero(factura.nProductos, "El numero de productos"));

    factura.total = 0;
    factura.activo = 1;

    for (int i = 0; i < factura.nProductos; i++) {
        printf("Ingrese el Nombre del producto %d: ", i + 1);
        leerCadena(factura.productos[i].nombre, 50);

        do {
            printf("Ingrese el Precio del producto %d: ", i + 1);
            scanf("%f", &factura.productos[i].precio);
        } while (!validarMayorCero(factura.productos[i].precio, "El precio"));

        do {
            printf("Ingrese la Cantidad del producto %d: ", i + 1);
            scanf("%d", &factura.productos[i].cantidad);
        } while (!validarMayorCero(factura.productos[i].cantidad, "La cantidad"));

        factura.total += factura.productos[i].precio * factura.productos[i].cantidad;
    }

    saveFactura(&factura);
}

void readFacturas() {
    FILE *archivo;
    archivo = fopen("factura.dat", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    struct Factura factura;
    int hayFacturasActivas = 0;

    while (fread(&factura, sizeof(struct Factura), 1, archivo) == 1) {
        if (factura.activo) {
            hayFacturasActivas = 1;
            printf("Nombre del cliente: %s\n", factura.nombre);
            printf("Cedula del cliente: %d\n", factura.cedula);
            printf("#\tNombre\t\tPrecio\t\tCantidad\n");
            for (int i = 0; i < factura.nProductos; i++) {
                printf("%d\t%s\t\t%.2f\t\t%d\n", i + 1, 
                       factura.productos[i].nombre, 
                       factura.productos[i].precio, 
                       factura.productos[i].cantidad);
            }
            printf("Total: %.2f\n", factura.total);
            printf("\n");
        }
    }

    if (!hayFacturasActivas) {
        printf("No hay facturas activas para mostrar.\n");
    }

    fclose(archivo);
}

void eliminarFactura() {
    int cedula;
    printf("Ingrese la cedula para eliminar: ");
    scanf("%d", &cedula);

    FILE *archivo = fopen("factura.dat", "rb+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    struct Factura factura;
    int encontrado = 0;
    int posicion = 0;

    while (fread(&factura, sizeof(struct Factura), 1, archivo)) {
        if (factura.cedula == cedula && factura.activo) {
            encontrado = 1;
            factura.activo = 0;

            fseek(archivo, posicion, 0);
            fwrite(&factura, sizeof(struct Factura), 1, archivo);
            printf("Factura eliminada.\n");
            break;
        }
        posicion = ftell(archivo);
    }

    if (!encontrado) {
        printf("Factura no encontrada.\n");
    }

    fclose(archivo);
}
