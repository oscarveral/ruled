#include "Regla.hpp"

const string Regla::PATRON_REGLA = "^([[:alnum:]]+?): Si ([[:alnum:]]+?)(?:((?: o [[:alnum:]]+?)+)|((?: y [[:alnum:]]+?)+))? Entonces ([[:alnum:]]+?), FC=(-?(?:1|0\\.[[:digit:]]+?))$";
const string Regla::PATRON_ANTECEDENTES = " (?:o|y) ([[:alnum:]]+)";
const string Regla::ERR_FORMATO = "Regla: Error de formato de los datos utilizados. Comprueba que la cadena de datos de la regla está bien formateada.\n";
const string Regla::ERR_RANGO = "Regla: Error. El factor de certeza dado no es un número representable. Comprueba que es correcto.\n";
const string Regla::ERR_RECURSIVIDAD = "Regla: Error. El consecuente de la regla es uno de los antecedentes. Comprueba que es correcto.\n";

const regex Regla::ER_REGLA = regex(Regla::PATRON_REGLA);
const regex Regla::ER_ANTECEDENTES = regex(Regla::PATRON_ANTECEDENTES);

Regla::Regla(string &datos)
{
    // Se aplica la expresión regular para verificar el formato de la cadena de datos dada.
    smatch resultado;
    bool esCadenaValida = regex_match(datos, resultado, ER_REGLA);
    if (!esCadenaValida)
        throw runtime_error(ERR_FORMATO + "En línea \"" + datos + "\".");

    // Se extraen los diferentes campos de la regla para construir nuestro objeto.
    this->identificador = resultado.str(1);

    this->antecedentes = vector<string>(0);
    this->antecedentes.push_back(resultado.str(2));

    // Dependiendo del tipo de regla será necesario extraer más antecedentes.
    if (resultado.str(3) != "")
    {
        this->tipoRegla = Disyuncion;
        smatch antecedente;
        string otrosAntecedentes = resultado.str(3);
        // Iteramos sobre los resultados de aplicar la expresión regular de antecedentes para insertarlos en nuestra lista.
        for (sregex_iterator it = sregex_iterator(otrosAntecedentes.begin(), otrosAntecedentes.end(), ER_ANTECEDENTES); it != sregex_iterator(); it++)
        {
            antecedente = *it;
            this->antecedentes.push_back(antecedente.str(1));
        }
    }
    else if (resultado.str(4) != "")
    {
        this->tipoRegla = Conjuncion;
        smatch antecedente;
        string otrosAntecedentes = resultado.str(4);
        // Iteramos sobre los resultados de aplicar la expresión regular de antecedentes para insertarlos en nuestra lista.
        for (sregex_iterator it = sregex_iterator(otrosAntecedentes.begin(), otrosAntecedentes.end(), ER_ANTECEDENTES); it != sregex_iterator(); it++)
        {
            antecedente = *it;
            this->antecedentes.push_back(antecedente.str(1));
        }
    }
    else
        this->tipoRegla = Simple;

    this->consecuente = resultado.str(5);

    // Una regla no puede tener su consecuente de antecedente.
    if (find(this->antecedentes.begin(), this->antecedentes.end(), this->consecuente) != this->antecedentes.end())
        throw runtime_error(ERR_RECURSIVIDAD + "En línea \"" + datos + "\".");

    try
    {
        // Convertimos el factor de certeza a un número en coma flotante de doble precisión (siempre que pueda ser representado).
        this->factorCerteza = stod(resultado.str(6));
    }
    catch (...)
    {
        throw runtime_error(ERR_RANGO + "En línea \"" + datos + "\".");
    }
}

Regla::~Regla(void) {}

// Implementación del operador de igualdad entre reglas.
bool Regla::operator==(const Regla &otra) const
{
    if (this->identificador != otra.identificador)
        return false;
    if (this->consecuente != otra.consecuente)
        return false;
    if (this->tipoRegla != otra.tipoRegla)
        return false;
    if (this->factorCerteza != otra.factorCerteza)
        return false;
    if (this->antecedentes != otra.antecedentes)
        return false;

    return true;
}

string &Regla::getConsecuente(void)
{
    return this->consecuente;
}

vector<string> &Regla::getAntecedentes(void)
{
    return this->antecedentes;
}

TipoRegla &Regla::getTipoRegla(void)
{
    return this->tipoRegla;
}

double &Regla::getFactorCerteza(void)
{
    return this->factorCerteza;
}

string &Regla::getIdentificador(void)
{
    return this->identificador;
}

string Regla::toString(void)
{
    // Se construye el string original que originó la regla.
    string regla = this->identificador + ": Si ";
    
    regla += this->antecedentes[0] + " ";
    for (size_t i = 1; i < this->antecedentes.size(); i++)
    {
        regla += this->tipoRegla;
        regla += " " + this->antecedentes[i] + " ";
    }
    regla += "Entonces " + this->consecuente + ", FC=" + to_string(this->factorCerteza);
    return regla;
}

string Regla::debugInfo(void)
{
    string dbgInfo = "#debug Regla [" + this->identificador + ": Si ";
    if (this->antecedentes.size() > 0)
    {
        dbgInfo += this->antecedentes[0] + " ";
        for (long unsigned int i = 1; i < this->antecedentes.size(); i++)
        {
            dbgInfo += this->tipoRegla;
            dbgInfo += " " + this->antecedentes[i] + " ";
        }
    }
    dbgInfo += "Entonces " + this->consecuente + ", " + to_string(this->factorCerteza) + "]";
    return dbgInfo;
}
