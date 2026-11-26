#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <chrono>

using namespace std;

struct ItemTexto {
    string subtitulo;
    string contenido;
};

struct CarpetaPrompter {
    string nombreCarpeta;
    vector<ItemTexto> items;
};

void limpiarPantalla() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        cout << "\033[2J\033[1;1H";
    #endif
}

void ejecutarTeleprompter(const string& titulo, const vector<string>& lineas, int delayMs) {
    limpiarPantalla();
    cout << "=======================================================================" << endl;
    cout << "           TELEPROMPTER TERMINAL :: MODO REPRODUCCION                 " << endl;
    cout << "=======================================================================" << endl;
    cout << " Modulo: [" << titulo << "]" << endl;
    cout << " Retardo por línea: " << delayMs << " ms" << endl;
    cout << "-----------------------------------------------------------------------" << endl << endl;

    for (size_t i = 0; i < lineas.size(); ++i) {
        cout << lineas[i] << endl;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }

    cout << "\n-----------------------------------------------------------------------" << endl;
    cout << "[FIN DE LA LECTURA] Presiona ENTER para regresar al menú...";
    cin.ignore(
:root, '\n');
    cin.get();
}

void menuCarpeta(CarpetaPrompter& carpeta, int& delayMs) {
    while (true) {
        limpiarPantalla();
        cout << "=======================================================================" << endl;
        cout << " CARPETA: " << carpeta.nombreCarpeta << endl;
        cout << "=======================================================================" << endl;
        cout << " Subtítulos disponibles:" << endl << endl;

        for (size_t i = 0; i < carpeta.items.size(); ++i) {
            cout << "  [" << (i + 1) << "] " << carpeta.items[i].subtitulo << endl;
        }
        cout << "  [" << (carpeta.items.size() + 1) << "] Reproducir TODA la carpeta" << endl;
        cout << "  [0] Volver al menú principal" << endl;
        cout << "-----------------------------------------------------------------------" << endl;
        cout << " Selecciona una opción: ";

        int opcion;
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (opcion == 0) {
            return;
        }

        int idx = opcion - 1;
        if (idx >= 0 && idx < (int)carpeta.items.size()) {
            vector<string> lineas;
            lineas.push_back("<<< SUBTÍTULO: " + carpeta.items[idx].subtitulo + " >>>");
            lineas.push_back(carpeta.items[idx].contenido);
            ejecutarTeleprompter(carpeta.nombreCarpeta + " -> " + carpeta.items[idx].subtitulo, lineas, delayMs);
        } else if (opcion == (int)carpeta.items.size() + 1) {
            vector<string> lineasTotales;
            lineasTotales.push_back("=== CARPETA COMPLETA: " + carpeta.nombreCarpeta + " ===");
            for (const auto& item : carpeta.items) {
                lineasTotales.push_back("");
                lineasTotales.push_back("--- [SUBTÍTULO: " + item.subtitulo + "] ---");
                lineasTotales.push_back(item.contenido);
            }
            ejecutarTeleprompter("CARPETA: " + carpeta.nombreCarpeta, lineasTotales, delayMs);
        }
    }
}

void menuConfiguraciones(int& delayMs) {
    limpiarPantalla();
    cout << "=======================================================================" << endl;
    cout << " CONFIGURACIÓN GLOBAL" << endl;
    cout << "=======================================================================" << endl;
    cout << " Velocidad actual (delay): " << delayMs << " ms" << endl;
    cout << " Ingrese el nuevo delay en ms (ej. 300 para más rápido, 800 más lento): ";
    
    int nuevoDelay;
    if (cin >> nuevoDelay && nuevoDelay > 10) {
        delayMs = nuevoDelay;
        cout << "¡Velocidad actualizada con éxito!\n";
    } else {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada inválida.\n";
    }
    cout << "Presiona ENTER para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}

int main() {
    vector<CarpetaPrompter> arbolCarpetas;

    CarpetaPrompter cap1;
    cap1.nombreCarpeta = "1. Tablas y Schemas";
    cap1.items.push_back({"TABLE_settings", "Columns:\n  - server_version INTEGER NOT NULL\n  - create_time    BIGINT  NOT NULL"});
    cap1.items.push_back({"TABLE_tiles", "Columns:\n  - layer_id TEXT NOT NULL\n  - hashed_tile_key BLOB NOT NULL\n  - expiration_time BIGINT NOT NULL\n  - priority BIGINT NOT NULL"});
    cap1.items.push_back({"TABLE_resources", "Columns:\n  - hashed_resource_key BLOB NOT NULL\n  - priority BIGINT NOT NULL"});
    cap1.items.push_back({"TABLE_pinned_tiles", "Columns:\n  - hashed_tile_key BLOB NOT NULL\n  - pin_id BLOB NOT NULL"});
    arbolCarpetas.push_back(cap1);

    CarpetaPrompter cap2;
    cap2.nombreCarpeta = "2. Índices y Optimización";
    cap2.items.push_back({"INDEX_tiles_expiration", "CREATE INDEX ix_tile_cache_expiration ON tiles(expiration_time);"});
    cap2.items.push_back({"INDEX_tiles_priority", "CREATE INDEX ix_priority ON tiles(priority);"});
    cap2.items.push_back({"INDEX_resources_expiration", "CREATE INDEX ix_resource_cache_expiration_time ON resources(expiration_time);"});
    arbolCarpetas.push_back(cap2);

    int delayMsGlobal = 500;

    while (true) {
        limpiarPantalla();
        cout << "=======================================================================" << endl;
        cout << "          TELEPROMPTER CLI :: NAVEGADOR MULTIPLATAFORMA              " << endl;
        cout << "=======================================================================" << endl;
        cout << " Seleccione una Carpeta / Módulo:" << endl << endl;

        for (size_t i = 0; i < arbolCarpetas.size(); ++i) {
            cout << "  [" << (i + 1) << "] " << arbolCarpetas[i].nombreCarpeta 
                 << " (" << arbolCarpetas[i].items.size() << " subtítulos)" << endl;
        }
        cout << endl;
        cout << "  [C] Configurar velocidad" << endl;
        cout << "  [0] Salir" << endl;
        cout << "-----------------------------------------------------------------------" << endl;
        cout << " Selecciona una opción: ";

        char seleccion;
        if (!(cin >> seleccion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (seleccion == '0') {
            limpiarPantalla();
            cout << "Saliendo del teleprompter. ¡Hasta luego!\n";
            break;
        } else if (seleccion == 'c' || seleccion == 'C') {
            menuConfiguraciones(delayMsGlobal);
        } else {
            int idxCarpeta = seleccion - '1';
            if (idxCarpeta >= 0 && idxCarpeta < (int)arbolCarpetas.size()) {
                menuCarpeta(arbolCarpetas[idxCarpeta], delayMsGlobal);
            }
        }
    }

    return 0;
}