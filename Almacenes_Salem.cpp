#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_PRODUCTOS = 100; // Número máximo de productos

// Estructura para los productos
struct Producto {
    string nombre;
    string categoria;
    int stock;
};

// Árbol segmentado para manejar consultas rápidas de stock por rango
class SegmentTree {
    int tree[4 * MAX_PRODUCTOS];
    int size;

    void build(int stock[], int node, int start, int end) {
        if (start == end) {
            tree[node] = stock[start];
        } else {
            int mid = (start + end) / 2;
            build(stock, 2 * node + 1, start, mid);
            build(stock, 2 * node + 2, mid + 1, end);
            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }
    }


    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid) {
                update(2 * node + 1, start, mid, idx, val);
            } else {
                update(2 * node + 2, mid + 1, end, idx, val);
            }
            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }
    }

    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        int left = query(2 * node + 1, start, mid, l, r);
        int right = query(2 * node + 2, mid + 1, end, l, r);
        return left + right;
    }

public:
    SegmentTree(int stock[], int n) {
        size = n;
        build(stock, 0, 0, n - 1);
    }

    void update(int idx, int val) {
        update(0, 0, size - 1, idx, val);
    }

    int query(int l, int r) {
        return query(0, 0, size - 1, l, r);
    }
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
        if (numProductos >= MAX_PRODUCTOS) {
            cout << "Se alcanzó el límite máximo de productos.\n";
            break;
        }
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

    SegmentTree segTree(stock, numProductos);

    // Mostrar categorías únicas al inicio
    mostrarCategorias(productos, numProductos);

    int opcion;
    do {
        cout << "\n--- Menú ---\n";
        cout << "1. Agregar producto\n";
        cout << "2. Actualizar stock\n";
        cout << "3. Consultar stock por producto\n";
        cout << "4. Consultar stock por categoría\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (opcion == 1) {
            if (numProductos >= MAX_PRODUCTOS) {
                cout << "Error: límite de productos alcanzado.\n";
                continue;
            }
            Producto nuevoProducto;
            cout << "Ingrese el nombre del producto: ";
            cin >> nuevoProducto.nombre;
            cout << "Ingrese la categoría del producto: ";
            cin >> nuevoProducto.categoria;
            cout << "Ingrese la cantidad de stock: ";
            cin >> nuevoProducto.stock;

            productos[numProductos] = nuevoProducto;
            stock[numProductos] = nuevoProducto.stock;
            numProductos++;
            segTree = SegmentTree(stock, numProductos);  // Reconstruir el árbol segmentado

        } else if (opcion == 2) {
            string nombreProducto;
            int nuevoStock;
            cout << "Ingrese el nombre del producto a actualizar: ";
            cin >> nombreProducto;
            cout << "Ingrese el nuevo stock: ";
            cin >> nuevoStock;

            bool encontrado = false;
            for (int i = 0; i < numProductos; i++) {
                if (productos[i].nombre == nombreProducto) {
                    productos[i].stock = nuevoStock;
                    segTree.update(i, nuevoStock);
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) cout << "Producto no encontrado.\n";

        } else if (opcion == 3) {
            string nombreProducto;
            cout << "Ingrese el nombre del producto a consultar: ";
            cin >> nombreProducto;

            bool encontrado = false;
            for (int i = 0; i < numProductos; i++) {
                if (productos[i].nombre == nombreProducto) {
                    cout << "Stock de " << nombreProducto << ": " << segTree.query(i, i) << "\n";
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) cout << "Producto no encontrado.\n";

        } else if (opcion == 4) {
            string categoria;
            cout << "Ingrese la categoría a consultar: ";
            cin >> categoria;

            int totalStock = 0;
            for (int i = 0; i < numProductos; i++) {
                if (productos[i].categoria == categoria) {
                    totalStock += segTree.query(i, i);
                }
            }
            cout << "Stock total de la categoría '" << categoria << "': " << totalStock << "\n";

        } else if (opcion == 5) {
            cout << "Saliendo...\n";
        } else {
            cout << "Opción inválida.\n";
        }
    } while (opcion != 5);

    return 0;
}

/*
Almacenes Salem

gestion manualmente el inventario 
-mas de 50.000
-manejo de electronia hasta productos 
de consumo basico.

Se necesita un programa que permita mantener y consultar
eficientemente las cantidades de productos disponibles por categoría, garantizando que
los tiempos de consulta y actualización sean lo más rápido posible, incluso a medida que
crece el catálogo.







*/