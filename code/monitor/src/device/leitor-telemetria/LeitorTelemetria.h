#include <Arduino.h>
#include "src/device/logger-serial/LoggerSerial.h"

#define VALOR_MIN_DO_LDR 300
#define VALOR_MAX_DO_LDR 2800
#define PINO_ANALOGICO_DO_LDR 34

class LeitorTelemetria
{
  private:
    char* IDPlaca;
    bool deveMockarLDR;
    int mockaValorDoSensor();
  
  public:
    LeitorTelemetria(char* IDPlaca, bool deveMockarLDR);
    int obtemLeitura();
};