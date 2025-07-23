/*
* GerenciadorDataHora.h:
* Header do GerenciadorDataHora, utilizado para inicializar e coletar data e hora utilizadas na execução do Esp32
*/

#ifndef TIME_CONFIGURATOR_H
#define TIME_CONFIGURATOR_H


// Inclusão dos headers
#include <Arduino.h>
#include <time.h>
#include <cstdlib>


// Constantes utilizadas
#define SERVIDORES_NTP "pool.ntp.org", "time.nist.gov"
#define ANO_INICIAL_EPOCH_UNIX 1900
#define DIA_13_NOV_2017_EM_UNIX_TIME 1510592825
#define TIME_ZONE_GMT -3
#define GMT_OFFSET_SECS (TIME_ZONE_GMT * 3600)


/// @brief Protótipo da classe GerenciadorDataHora
class GerenciadorDataHora
{
  public:
    GerenciadorDataHora();
    static void defineTimeZone();
    static String obtemTimestampAtual();
};

#endif