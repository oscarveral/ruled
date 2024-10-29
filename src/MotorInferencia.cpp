#include "MotorInferencia.hpp"

const double MotorInferencia::NO_VERIFICADO = -1.0;

const string MotorInferencia::ERR_FICHERO_LOG = "MotorInferencia: Error al abrir el fichero de log.\n";
const string MotorInferencia::ERR_FATAL_INSERCION = "MotorInferencia: Error fatal al insertar en la base de hechos. Ocurrió un problema inesperado.\n";

const string MotorInferencia::LOG_INICIO = "SBR-FC: Iniciando motor de inferencia.";
const string MotorInferencia::LOG_INICIO_BH = "SBR-FC: Construyendo base de hechos: ";
const string MotorInferencia::LOG_INICIO_BC = "SBR-FC: Construyendo base de conocimiento: ";
const string MotorInferencia::LOG_OBJETIVO = "SBR-FC: Iniciando proceso de inferencia.\n\nHecho objetivo: ";
const string MotorInferencia::LOG_SELECCION_META = "Meta seleccionada: ";
const string MotorInferencia::LOG_META_PRESENTE = "Meta presente en la base de hechos. Certeza: ";
const string MotorInferencia::LOG_REGLAS_META = "Reglas con las que construir la meta: ";
const string MotorInferencia::LOG_NO_REGLAS = "No hay reglas que construyan la meta seleccionada.";
const string MotorInferencia::LOG_REGLA_SELECCIONADA = "Regla seleccionada: ";
const string MotorInferencia::LOG_NUEVAS_METAS = "Nuevas metas: ";
const string MotorInferencia::LOG_CERTEZA_CALCULADA = "Factor certeza ";
const string MotorInferencia::LOG_PARA_META = " para la meta ";
const string MotorInferencia::LOG_PARA_REGLA = " para la regla ";
const string MotorInferencia::LOG_PARA_ANTECEDENTES = " para los antecedentes ";
const string MotorInferencia::LOG_IGNORANDO_META = ". Se ignora.";
const string MotorInferencia::LOG_CERTEZA_META_1 = "La meta ";
const string MotorInferencia::LOG_CERTEZA_META_2 = " tiene una certeza de ";
const string MotorInferencia::LOG_CALCULO_CERTEZA_CASO_1 = "Calculo certeza Caso 1: Combinacion de antecedentes. Certeza calculada ";
const string MotorInferencia::LOG_CALCULO_CERTEZA_CASO_2 = "Calculo certeza Caso 2: Adquisición incremental de evidencia. Certeza calculada ";
const string MotorInferencia::LOG_CALCULO_CERTEZA_CASO_3 = "Calculo certeza Caso 3: Encadenación de evidencias. Certeza calculada ";
const string MotorInferencia::LOG_NO_REGLAS_VERIFICADAS = "Ninguna de las reglas pueden verificar la meta ";
const string MotorInferencia::LOG_USANDO = " usando ";
const string MotorInferencia::LOG_USANDO_REGLAS = " usando reglas ";
const string MotorInferencia::LOG_ERR_BH = "SBR-FC: Error en la construcción de la base de hechos.";
const string MotorInferencia::LOG_ERR_BC = "SBR-FC: Error en la construcción de la base de conocimiento.";
const string MotorInferencia::LOG_EXITO = "\nSBR-FC: Se ha verificado el objetivo ";
const string MotorInferencia::LOG_FRACASO = "\nSBR-FC: No se ha podido verificar el objetivo ";

double MotorInferencia::calcularCertezaCaso1(vector<double> &certezas, TipoRegla &tipoRegla)
{
	// Cogemos como certeza la del primer elemento del vector.
    double certeza = certezas[0];

    switch (tipoRegla)
    {
    case Simple:
		// Para reglas simples el vector solo tiene la certeza seleccionada y es la que tenemos que devolver.
        break;
    case Conjuncion:
		// Para conjunciones la ceretza resultante de los antecedentes es el mínimo de todas las certezas.
        for (size_t i = 1; i < certezas.size(); i++)
            certeza = min(certeza, certezas[i]);
        break;
    case Disyuncion:
		// Para conjunciones la ceretza resultante de los antecedentes es el máximo de todas las certezas.
        for (size_t i = 1; i < certezas.size(); i++)
            certeza = max(certeza, certezas[i]);
        break;
    }
    return certeza;
}

