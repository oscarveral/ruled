#ifndef __MOTOR_INFERENCIA_HPP__
#define __MOTOR_INFERENCIA_HPP__

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "BaseHechos.hpp"
#include "BaseConocimiento.hpp"

using namespace std;

class MotorInferencia
{
private:
    /// @brief Constante que se devolverá en caso de no existir una meta.
    static const double NO_VERIFICADO;

    /// @brief Mensajes de error expulsados en caso de fallo.
    static const string ERR_FICHERO_LOG;
    static const string ERR_FATAL_INSERCION;

    /// @brief Mensajes de información que se introducirán en el log final.
    static const string LOG_INICIO;
    static const string LOG_INICIO_BH;
    static const string LOG_INICIO_BC;
    static const string LOG_OBJETIVO;
    static const string LOG_SELECCION_META;
    static const string LOG_META_PRESENTE;
    static const string LOG_REGLAS_META;
    static const string LOG_NO_REGLAS;
    static const string LOG_REGLA_SELECCIONADA;
    static const string LOG_NUEVAS_METAS;
    static const string LOG_CERTEZA_CALCULADA;
    static const string LOG_PARA_META;
    static const string LOG_PARA_REGLA;
    static const string LOG_PARA_ANTECEDENTES;
    static const string LOG_IGNORANDO_META;
    static const string LOG_CERTEZA_META_1;
    static const string LOG_CERTEZA_META_2;
    static const string LOG_CALCULO_CERTEZA_CASO_1;
    static const string LOG_CALCULO_CERTEZA_CASO_2;
    static const string LOG_CALCULO_CERTEZA_CASO_3;
    static const string LOG_NO_REGLAS_VERIFICADAS;
    static const string LOG_USANDO;
    static const string LOG_USANDO_REGLAS;
    static const string LOG_ERR_BH;
    static const string LOG_ERR_BC;
    static const string LOG_EXITO;
    static const string LOG_FRACASO;

    /// @brief Elementos del motor de inferencia.
    BaseConocimiento bc;
    BaseHechos bh;
    int profundidad;
    ofstream log;

    /// @brief Funcín para calcular el factor de certeza en el caso 1.
    /// @param certezas Vector de las certezas de los antecedentes a combinar.
    /// @param tipoRegla Tipo de la regla sobre la que se están combinando antecedentes.
    /// @return Número de coma flotante con la certeza calculada.
    double calcularCertezaCaso1(vector<double> &certezas, TipoRegla &tipoRegla);

    /// @brief Funcín para calcular el factor de certeza en el caso 2.
    /// @param certezas Vector de las certezas producidas por todas las reglas a combinar.
    /// @return Número de coma flotante con la certeza calculada.
    double calcularCertezaCaso2(vector<double> &certezas);

    /// @brief Funcín para calcular el factor de certeza en el caso 3.
    /// @param certezaReglaAntecedente Certeza del antecedente de la regla.
    /// @param certezaReglaConsecuente Certeza de la aplicación de la regla.
    /// @return Número de coma flotante con la certeza calculada.
    double calcularCertezaCaso3(double &certezaReglaAntecedente, double &certezaReglaConsecuente);

    /// @brief Función que imprime en el log las tabulaciones necesarias según el nivel de profundidad.
    void tabularInfo(void);

    /// @brief Fumción que imprime en el log con las tabulaciones correctas.
    /// @param info string que se desea imprimir en el log.
    void logInfo(string info);

    /// @brief Función que imprime en el log un preludio y a continuación una lista de reglas.
    /// @param preludio string con el preludio que se desea imprimir.
    /// @param lista referencia a la lista de reglas que se desean imprimir.
    void logReglas(string preludio, vector<Regla> &lista);

    /// @brief Función que imprime en el log un preludio y a continuación una lista de metas.
    /// @param preludio string con el preludio que se desea imprimir.
    /// @param metas referencia a la lista de metas que se desean imprimir.
    void logMetas(string preludio, vector<string> &metas);

public:
    /// @brief Constructor del motor de inferencia a partid de los ficheros de BC, BH y el fichero de salida del log.
    /// @param nombreFicheroBC Referencia al nombre del fichero de la BC.
    /// @param nombreFicheroBH Referencia al nombre del fichero de la BH.
    /// @param nombreFicheroLog Referencia al nombre del fichero que se usará como log.
    MotorInferencia(string &nombreFicheroBC, string &nombreFicheroBH, string &nombreFicheroLog);

    MotorInferencia() = default;
    ~MotorInferencia(void);

    /// @brief Función principal del motor de inferencia que realizará el proceso de inferencia para el objetivo de la BH. 
    /// @return Devuelve true si se pudo construir el objetivo y false en caso contrario.
    bool inferir(void);

    /// @brief Función de verificación que verifica la existencia de una meta dada realizando un proceso de inferencia para encontrarla o construirla.
    /// @param meta Referencia al string de la meta que se desea verificar.
    /// @return Número de coma flotante con el factor de certeza de la meta verificada o la constate NO_VERIFICADO si no se pudo construir la meta dada.
    double verificar(string &meta);

    string debugInfo(void);
};

#endif