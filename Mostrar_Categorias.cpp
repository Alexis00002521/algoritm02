#include <iostream>
#include <fstream>
#include <string>

using namespace std ;
const int MAX_PRODUCTOS = 100; // Número máximo de productos

// estructura del producto 
struct Producto {
    string nombre;
    string categoria;
    int stock;
};

// Función para cargar productos desde el archivo en la misma carpeta
int cargarProductosDesdeArchivo(Producto productos[], int stock[], int &numProductos) {
    ifstream archivo("productos.txt"); // Archivo en la misma carpeta

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo productos.txt en la carpeta del proyecto." << endl;
        return numProductos;
    }

    numProductos = 0;
    while (archivo >> productos[numProductos].nombre >> productos[numProductos].categoria >> productos[numProductos].stock) {
        stock[numProductos] = productos[numProductos].stock;
        numProductos++;
    
    }
    archivo.close();
    return numProductos;
}
// Función para mostrar categorías únicas
void mostrarCategorias(Producto productos[], int numProductos) {
    cout << "\nCategorías disponibles:\n";
    for (int i = 0; i < numProductos; i++) {
        bool yaMostrada = false;
        for (int j = 0; j < i; j++) {
            if (productos[i].categoria == productos[j].categoria) {
                yaMostrada = true;
                break;
            }
        }
        if (!yaMostrada) {
            cout << "- " << productos[i].categoria << endl;
        }
    }
}

int main() {
    Producto productos[MAX_PRODUCTOS];
    int stock[MAX_PRODUCTOS];
    int numProductos = 0;

    // Cargar productos desde archivo al inicio
    cargarProductosDesdeArchivo(productos, stock, numProductos);

    //SegmentTree segTree(stock, numProductos);

    // Mostrar categorías únicas al inicio
    mostrarCategorias(productos, numProductos);

    return 0;
}