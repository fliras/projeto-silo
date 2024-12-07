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

  async obtemDadosDoSiloDaPlaca(idPlaca) {
    return await this._knex('silos')
      .select(
        'id_silo as idSilo',
        'nome',
        'volume_fixo_em_m3 as volumeFixo',
        'volume_variavel_em_m3 as volumeVariavel',
        'densidade_armazenada_em_t_por_m3 as densidade',
        this._knex.raw('(volume_fixo_em_m3 + volume_variavel_em_m3) as volumeTotal'),
        'id_fazenda as idFazenda',
        'id_placa_medidora as idPlacaMedidora'
      )
      .first()
      .where({ id_placa_medidora: idPlaca });
  }

  async obtemNivelDeSiloPeloPercentualDeVolume(percentual) {
    return await this._knex('niveis_de_silo')
      .select('id_nivel_de_silo')
      .first()
      .where('percentual_minimo', '<=', percentual)
      .andWhere('percentual_maximo', '>=', percentual);
  }

  async registraMedicao(medicao) {
    await this._knex('medicoes_silos').insert(medicao);
  }
}

module.exports = OperacoesBD;