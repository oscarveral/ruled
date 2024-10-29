#ifndef __REGLA_HPP__
#define __REGLA_HPP__

#include <regex>
#include <string>
#include <vector>
#include "TipoRegla.hpp"

using namespace std;

class Regla
{
private:
    /// @brief Patrones de las expresiones regulares a continuación.
    static const string PATRON_REGLA;
    static const string PATRON_ANTECEDENTES;

    /// @brief Diferentes mensajes a expulsar en caso de error.
    static const string ERR_FORMATO;
    static const string ERR_RANGO;
    static const string ERR_RECURSIVIDAD;

    /// @brief Expresiones regulares utilizadas para verificar el formato de la regla.
    static const regex ER_REGLA;
    static const regex ER_ANTECEDENTES;

    /// @brief Elementos de una regla.
    string identificador;
    vector<string> antecedentes;
    TipoRegla tipoRegla;
    string consecuente;
    double factorCerteza;

public:
    /// @brief Constructor de objetos tipo regla a partir de una referencia a un string que contenga los datos de construcción.
    /// @param datos String que contiene los datos necesarios para crear una regla.
    /// @throws runtime_exception si ocurre algun error al crear la regla debido al formato.
    Regla(string &datos);
    
    Regla() = default;
    ~Regla(void);

    bool operator==(const Regla &otra) const;

    /// @brief Función utilizada para obtener el consecuente de la regla.
    /// @return Referencia al string que representa al consecuente de la regla.
    string &getConsecuente(void);

    /// @brief Función utilizada para obtener la lista de antecedentes de la regla.
    /// @return Referencia al vector con los strings que representan a cada uno de los antecedentes.
    vector<string> &getAntecedentes(void);

    /// @brief Función utilizada para obtener el tipo de la regla. 
    /// @return Referencia al elemento del enumerado TipoRegla que representa su tipo.
    TipoRegla &getTipoRegla(void);

    /// @brief Función utilizada para obtener el factor de certeza de la regla. 
    /// @return Referencia al número de coma flotante de doble precisión que contiene el valor de certeza de la regla.
    double &getFactorCerteza(void);

    /// @brief Función utilizada para obtener el indentificador de la regla. 
    /// @return Referencia al string que contiene el identificador o nombre de la regla.
    string &getIdentificador(void);

    /// @brief Función utilizada para obtener la regla como un string. 
    /// @return string bien formateado que representa la regla.
    string toString(void);
    
    string debugInfo(void);
};

#endif