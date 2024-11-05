#include <iostream>
#include <string>

using namespace std;

// Estructura para representar un barco
struct Barco {
    string nombre;
    int prioridad;
    bool deseaAtraque;
    Barco* siguiente;
};

// Estructura para representar una cola de barcos en una terminal
struct Cola {
    Barco* frente;
    Barco* final;
};

// Estructura para representar una terminal de carga
struct Terminal {
    Cola colaAtraque;
    Cola colaPartida;
    Terminal* siguiente;
    string nombre;
};

// Estructura para representar un puerto
struct Puerto {
    Terminal* terminales;
    Puerto* siguiente;
};

// Prototipos de funciones
void inicializarCola(Cola &c);
void encolarBarco(Cola &c, string nombre, int prioridad, bool deseaAtraque);
void desencolarBarco(Cola &c);
void mostrarCola(const Cola &c, bool esAtraque);
void agregarTerminal(Terminal*& puerto, string nombre);
void reconocimiento(Puerto* puerto, string nombreTerminal, string nombreBarco, int prioridad, bool deseaAtraque);
void accion(Puerto* puerto, string nombreTerminal);
void mostrarEstadoPuerto(Puerto* puerto);
void liberarPuerto(Puerto*& puerto);
void menu();
void pedirDatosBarco(string& nombre, int& prioridad, bool& deseaAtraque);
void eliminarBarco(Terminal* terminal, string nombreBarco, bool deseaAtraque);
void eliminarTerminal(Puerto*& puerto, string nombreTerminal);

// Función principal
int main() {
    Puerto* puerto = NULL;
    int opcion;
    string nombreTerminal, nombreBarco;
    int prioridad;
    bool deseaAtraque;

    // Agregar puerto por defecto
    puerto = new Puerto;
    puerto->siguiente = NULL;
    puerto->terminales = NULL;

    agregarTerminal(puerto->terminales, "Terminal Norte");
    agregarTerminal(puerto->terminales, "Terminal Sur");

    do {
        menu();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cout << "---------------------------------\n";
        cin.ignore();  // Limpiar el buffer

        switch (opcion) {
            case 1:
                cout << "\n\tPuertos actuales:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                cout << "\nIngrese el nombre de la nueva terminal: ";
                getline(cin, nombreTerminal);
                agregarTerminal(puerto->terminales, nombreTerminal);
                break;

            case 2:
                cout << "\n\tPuertos disponibles:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                cout << "\nIngrese el nombre de la terminal donde desea encolar el barco: ";
                getline(cin, nombreTerminal);
                pedirDatosBarco(nombreBarco, prioridad, deseaAtraque);
                reconocimiento(puerto, nombreTerminal, nombreBarco, prioridad, deseaAtraque);
                break;

            case 3:
                cout << "\n\tPuertos disponibles:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                cout << "\nIngrese el nombre de la terminal donde desea procesar un barco: ";
                getline(cin, nombreTerminal);
                accion(puerto, nombreTerminal);
                break;

            case 4:
                cout << "\n\tPuertos actuales:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                break;

            case 5:
                cout << "\n\tIngrese el nombre de la terminal a eliminar: ";
                getline(cin, nombreTerminal);
                eliminarTerminal(puerto, nombreTerminal);
                break;

            case 6:
                cout << "\n\tIngrese el nombre del barco a eliminar: ";
                getline(cin, nombreBarco);
                cout << "Ingrese si el barco estaba en (1) Atraque o (2) Partida: ";
                cin >> prioridad;  // Usamos prioridad como un indicador de estado
                deseaAtraque = (prioridad == 1);
                cin.ignore();  // Limpiar el buffer
                for (Terminal* temp = puerto->terminales; temp != NULL; temp = temp->siguiente) {
                    eliminarBarco(temp, nombreBarco, deseaAtraque);
                }
                break;

            case 7:
                cout << "\tSaliendo del programa...\n";
                liberarPuerto(puerto);
                break;

            default:
                cout << "\tOpcion no válida. Intente de nuevo.\n";
        }

    } while (opcion != 7);

    return 0;
}

