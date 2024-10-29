#ifndef __BASE_CONOCIMIENTO_HPP__
#define __BASE_CONOCIMIENTO_HPP__

#include <map>
#include <regex>
#include <vector>
#include <string>
#include <fstream>
#include "Regla.hpp"

using namespace std;

class BaseConocimiento
{
private:
    /// @brief Patrón de la expresión regular a continuación.
    static const string PATRON_NUMERO;

    /// @brief Mensajes de error expulsados en caso de fallo.
    static const string ERR_NO_EXISTE_BC;
    static const string ERR_NUM_REGLAS;
    static const string ERR_RANGO;
    static const string ERR_NO_REGLAS;
    static const string ERR_POCAS_REGLAS;
    static const string ERR_REGLA;
    static const string ERR_MUCHAS_REGLAS;
    static const string ERR_REGLA_DUPLICADA;

    /// @brief Expresión regular utilizada para verificar el formato de la base de conocimiento.
    static const regex ER_NUMERO;

    /// @brief Elementos de la base de conocimiento.
    unsigned numeroReglas;
    multimap<string, Regla> reglas;

public:
    /// @brief Constructor de la base de conocimientos a partir de un fichero.
    /// @param nombreFicheroBC Referencia al nombre del fichero de la base de conocimientos.
    BaseConocimiento(string &nombreFicheroBC);

    BaseConocimiento(void) = default;
    ~BaseConocimiento(void);

    /// @brief Función de equparación de la base de conocmiento.
    /// @param consecuente Referencia al string del consecuente que se desea equiparar.
    /// @return Vector con la lista de reglas que producen el consecuente dado.
    vector<Regla> equiparar(string &consecuente);

    string debugInfo(void);
};

#endif