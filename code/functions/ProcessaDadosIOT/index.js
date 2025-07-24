/*
* Arquivo central da function ProcessaDadosIOT. Aqui são montados e inicializados todos os componentes utilizados
* para receber e processar as requisições enviadas pelos Esp32 dos silos, sejam mensagens de Ping ou medição.
*/

// Inclusão dos componentes
const OperacoesBD = require('./servicos/operacoesBD');
const RegistroMedicao = require('./servicos/registroMedicao');


// Exportação da função principal (o código da Azure Function propriamente dita).
// quando uma requisição à function é feita, o Azure disponibiliza o parâmetro "context", uma espécie de "sessão".
// Adicionalmente, é disponibilizado o objeto "mensagensIoTHub", com as mensagens enviadas pelo IoT Hub, que é o
// que dispara o evento.
module.exports = (context, mensagensIoTHub) => {
  context.log(`Function do IoTHub chamada para tratar ${mensagensIoTHub.length} mensagens`);

  const operacoesBD = new OperacoesBD();
  const registroMedicao = new RegistroMedicao(operacoesBD, context);
  const promises = [];

  // itera por todas as mensagens recebidas (pode chegar mais de uma ao mesmo tempo) e as prepara para execução.
  // junta as operações envolvidas em uma lista de promises que serão executadas em paralelo.
  mensagensIoTHub.forEach(stringDaMensagem => {
    context.log(`Mensagem recebida: ${stringDaMensagem}`);

    const mensagemIOT = JSON.parse(stringDaMensagem);
    if (mensagemIOT.comando == 'MEDICAO')
      promises.push(registroMedicao.handle(mensagemIOT));

    // sempre registra a interação da placa no BD, seja a mensagem de Ping ou de medição.
    promises.push(operacoesBD.registraInteracaoDaPlaca(mensagemIOT.idPlaca, mensagemIOT.timestamp));
  });

  // executa todas as operações em paralelo, fecha a conexão com o BD e encerra a execução da function.
  Promise.all(promises)
    .then(() => {
      context.log('Operação Completa');
    })
    .catch(erro => {
      context.log("Erro na operação:");
      context.log(erro);
    })
    .finally(() => {
      operacoesBD.desconecta();
      context.done();
    });
};