// Función para mostrar el menú de opciones
void menu() {
    cout << "\n-------------------------------------------------------\n";
    cout << "|        Gestion del Puerto Horizonte infinito        |\n";
    cout << "-------------------------------------------------------\n";
    cout << "| [1] => Agregar nueva terminal                       |\n";
    cout << "| [2] => Encolar barco en una terminal                |\n";
    cout << "| [3] => Procesar barco en una terminal               |\n";
    cout << "| [4] => Mostrar estado del puerto                    |\n";
    cout << "| [5] => Eliminar terminal                            |\n";
    cout << "| [6] => Eliminar barco                               |\n";
    cout << "| [7] => Salir                                        |\n";
    cout << "-------------------------------------------------------\n";
}

// Función para pedir los datos de un barco al usuario
void pedirDatosBarco(string& nombre, int& prioridad, bool& deseaAtraque) {
    cout << "Ingrese el nombre del barco: ";
    getline(cin, nombre);
    cout << "Ingrese la prioridad del barco (1 => Alta, 2 => Media, 3 => Baja): ";
    cin >> prioridad;
    int opcion;
    cout << "El barco desea (1) => Atraque o (2) => Partida: ";
    cin >> opcion;
    deseaAtraque = (opcion == 1);
    cin.ignore();  // Limpiar el buffer
}

// Inicializar una cola vacía
void inicializarCola(Cola &c) {
    c.frente = c.final = NULL;
}

// Encolar barco en la cola correspondiente (atraque o partida)
void encolarBarco(Cola &c, string nombre, int prioridad, bool deseaAtraque) {
    Barco* nuevo = new Barco{nombre, prioridad, deseaAtraque, NULL};
    
    if (c.frente == NULL) {  // Si la cola está vacía
        c.frente = c.final = nuevo;
    } else {
        // Insertar el barco en la posición adecuada según su prioridad
        Barco* actual = c.frente;
        Barco* anterior = NULL;
        while (actual != NULL && actual->prioridad <= prioridad) {
            anterior = actual;
            actual = actual->siguiente;
        }
        
        if (anterior == NULL) { // El nuevo barco tiene la prioridad más alta
            nuevo->siguiente = c.frente;
            c.frente = nuevo;
        } else { // Insertar el nuevo barco en la posición encontrada
            nuevo->siguiente = actual;
            anterior->siguiente = nuevo;
            if (actual == NULL) { // Si se insertó al final, actualizar el final de la cola
                c.final = nuevo;
            }
        }
    }
    cout << "\tEl barco " << nombre << " se ha encolado para " << (deseaAtraque ? "atraque" : "partida") << " con prioridad " << prioridad << ".\n";
}

// Desencolar barco de la cola correspondiente (atraque o partida)
void desencolarBarco(Cola &c) {
    if (c.frente != NULL) {
        Barco* aux = c.frente;
        c.frente = aux->siguiente;
        cout << "\tProcesando barco " << aux->nombre << " con prioridad " << aux->prioridad << ".\n";
        delete aux;
        if (c.frente == NULL) {
            c.final = NULL;
        }
    } else {
        cout << "\tNo hay barcos en espera.\n";
    }
}

// Mostrar la cola de barcos (atraque o partida)
void mostrarCola(const Cola &c, bool esAtraque) {
    Barco* actual = c.frente;
    cout << (esAtraque ? "Atraque" : "Partida") << ": ";
    while (actual != NULL) {
        cout << actual->nombre << " (Prioridad " << actual->prioridad << ") => ";
        actual = actual->siguiente;
    }
    cout << "NULL\n";
}

// Agregar una terminal al puerto
void agregarTerminal(Terminal*& puerto, string nombre) {
    Terminal* nueva = new Terminal{{NULL, NULL}, {NULL, NULL}, puerto, nombre};
    puerto = nueva;
    cout << "\n\tSe ha agregado la terminal: " << nombre << "\n";
}

