#ifndef __TIPO_REGLA_HPP__
#define __TIPO_REGLA_HPP__

/// @brief Enumerado que representa los diferentes tipos de reglas que pueden existir en nuestro SBR-FC.
enum TipoRegla : char
{
    Simple = '\0',
    Disyuncion = 'o',
    Conjuncion = 'y',
};

#endif