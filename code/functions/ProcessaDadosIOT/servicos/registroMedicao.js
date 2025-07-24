
// Classe responsável por registrar no banco de dados as medições enviadas pelos Esp32.
class RegistroMedicao {

  // construtor que recebe a conexão com o banco e o context utilizado pela function no momento da requisição.
  constructor(operacoesBD, context) {
    this._bd = operacoesBD;
    this._context = context;
  }

  // Método principal do RegistroMedicao.
  // obtém o silo associado ao Esp32, identifica o nível de silo atual por meio da medição do sensor e
  // registra essa informação no banco de dados.
  async handle(mensagemIOT) {
    const idSilo = await this._bd.obtemIdDoSiloDaPlaca(mensagemIOT.idPlaca);
    if (!idSilo)
      throw this.montaErroPlacaNaoEncontrada(mensagemIOT.idPlaca);

    const miliAmperesDaMedicao = await this.calculaMiliAmperesDaMedicao(mensagemIOT.dados.medicao);
    const idNivelSilo = await this._bd.obtemIdDoNivelDeSiloPelosMiliamperes(miliAmperesDaMedicao);

    if (!idNivelSilo)
      throw this.montaErroNivelDeSiloNaoEncontrado(mensagemIOT.dados.medicao, miliAmperesDaMedicao);

    this._context.log(`mA: ${miliAmperesDaMedicao}`);
    this._context.log(`id do Nivel: ${idNivelSilo}`);
    await this._bd.registraMedicao({
      idSilo,
      idNivelSilo,
      timestampDaMedicao: mensagemIOT.timestamp
    })
  }


  // método que monta uma exception lançada caso não se encontre o Esp32 respectivo à mensagem enviada.
  montaErroPlacaNaoEncontrada(idPlaca) {
    return new Error(`Placa de id ${idPlaca} não encontrada!`);
  }

  // Método que calcula o valor em mA, que servirá para identificar o nível atual do silo, utilizando
  // um polinômio de grau 3. A entrada é o valor coletado pelo sensor.
  async calculaMiliAmperesDaMedicao(medicao) {

    // para se estipular os níveis de silo, trabalha-se com uma faixa de mA indo de "maMinimo" a "maMaximo".
    const {maMinimo, maMaximo} = await this._bd.obtemLimitesDeMiliampere();

    // Os coeficientes foram obtidos por meio de testes de bancada com o sensor.
    const coeficienteA = 1.853 * Math.pow(10, -9);
    const coeficienteB = -1.1 * Math.pow(10, -5);
    const coeficienteC = 0.02413;
    const coeficienteD = -1.847;

    // Y(x) = aX³ + bX² + cX + d
    let miliamperes = (coeficienteA * Math.pow(medicao, 3))
      + (coeficienteB * Math.pow(medicao, 2))
      + (coeficienteC * medicao)
      + coeficienteD;

    // é necessário adequar o valor de mA calculado, caso ele esteja fora dos limites mínimo ou máximo.
    if (miliamperes < maMinimo)
      miliamperes = maMinimo;
    else if (miliamperes > maMaximo)
      miliamperes = maMaximo;

    // necessário arredondar o valor para apenas uma casa decimal
    return parseFloat(miliamperes.toFixed(1));
  }


  // monta uma exception lançada caso não se identifique o nível de silo (apenas contingência, teoricamente não acontecerá)
  montaErroNivelDeSiloNaoEncontrado(medicao, miliamperes) {
    return new Error('Não foi possível classificar o nível de silo atual!\n'
      + `valor do sensor: ${medicao}\n`
      + `miliamperes: ${miliamperes}`);
  }
}

// exporta a classe
module.exports = RegistroMedicao