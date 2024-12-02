// Renomeie este template para "parametros.h" e preencha os parâmetros
// indicados para que o dispositivo possa operar corretamente.

// Configurações do IoT Hub
#define CFG_IOTHUB_DEVICE_ID "<preencha-aqui>"
#define CFG_IOTHUB_DEVICE_KEY "<preencha-aqui>"
#define CFG_IOTHUB_FQDN "<preencha-aqui>"
#define CFG_IOTHUB_URI_MQTT "mqtts://" CFG_IOTHUB_FQDN

// Configurações Wi-Fi
#define CFG_WIFI_SSID_AP CFG_IOTHUB_DEVICE_ID
#define CFG_WIFI_SENHA_AP "12345678"

// Configurações Gerais
#define CFG_INTERVALO_ENVIO_TELEMETRIA_EM_MILISSEGS 25000
#define CFG_DEVE_MOCKAR_VALOR_DO_LDR 1
#define CFG_DEVE_ENVIAR_TELEMETRIA 1
