#include "BaseConocimiento.hpp"

const string BaseConocimiento::PATRON_NUMERO = "^[[:digit:]]+$";
const string BaseConocimiento::ERR_NO_EXISTE_BC = "BaseConocimiento: Error. No se ha podido abrir el fichero proporcionado.\n";
const string BaseConocimiento::ERR_NUM_REGLAS = "BaseConocimiento: Error. Comprueba que la primera línea contiene el numero de reglas del fichero.\n";
const string BaseConocimiento::ERR_RANGO = "BaseConocimiento: Error. La cantidad de reglas dada no es un número representable. Reduce la cantidad de reglas.\n";
const string BaseConocimiento::ERR_NO_REGLAS = "BaseConocimiento: Error. El fichero no contiene ninguna regla.\n";
const string BaseConocimiento::ERR_POCAS_REGLAS = "BaseConocimiento: Error. Se han proporcionado menos reglas de las esperadoas.\n";
const string BaseConocimiento::ERR_REGLA = "BaseConocimiento: Error. Ha fallado la construcción de un regla.\n";
const string BaseConocimiento::ERR_MUCHAS_REGLAS = "BaseConocimiento: Error. Se han proporcionado más reglas de las esperadas.\n";
const string BaseConocimiento::ERR_REGLA_DUPLICADA = "BaseConocimiento: Error. Se ha encontrado una regla duplicada.\n";

const regex BaseConocimiento::ER_NUMERO = regex(BaseConocimiento::PATRON_NUMERO);

BaseConocimiento::BaseConocimiento(string &nombreFicheroBC)
{
    // Arbimos el fichero de la BC.
    ifstream ficheroBC(nombreFicheroBC);
    if (!ficheroBC.is_open())
        throw runtime_error(ERR_NO_EXISTE_BC + "En \"" + nombreFicheroBC + "\".");

    string linea;

    // Obtenemos la cantidad de reglas que contiene.
    getline(ficheroBC, linea);
    if (!regex_match(linea, ER_NUMERO))
        throw runtime_error(ERR_NUM_REGLAS + "En \"" + nombreFicheroBC + "\" línea \"" + linea + "\".");

    try
    {
        // El número debe ser representable como un entero.
        this->numeroReglas = stoi(linea);
    }
    catch (...)
    {
        throw runtime_error(ERR_RANGO + "En \"" + nombreFicheroBC + "\" línea \"" + linea + "\".");
    }

    // No se pueden dar 0 reglas.
    if (this->numeroReglas == 0)
        throw runtime_error(ERR_NO_REGLAS + "En \"" + nombreFicheroBC + "\".");

    // Leemos cada regla.
    for (unsigned i = 0; i < this->numeroReglas; i++)
    {
        linea.clear();
        getline(ficheroBC, linea);

        // Si la línea está vacía entonces llegamos al final y hay menos reglas de las especificadas.
        if (linea.empty())
            throw runtime_error(ERR_POCAS_REGLAS + "En \"" + nombreFicheroBC + "\".");
        try
        {
            Regla reglaLeida = Regla(linea);

            // No podemos tener reglas duplicadas en el fichero.
            for (auto regla : this->reglas)
                if (regla.first == reglaLeida.getConsecuente() && regla.second == reglaLeida)
                    throw runtime_error(ERR_REGLA_DUPLICADA + "En \"" + nombreFicheroBC + "\".");
            //Insertamos la regla en el mapeado.
            this->reglas.insert({reglaLeida.getConsecuente(), reglaLeida});
        }
        catch (const std::exception &e)
        {
            // El fallo en la construcción de una regla provoca fallo general
            throw runtime_error(ERR_REGLA + "En \"" + nombreFicheroBC + "\".\n" + e.what());
        }
    }

    // Si quedan más líneas por procesar el fichero no es válido y es posible que contenga reglas de más.
    if (!ficheroBC.eof())
        throw runtime_error(ERR_MUCHAS_REGLAS + "En \"" + nombreFicheroBC + "\".");

    ficheroBC.close();
}

BaseConocimiento::~BaseConocimiento(void) {}

vector<Regla> BaseConocimiento::equiparar(string &consecuente)
{
    vector<Regla> reglasAplicables (0);

    // El proceso de equiparación retorna todas las reglas cuyo consecuente sea igual al dado.
    for (auto regla : this->reglas)
        if (regla.first == consecuente)
            reglasAplicables.push_back(regla.second);

    return reglasAplicables;
}

string BaseConocimiento::debugInfo(void)
{
    string dbgInfo = "#debug BaseConocimiento:\n{\n\tnumeroReglas = " + to_string(this->numeroReglas) + "\n\treglas = \n";
    for (auto i : this->reglas)
    {
        dbgInfo += "\t\t{Consecuente = " + i.first + ", " + i.second.debugInfo() + "}\n";
    }
    dbgInfo += "}";
    return dbgInfo;
}
