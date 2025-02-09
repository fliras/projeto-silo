#include <Arduino.h>
#include "src/device/logger-serial/LoggerSerial.h"

#define VALOR_MIN_DO_LDR 0
#define VALOR_MAX_DO_LDR 4095
#define PINO_ANALOGICO_DO_LDR 34

class LeitorTelemetria
{
  private:
    char* IDPlaca;
    bool deveMockarLDR;
    float obtemPorcentagemDoSensor();
    int mockaValorDoSensor();
    float remapeiaLDRParaPorcentagem(float x, float in_min, float in_max, float out_min, float out_max);
    String montaPayload(float medicao);
  
  public:
    LeitorTelemetria(char* IDPlaca, bool deveMockarLDR);
    String obtemLeitura();
};