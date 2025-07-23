// Renomeie este template para "parametros.h" e preencha os parâmetros
// indicados para que o dispositivo possa operar corretamente.

// Configurações do IoT Hub

// Dados de autenticação do Esp32 disponibilizados após o registro do dispositivo IoT no Azure
#define CFG_IOTHUB_DEVICE_ID "<preencha-aqui>"
#define CFG_IOTHUB_DEVICE_KEY "<preencha-aqui>"
#define CFG_IOTHUB_FQDN "<preencha-aqui>"
#define CFG_IOTHUB_URI_MQTT "mqtts://" CFG_IOTHUB_FQDN

/*********************************************************************/

// Wi-Fi

// Indica se o Esp32 permitirá escolher a rede Wi-Fi com a qual irá se conectar em tempo de execução (1)
// ou se irá conectar em uma rede Wi-Fi fixa (0) 
#define CFG_UTILIZAR_CONEXAO_DINAMICA 1

// SSID e senha do AccessPoint que o Esp32 abrirá para que o usuário se conecte e escolha a rede Wi-Fi
// com a qual a placa deverá se conectar, caso CFG_UTILIZAR_CONEXAO_DINAMICA = 1.
// OBS: Uma vez conectado na rede especificada, o Esp32 irá conectar automaticamente a cada inicialização,
// só reabrindo o AP para escolher uma nova rede Wi-Fi caso não consiga conectar na especificada anteriormente.
#define CFG_WIFI_SSID_AP CFG_IOTHUB_DEVICE_ID
#define CFG_WIFI_SENHA_AP "12345678"

// SSID e Senha da rede Wi-Fi com a qual o Esp32 sempre deverá se conectar, caso CFG_UTILIZAR_CONEXAO_DINAMICA = 0
#define CFG_WIFI_SSID_FIXO ""
#define CFG_WIFI_SENHA_FIXA ""

/*********************************************************************/

// Configurações Gerais

// Intervalo de tempo, em milisegundos, entre cada envio de mensagem ao IoT Hub
#define CFG_INTERVALO_ENVIO_MSG_EM_MILISSEGS 1000 * 60 * 2

// Indica se o Esp32 deve mockar a leitura do sensor (1) ou obter o valor analógico real (0)
#define CFG_DEVE_MOCKAR_VALOR_DO_LDR 0

// Indica se o Esp32 deve mandar as medições ao IoT Hub (1) ou não (0)
#define CFG_DEVE_ENVIAR_MSG 1

// Indica a cada quantas mensagens de Ping o Esp32 deve enviar uma mensagem com as medições ao IoTHub
// Ex: com um valor 15, o Esp32 irá enviar 14 mensagens de Ping e, somente no 15º envio, as medições serão
// extraídas e enviadas ao IoTHub, reiniciando a contagem. Associado a um intervalo de envio de 2 minutos
// entre cada mensagem, o silo associado a este Esp32 teria um registro de medição a cada 30 minutos
// (15 mensagens x 2 minutos). 
#define CFG_MULTIPLO_DE_ENVIO_DE_TELEMETRIA 15