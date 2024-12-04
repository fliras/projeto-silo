let knex;

const registraMedicaoNoBD = async (medicao) => {
  await knex('medicoes_silos').insert(medicao);
}

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
  const nivelAtual = await knex('niveis_de_silo')
    .select('id_nivel_de_silo')
    .first()
    .where('percentual_minimo', '<=', percentualVolumeAtual)
    .andWhere('percentual_maximo', '>=', percentualVolumeAtual);
  return nivelAtual.id_nivel_de_silo;
}

const calculaVolumeAtual = (dadosDoSilo, percentualVazioDoSilo) => {
  const { volumeFixo, volumeVariavel } = dadosDoSilo;
  const fatorDoVolumeVariavelAtual = (100 - percentualVazioDoSilo) / 100;
  return volumeFixo + (volumeVariavel * fatorDoVolumeVariavelAtual);
}

const obtemDadosDoSiloDaPlaca = async (idPlaca) => {
  return await knex('silos')
    .select(
      'id_silo as idSilo',
      'nome',
      'volume_fixo_em_m3 as volumeFixo',
      'volume_variavel_em_m3 as volumeVariavel',
      knex.raw('(volume_fixo_em_m3 + volume_variavel_em_m3) as volumeTotal'),
      'id_fazenda as idFazenda',
      'id_placa_medidora as idPlacaMedidora'
    )
    .first()
    .where({ id_placa_medidora: idPlaca });
}

const registraInteracaoDaPlaca = async (idPlaca, timestamp) => {
  await knex('placas_medidoras')
    .update({ timestamp_ultima_interacao: timestamp })
    .where({ id_placa_medidora: idPlaca });
}

const montaKnex = () => {
  knex = require('knex')({
    client: 'mysql',
    connection:{
      host: process.env.DB_HOST,
      port: process.env.DB_PORT,
      user: process.env.DB_USER,
      password: process.env.DB_PASSWORD,
      database: process.env.DB_NAME,
  },
});
}

const processaDadosIOT = async (context, dados) => {
    if (dados.comando == 'PING') {
      await registraInteracaoDaPlaca(dados.idPlaca, dados.timestamp);
      return;
    }

    const dadosDoSilo = await obtemDadosDoSiloDaPlaca(dados.idPlaca);
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

    await registraMedicaoNoBD(medicao);
    await registraInteracaoDaPlaca(dados.idPlaca, dados.timestamp);

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
    knex.destroy();
  })
  .catch(erro => {
    context.error("Erro na operação");
    context.error(erro);
    knex.destroy();
  });

  context.done();
};