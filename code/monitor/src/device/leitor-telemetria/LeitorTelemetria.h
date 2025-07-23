/*
* LeitorTelemetria.h:
* Header do LeitorTelemetria, utilizado para pré-declarar os componentes utilizados na coleta de dados do sensor.
*/

// Inclusão de bibliotecas e headers de outros componentes.
#include <Arduino.h>
#include "src/device/logger-serial/LoggerSerial.h"


// Constantes utilizadas
#define VALOR_MIN_DO_SENSOR 300
#define VALOR_MAX_DO_SENSOR 2800
#define PINO_ANALOGICO_DO_SENSOR 34


/// @brief Protótipo da classe LeitorTelemetria, utilizada para coletar dados reais do sensor ou simulá-los.
class LeitorTelemetria
{
  private:
    bool deveMockarSensor;
    int mockaValorDoSensor();
  
  public:
    LeitorTelemetria(bool deveMockarSensor);
    int obtemLeitura();
};