double MotorInferencia::calcularCertezaCaso2(vector<double> &certezas)
{
	// Selecionamos la primera certeza como las certeza incremental resultante.
	double certezaIncremental = certezas[0];

	// Recorremos el resto del vector de certezas calculando la nueva certeza incremental en cada iteración según el caso.
	for (size_t i = 1; i < certezas.size(); i++)
		if (certezaIncremental >= 0 && certezas[i] >= 0)
			certezaIncremental = certezaIncremental + certezas[i] * (1 - certezaIncremental);
		else if (certezaIncremental < 0 && certezas[i] < 0)
			certezaIncremental = certezaIncremental + certezas[i] * (1 + certezaIncremental);
		else
			certezaIncremental = (certezaIncremental + certezas[i]) / (1 - min(abs(certezaIncremental), abs(certezas[i])));
	return certezaIncremental;
}

double MotorInferencia::calcularCertezaCaso3(double &certezaAntecedente, double &certezaRegla)
{
	// Cálculo de la certeza de la encadenación de evidencias.
	return certezaRegla * max(0.0, certezaAntecedente);
}

void MotorInferencia::tabularInfo()
{
	// Se imprimen tantas tabulaciones como profundidad.
	for (int i = 0; i < this->profundidad; i++)
		this->log << "\t";
}

void MotorInferencia::logInfo(string info)
{
	// Se imprime la info tabulada correctamente.
	this->tabularInfo();
	this->log << info << endl;
}

void MotorInferencia::logReglas(string preludio, vector<Regla> &lista)
{
	// Se imprimen correctamente tabulados el preludio y la lista.
	this->tabularInfo();
	this->log << preludio << "{ ";
	this->log << lista[0].getIdentificador();
	for (size_t i = 1; i < lista.size(); i++)
		this->log << ", " << lista[i].getIdentificador() ;
	this->log << " }" << endl;
}

void MotorInferencia::logMetas(string preludio, vector<string> &metas)
{
	// Se imprimen correctamente tabulados el preludio y las metas.
	this->tabularInfo();
	this->log << preludio << "{ ";
	this->log << metas[0];
	for (size_t i = 1; i < metas.size(); i++)
		this->log << ", " << metas[i] ;
	this->log << " }" << endl;
}

MotorInferencia::MotorInferencia(string &nombreFicheroBC, string &nombreFicheroBH, string &nombreFicheroLog)
{
	// Abrimos el fichero de log.
	this->log.open(nombreFicheroLog);
	if (!this->log.is_open())
		throw runtime_error(ERR_FICHERO_LOG + "En \"" + nombreFicheroLog + "\".");
	this->profundidad = -1;
	this->logInfo(LOG_INICIO);
	this->logInfo(LOG_INICIO_BC + nombreFicheroBC);
	
	// Inicializamos la base de conocimiento.
	try
	{
		this->bc = BaseConocimiento(nombreFicheroBC);
	}
	catch (const runtime_error &err)
	{
		this-> logInfo(LOG_ERR_BC);
		throw err;
	};
	this->logInfo(LOG_INICIO_BH + nombreFicheroBH);
	
	// Inicializamos la base de hechos.
	try
	{
		this->bh = BaseHechos(nombreFicheroBH);
	}
	catch(const runtime_error &err)
	{
		this-> logInfo(LOG_ERR_BH);
		throw err;
	}	
}

MotorInferencia::~MotorInferencia(void) {}

bool MotorInferencia::inferir(void)
{
	// La función de inferencia intenta verificar el objetivo de la base de hechos.
	this->logInfo(LOG_OBJETIVO + this->bh.getObjetivo());
	double resultado = this->verificar(this->bh.getObjetivo());

    if (resultado != NO_VERIFICADO)
	{
		this->logInfo(LOG_EXITO + this->bh.getObjetivo());
        return true;
	}
	this->logInfo(LOG_FRACASO + this->bh.getObjetivo());
	return false;
}

