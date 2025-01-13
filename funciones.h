struct Producto {
    char nombre[50];
    float precio;
    int cantidad;
};

struct Factura {
    char nombre[20];
    int cedula;
    int nProductos;
    struct Producto productos[5];
    float total;
    int activo;
};

int menu();
void saveFactura(struct Factura *factura);
void updateFactura(struct Factura *factura, int posicion);
int findByCedula(int cedula);
void leerCadena(char *cadena, int longitud);
int validarMayorCero(float valor, const char *mensaje);
void createFactura();
void actualizarFactura();
void readFacturas();
void eliminarFactura();
