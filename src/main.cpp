#include <iostream>

#include "MotorInferencia.hpp"

using namespace std;

const string ERR_ARGUMENTOS = "No se han podido procesar los argumentos del programa.";
const string GUIA_USO = "Uso: \".\\SBR-FC.exe ruta_o_nombre_fichero_BC ruta_o_nombre_fichero_BH\".";

int main(int argc, char **argv)
{
    // Comprobamos la cantidad de argumentos.
    if (argc != 3)
    {
        cerr << ERR_ARGUMENTOS << endl
             << GUIA_USO << endl;
        return EXIT_FAILURE;
    }

    // Extraemos los nombres de los ficheros.
    string ficheroBC(argv[1]);
    string ficheroBH(argv[2]);

    // Nombre del fichero con el proceso de inferencia realizado.
    string log_output = "SBR-FC Inference Process.txt";

    try
    {
        MotorInferencia sbr_fc(ficheroBC, ficheroBH, log_output);
        sbr_fc.inferir();
    }
    catch (const runtime_error &err)
    {
        cerr << err.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
