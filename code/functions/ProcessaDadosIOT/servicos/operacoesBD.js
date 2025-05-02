class OperacoesBD {
  constructor() {
    this.conecta();
  }

  conecta() {
    this._knex = require('knex')({
      client: 'mysql',
      connection: {
        host: process.env.DB_HOST,
        port: process.env.DB_PORT,
        user: process.env.DB_USER,
        password: process.env.DB_PASSWORD,
        database: process.env.DB_NAME,
      },
    })
  }

  desconecta() {
    this._knex.destroy();
    this._knex = null;
  }

  async registraInteracaoDaPlaca(idPlaca, timestamp) {
    await this._knex('placas_medidoras')
      .update({ timestamp_ultima_interacao: timestamp })
      .where({ id_placa_medidora: idPlaca });
  }

  async obtemIdDoSiloDaPlaca(idPlaca) {
    const silo = await this._knex('silos')
      .select('id_silo')
      .first()
      .where({ id_placa_medidora: idPlaca });
    return silo?.id_silo;
  }

  async obtemIdDoNivelDeSiloPelosMiliamperes(miliamperes) {
    const faixaMiliampere = await this._knex('faixas_miliampere')
      .select('id_nivel_de_silo')
      .first()
      .where('ma_minimo', '<=', miliamperes)
      .andWhere('ma_maximo', '>=', miliamperes);
    return faixaMiliampere?.id_nivel_de_silo;
  }

  async registraMedicao({ idSilo, idNivelSilo, timestampDaMedicao }) {
    const idMedicao = crypto.randomUUID();
    await this._knex('medicoes_silos').insert({
      id_medicao_silo: idMedicao,
      id_silo: idSilo,
      id_nivel_de_silo: idNivelSilo,
      timestamp_medicao: timestampDaMedicao
    });
  }
}

module.exports = OperacoesBD;