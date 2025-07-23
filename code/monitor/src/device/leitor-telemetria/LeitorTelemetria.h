#include <Arduino.h>
#include "src/device/logger-serial/LoggerSerial.h"

#define VALOR_MIN_DO_SENSOR 300
#define VALOR_MAX_DO_SENSOR 2800
#define PINO_ANALOGICO_DO_SENSOR 34

class LeitorTelemetria
{
  private:
    bool deveMockarSensor;
    int mockaValorDoSensor();
  
  public:
    LeitorTelemetria(bool deveMockarSensor);
    int obtemLeitura();
};