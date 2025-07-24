// Classe responsável por abstrair o código utilizado em operações no banco de dados, utilizando uma
// biblioteca de código externo. Atualmente, utiliza-se a biblioteca Knex. Para mais informações, consultar:
// https://knexjs.org/guide/

class OperacoesBD {

  // a conexão com o BD é aberta assim que a instância é criada.
  constructor() {
    this.conecta();
  }


  // método que abre a conexão com o banco de dados.
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

  // método que faz a desconexão do banco de dados.
  desconecta() {
    this._knex.destroy();
    this._knex = null;
  }

  // registra a interação do Esp32, atualizando a data de sua última interação.
  async registraInteracaoDaPlaca(idPlaca, timestamp) {
    await this._knex('placas_medidoras')
      .update({ timestamp_ultima_interacao: timestamp })
      .where({ id_placa_medidora: idPlaca });
  }


  // obtém o id do silo com base no id do esp32 recebido na mensagem.
  async obtemIdDoSiloDaPlaca(idPlaca) {
    const silo = await this._knex('silos')
      .select('id_silo')
      .first()
      .where({ id_placa_medidora: idPlaca });
    return silo?.id_silo;
  }


  // identifica o nível atual do silo com base no valor de miliamperes calculado.
  async obtemIdDoNivelDeSiloPelosMiliamperes(miliamperes) {
    const faixaMiliampere = await this._knex('faixas_miliampere')
      .select('id_nivel_de_silo')
      .first()
      .where('ma_minimo', '<=', miliamperes)
      .andWhere('ma_maximo', '>=', miliamperes);
    return faixaMiliampere?.id_nivel_de_silo;
  }


  // registra o nível atual do respectivo silo.
  async registraMedicao({ idSilo, idNivelSilo, timestampDaMedicao }) {
    const idMedicao = crypto.randomUUID();
    await this._knex('medicoes_silos').insert({
      id_medicao_silo: idMedicao,
      id_silo: idSilo,
      id_nivel_de_silo: idNivelSilo,
      timestamp_medicao: timestampDaMedicao
    });
  }


  // obtém os valores mínimo e máximo de miliamperes que definem os níveis
  // de silo, conforme cadastrado no banco de dados.
  async obtemLimitesDeMiliampere() {
    return await this._knex('faixas_miliampere')
      .select(
        { maMinimo: this._knex.min('ma_minimo') },
        { maMaximo: this._knex.max('ma_maximo') }
      )
      .first();
  }
}

module.exports = OperacoesBD;