double MotorInferencia::verificar(string &meta)
{
	// Por cada nivel de recursion se incrementa en 1 la profundidad.
	this->profundidad++;

	this->logInfo(LOG_SELECCION_META + meta);

	// Si la base de hechos contiene el hecho pedido se vuelve retornando su factor de certeza
    if (this->bh.contieneHecho(meta))
	{
		this->logInfo(LOG_META_PRESENTE + to_string(this->bh.getFactorCerteza(meta)));
		this->profundidad--;
		return this->bh.getFactorCerteza(meta);
	}
    
	// Obtenemos las reglas que permiten construir la meta dada.
    vector<Regla> reglasAplicables = this->bc.equiparar(meta);
	// Si no hay reglas entonces no se puede construir.
    if (reglasAplicables.empty())
	{
		this->logInfo(LOG_NO_REGLAS);
		this->profundidad--;
    	return NO_VERIFICADO;
	}

	this->logReglas(LOG_REGLAS_META, reglasAplicables);

	// Vector de las certezas producidas por cada regla aplicada para obtener al meta.
	vector<double> certezasMeta (0);

	// Iteramos para cada regla.
    for (Regla regla : reglasAplicables)
    {
		this->logInfo(LOG_REGLA_SELECCIONADA + regla.toString());
		// Los antecedentes de la regla son las nuevas metas a verificar.
        vector<string> nuevasMetas = regla.getAntecedentes();
		// Vector con las certezas de cada antecedente de la regla.
        vector<double> certezasNuevasMetas;

		this->logMetas(LOG_NUEVAS_METAS, nuevasMetas);

		// Iteramos sobre cada meta a verificar.
        for (string nuevaMeta : nuevasMetas)
        {
			// LLamada recursiva para verificarla.
			double certeza = this->verificar(nuevaMeta);
			this->logInfo(LOG_CERTEZA_CALCULADA + to_string(certeza) + LOG_PARA_META + nuevaMeta);
			// Introducimos la certeza calculada en el vector de certezas.
			certezasNuevasMetas.push_back(certeza);
		}

		// Se calcula el caso 1 para todos los antecedentes de la regla.
		double certezaAntecedente = this->calcularCertezaCaso1(certezasNuevasMetas, regla.getTipoRegla());
		this->logMetas(LOG_CALCULO_CERTEZA_CASO_1 + to_string(certezaAntecedente) + LOG_PARA_ANTECEDENTES, nuevasMetas);

		// Se calcula el caso 3 para saber la certeza final de la meta después de aplicar la regla.
		double certezaRegla = regla.getFactorCerteza();
		double certezaConsecuente = this->calcularCertezaCaso3(certezaAntecedente, certezaRegla);
		this->logInfo(LOG_CALCULO_CERTEZA_CASO_3 + to_string(certezaConsecuente) + LOG_PARA_META + meta + LOG_USANDO + regla.getIdentificador());

		// Introducimos la certeza final en el vector de certezas calculadas para cada regla aplicable.
		certezasMeta.push_back(certezaConsecuente);
    }
	
	// Calculamos la certeza resultante de todas las reglas aplicadas.
	double certezaMeta = this->calcularCertezaCaso2(certezasMeta);
	this->logReglas(LOG_CALCULO_CERTEZA_CASO_2 + to_string(certezaMeta) + LOG_PARA_META + meta + LOG_USANDO_REGLAS, reglasAplicables);

	// Insertamos el nuevo hecho calculado en la base de hechos. Si no se pudo insertar es un error fatal.
	if (this->bh.insertarHecho(meta, certezaMeta) == false)
		throw runtime_error(ERR_FATAL_INSERCION);

	this->logInfo(LOG_CERTEZA_META_1 + meta + LOG_CERTEZA_META_2 + to_string(certezaMeta));
	this->profundidad--;

	// Devolvemos la certeza final calculada
	return certezaMeta;
}

string MotorInferencia::debugInfo(void)
{
    string dbgInfo = "#debug MotorInferencia: \n" + this->bc.debugInfo() + "\n" + this->bh.debugInfo();
    return dbgInfo;
}
