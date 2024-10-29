#include "BaseHechos.hpp"

const string BaseHechos::PATRON_NUMERO = "^[[:digit:]]+$";
const string BaseHechos::PATRON_HECHO = "^([[:alnum:]]+), FC=(-?(?:1|0\\.[[:digit:]]+?))$";
const string BaseHechos::PATRON_PRE_OBJETIVO = "Objetivo";
const string BaseHechos::PATRON_OBJETIVO = "^[[:alnum:]]+$";
const string BaseHechos::ERR_NO_EXISTE_BH = "BaseHechos: Error. No se ha podido abrir el fichero proporcionado.\n";
const string BaseHechos::ERR_NUM_HECHOS = "BaseHechos: Error. Comprueba que la primera línea contiene el numero de hechos del fichero.\n";
const string BaseHechos::ERR_RANGO = "BaseHechos: Error. La cantidad de hechos dada no es un número representable. Reduce la cantidad de hechos.\n";
const string BaseHechos::ERR_NO_HECHOS = "BaseHechos: Error. El fichero no contiene ningún hecho.\n";
const string BaseHechos::ERR_FORMATO_HECHO = "BaseHechos: Error. La línea leída no presenta el formato adecuado usado para representar un hecho.\nTambién es posible que el fichero contenga menos hechos de los especificados si se ha encontrado el token \"Objetivo\".\n";
const string BaseHechos::ERR_RANGO_CERTEZA = "BaseHechos: Error. El factor de certeza dado no es un número representable. Comprueba que es correcto.\n";
const string BaseHechos::ERR_PRE_OBJETIVO = "BaseHechos: Error. No se ha podido identificar el token \"Objetivo\". Es posible que el fichero contenga más hechos de los especificados.\n";
const string BaseHechos::ERR_OBJETIVO = "BaseHechos: Error. El formato del hecho objetivo no es correcto.\n";
const string BaseHechos::ERR_FIN_BH = "BaseHechos: Error. El fichero proporcionado como base de hechos no presenta un formato correcto al final.\n";
const string BaseHechos::ERR_HECHO_DUPLICADO = "BaseHechos: Error. El hecho leído ya se encuentra en la base de hechos.\n";

const regex BaseHechos::ER_NUMERO = regex(BaseHechos::PATRON_NUMERO);
const regex BaseHechos::ER_HECHO = regex(BaseHechos::PATRON_HECHO);
const regex BaseHechos::ER_OBJETIVO = regex(BaseHechos::PATRON_OBJETIVO);

BaseHechos::BaseHechos(string &nombreFicheroBH)
{
    // Abrimos el fichero para leerlo.
    ifstream ficheroBH(nombreFicheroBH);
    if (!ficheroBH.is_open())
        throw runtime_error(ERR_NO_EXISTE_BH + "En \"" + nombreFicheroBH + "\"");

    string linea;

    // Obtenemos la primera línea y verificamos que sea un número.
    getline(ficheroBH, linea);
    if (!regex_match(linea, ER_NUMERO))
        throw runtime_error(ERR_NUM_HECHOS + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");

    try
    {
        // El número debe ser representable como un entero.
        this->numeroHechos = stoi(linea);
    }
    catch (...)
    {
        throw runtime_error(ERR_RANGO + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");
    }

    // No se pueden dar 0 hechos.
    if (this->numeroHechos == 0)
        throw runtime_error(ERR_NO_HECHOS + "En \"" + nombreFicheroBH + "\".");

    // Se leen tantas líneas como hechos dados.
    for (unsigned i = 0; i < this->numeroHechos; i++)
    {
        linea.clear();
        getline(ficheroBH, linea);

        smatch coincidencia;
        // Verificamos el formato del hecho.
        if (!regex_match(linea, coincidencia, ER_HECHO))
            throw runtime_error(ERR_FORMATO_HECHO + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");

        // Extraemos los campos del hecho para insertarlos en nuestro mapeado.
        string nombreHecho = coincidencia.str(1);
        double factorCerteza;
        try
        {
            // Convertimos el factor de certeza a un número en coma flotante de doble precisión (siempre que pueda ser representado).
            factorCerteza = stod(coincidencia.str(2));
        }
        catch (...)
        {
            throw runtime_error(ERR_RANGO_CERTEZA + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");
        }

        // No pueden existir hechos duplicados.
        if (this->hechos.insert({nombreHecho, factorCerteza}).second == false)
            throw runtime_error(ERR_HECHO_DUPLICADO + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");
    }

    // Verificamos que la sigueinte línea es "Objetivo"
    getline(ficheroBH, linea);
    if (linea != PATRON_PRE_OBJETIVO)
        throw runtime_error(ERR_PRE_OBJETIVO + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");

    // Obtenemos el objetivo del fichero
    getline(ficheroBH, linea);
    if (!regex_match(linea, ER_OBJETIVO))
        throw runtime_error(ERR_OBJETIVO + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");

    this->hechoObjetivo = linea;

    // Si quedan datos en el fichero no va a ser aceptado como base de hechos.
    if (!ficheroBH.eof())
        throw runtime_error(ERR_FIN_BH + "En \"" + nombreFicheroBH + "\" línea \"" + linea + "\".");
}

BaseHechos::~BaseHechos(void) {}

string &BaseHechos::getObjetivo(void)
{
    return this->hechoObjetivo;
}

bool BaseHechos::contieneHecho(string &hecho)
{
    return (this->hechos.find(hecho) != this->hechos.end());
}

double BaseHechos::getFactorCerteza(string &hecho)
{
    return this->hechos.at(hecho);
}

bool BaseHechos::insertarHecho(string &hecho, double &factorCerteza)
{
    if ((this->hechos.insert({hecho, factorCerteza})).second)
    {
        this->numeroHechos++;
        return true;
    }
    return false;
}

string BaseHechos::debugInfo(void)
{
    string dbgInfo = "#debug BaseHechos:\n{\n\tnumeroHechos = " + to_string(this->numeroHechos) + "\n";
    dbgInfo += "\thechoObjetivo = " + this->hechoObjetivo + "\n\thechos = \n";
    for (auto i : this->hechos)
    {
        dbgInfo += "\t\t{Hecho = " + i.first + ", FC = " + to_string(i.second) + "}\n";
    }
    dbgInfo += "}";
    return dbgInfo;
}
