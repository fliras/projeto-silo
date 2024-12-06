const OperacoesBD = require('./operacoesBD');
let operacoesBD;

const montaRegistroMedicao = ({ dadosDoSilo, volumeAtualDoSilo, idNivelSilo, timestampDaMedicao }) => {
  const idMedicao = crypto.randomUUID();
  return {
    id_medicao_silo: idMedicao,
    id_silo: dadosDoSilo.idSilo,
    volume_em_m3: volumeAtualDoSilo,
    id_nivel_de_silo: idNivelSilo,
    timestamp_medicao: timestampDaMedicao
  }
}

const defineNivelAtual = async (volumeTotal, volumeAtual) => {
  const percentualVolumeAtual = parseFloat((volumeAtual / volumeTotal).toFixed(2));
  const nivelAtual = await operacoesBD.obtemNivelDeSiloPeloPercentualDeVolume(percentualVolumeAtual);
  return nivelAtual.id_nivel_de_silo;
}

const calculaVolumeAtual = (dadosDoSilo, percentualVazioDoSilo) => {
  const { volumeFixo, volumeVariavel } = dadosDoSilo;
  const fatorDoVolumeVariavelAtual = (100 - percentualVazioDoSilo) / 100;
  return volumeFixo + (volumeVariavel * fatorDoVolumeVariavelAtual);
}

const montaKnex = () => {
  operacoesBD = new OperacoesBD();
  operacoesBD.conecta();
}

const processaDadosIOT = async (context, dados) => {
  if (dados.comando == 'PING') {
    await operacoesBD.registraInteracaoDaPlaca(dados.idPlaca, dados.timestamp);
    return;
  }

  const dadosDoSilo = await operacoesBD.obtemDadosDoSiloDaPlaca(dados.idPlaca);
  if (!dadosDoSilo)
    throw new Error(`Silo de id ${dados.idPlaca} não encontrado!`);

  const volumeAtualDoSilo = calculaVolumeAtual(dadosDoSilo, dados.dados.medicao);
  const idNivelSilo = await defineNivelAtual(dadosDoSilo.volumeTotal, volumeAtualDoSilo);
  const medicao = montaRegistroMedicao({
    dadosDoSilo,
    volumeAtualDoSilo,
    idNivelSilo,
    timestampDaMedicao: dados.timestamp
  });

  await operacoesBD.registraMedicao(medicao);
  await operacoesBD.registraInteracaoDaPlaca(dados.idPlaca, dados.timestamp);

  context.log("Operação Completa");
}

module.exports = function (context, IoTHubMessages) {
  context.log(`JavaScript eventhub trigger function called for message array: ${IoTHubMessages}`);

  const promises = [];
  montaKnex();

  IoTHubMessages.forEach(message => {
    const dados = JSON.parse(message);
    context.log(`Received message: ${dados}`);
    promises.push(processaDadosIOT(context, dados));
  });

  Promise.all(promises)
    .then(() => {
      operacoesBD.desconecta();
    })
    .catch(erro => {
      context.error("Erro na operação");
      context.error(erro);
      operacoesBD.desconecta();
    });

  context.done();
};