// Encolar barco en la terminal especificada
void reconocimiento(Puerto* puerto, string nombreTerminal, string nombreBarco, int prioridad, bool deseaAtraque) {
    Terminal* terminalActual = puerto->terminales;
    while (terminalActual != NULL && terminalActual->nombre != nombreTerminal) {
        terminalActual = terminalActual->siguiente;
    }
    
    if (terminalActual != NULL) {
        if (deseaAtraque) {
            encolarBarco(terminalActual->colaAtraque, nombreBarco, prioridad, deseaAtraque);
        } else {
            encolarBarco(terminalActual->colaPartida, nombreBarco, prioridad, deseaAtraque);
        }
    } else {
        cout << "\tTerminal no encontrada.\n";
    }
}

// Procesar barco en la terminal especificada
void accion(Puerto* puerto, string nombreTerminal) {
    Terminal* terminalActual = puerto->terminales;
    while (terminalActual != NULL && terminalActual->nombre != nombreTerminal) {
        terminalActual = terminalActual->siguiente;
    }
    
    if (terminalActual != NULL) {
        cout << "\tColas de barcos en la terminal " << terminalActual->nombre << ":\n";
        mostrarCola(terminalActual->colaAtraque, true);
        mostrarCola(terminalActual->colaPartida, false);
        cout << "Desea procesar un barco de atraque o partida? (1) => Atraque (2) => Partida: ";
        int opcion;
        cin >> opcion;
        cin.ignore();  // Limpiar el buffer
        if (opcion == 1) {
            desencolarBarco(terminalActual->colaAtraque);
        } else if (opcion == 2) {
            desencolarBarco(terminalActual->colaPartida);
        } else {
            cout << "\tOpción no válida.\n";
        }
    } else {
        cout << "\tTerminal no encontrada.\n";
    }
}

// Mostrar el estado del puerto
void mostrarEstadoPuerto(Puerto* puerto) {
    Terminal* actual = puerto->terminales;
    if (actual == NULL) {
        cout << "\tNo hay terminales en el puerto.\n";
        return;
    }

    while (actual != NULL) {
        cout << "\t- " << actual->nombre << "\n";
        mostrarCola(actual->colaAtraque, true);
        mostrarCola(actual->colaPartida, false);
        actual = actual->siguiente;
    }
}

// Liberar la memoria del puerto
void liberarPuerto(Puerto*& puerto) {
    Terminal* actual = puerto->terminales;
    while (actual != NULL) {
        Terminal* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    delete puerto;
    puerto = NULL;
}

// Eliminar barco de la cola correspondiente
void eliminarBarco(Terminal* terminal, string nombreBarco, bool deseaAtraque) {
    // if (deseaAtraque) {
    //     resultado = terminal->colaAtraque;
    // } else {
    //     resultado = terminal->colaPartida;
    // }
    Cola& cola = deseaAtraque ? terminal->colaAtraque : terminal->colaPartida;
    Barco* actual = cola.frente;
    Barco* anterior = NULL;

    while (actual != NULL && actual->nombre != nombreBarco) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual != NULL) {
        if (anterior == NULL) {  // Es el primer barco
            cola.frente = actual->siguiente;
            if (cola.frente == NULL) {
                cola.final = NULL;
            }
        } else {
            anterior->siguiente = actual->siguiente;
            if (actual->siguiente == NULL) {
                cola.final = anterior;  // Si se eliminó el último
            }
        }
        delete actual;
        cout << "\tEl barco " << nombreBarco << " ha sido eliminado de la cola.\n";
    } else {
        cout << "\tEl barco no se encuentra en la cola.\n";
    }
}

// Eliminar una terminal del puerto
void eliminarTerminal(Puerto*& puerto, string nombreTerminal) {
    Terminal* actual = puerto->terminales;
    Terminal* anterior = NULL;

    while (actual != NULL && actual->nombre != nombreTerminal) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual != NULL) {
        // Si es la primera terminal en la lista
        if (anterior == NULL) {
            puerto->terminales = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }

        // Liberar memoria de la cola de atraque
        while (actual->colaAtraque.frente != NULL) {
            desencolarBarco(actual->colaAtraque);
        }

        // Liberar memoria de la cola de partida
        while (actual->colaPartida.frente != NULL) {
            desencolarBarco(actual->colaPartida);
        }

        delete actual;
        cout << "\tLa terminal " << nombreTerminal << " ha sido eliminada.\n";
    } else {
        cout << "\tTerminal no encontrada.\n";
    }
}
