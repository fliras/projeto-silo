const OperacoesBD = require('./operacoesBD');
const RegistroMedicao = require('./registroMedicao');

module.exports = function (context, mensagensIoTHub) {
  context.log(`Function do IoTHub chamada para tratar ${mensagensIoTHub.length} mensagens`)

  const operacoesBD = new OperacoesBD();
  const registroMedicao = new RegistroMedicao(operacoesBD);
  const promises = [];

  mensagensIoTHub.forEach(stringDaMensagem => {
    const mensagemIOT = JSON.parse(stringDaMensagem);
    context.log(`Mensagem recebida: ${mensagemIOT}`);

    if (mensagemIOT.comando == 'MEDICAO')
      promises.push(registroMedicao.handle(mensagemIOT))

    promises.push(operacoesBD.registraInteracaoDaPlaca(mensagemIOT.idPlaca, mensagemIOT.timestamp))
  });

  Promise.all(promises)
    .then(() => {
      operacoesBD.desconecta();
      context.log('Operação Completa');
    })
    .catch(erro => {
      context.error("Erro na operação");
      context.error(erro);
      operacoesBD.desconecta();
    });

  context.done();
};