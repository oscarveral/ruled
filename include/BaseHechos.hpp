#ifndef __BASE_HECHOS_HPP__
#define __BASE_HECHOS_HPP__

#include <map>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

class BaseHechos
{
private:
    /// @brief Patrones de las expresiones regulares a continuación.
    static const string PATRON_NUMERO;
    static const string PATRON_HECHO;
    static const string PATRON_PRE_OBJETIVO;
    static const string PATRON_OBJETIVO;

    /// @brief Diferentes mensajes de error expulsados en la creación de la base de hechos.
    static const string ERR_NO_EXISTE_BH;
    static const string ERR_NUM_HECHOS;
    static const string ERR_RANGO;
    static const string ERR_NO_HECHOS;
    static const string ERR_FORMATO_HECHO;
    static const string ERR_RANGO_CERTEZA;
    static const string ERR_PRE_OBJETIVO;
    static const string ERR_OBJETIVO;
    static const string ERR_FIN_BH;
    static const string ERR_HECHO_DUPLICADO;

    /// @brief Expresiones regulares utilizadas para verificar el formato de la base de hechos.
    static const regex ER_NUMERO;
    static const regex ER_HECHO;
    static const regex ER_OBJETIVO;

    /// @brief Elementos de la base de hechos.
    unsigned numeroHechos;
    map<string, double> hechos;
    string hechoObjetivo;

public:
    /// @brief Constructor de una base de hechos a partir del nombre del fichero que la contiene.
    /// @param nombreFicheroBH Referencia al nombre del fichero que contiene la base de hechos.
    /// @throws runtime_exception si falla la construcción de la base de hechos.
    BaseHechos(string &nombreFicheroBH);

    BaseHechos(void) = default;
    ~BaseHechos(void);

    /// @brief Función utilizada para obtener el objetivo de inferencia de la base de hechos.
    /// @return Referencia al string que representa al objetivo de la base de hechos.
    string &getObjetivo(void);

    /// @brief Función utilizada para comprobar si un hecho está en la base de hechos.
    /// @param hecho Referencia al string del hecho que se desea buscar en la base de hechos.
    /// @return true si hecho está contenido en la base de hechos y false en cualquier otro caso.
    bool contieneHecho(string &hecho);

    /// @brief Función utilizada para obtener el factor de certeza asociado a un hecho.
    /// @param hecho Referencia al string del hecho del que se desea obtener su certeza.
    /// @return Número de coma flotante de doble precisión que representa el factor de certeza de hecho.
    double getFactorCerteza(string &hecho);

    /// @brief Función utilizada para insertar un hecho en la base de hechos.
    /// @param hecho Referencia al string del hecho que se desea insertar.
    /// @param factorCerteza Factor de certeza que se desea asociar al hecho.
    /// @return true si se ha insertado el hecho y false en caso contrario.
    bool insertarHecho(string &hecho, double &factorCerteza);

    string debugInfo(void);
};

#endif