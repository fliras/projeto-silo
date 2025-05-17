class RegistroMedicao {
  constructor(operacoesBD, context) {
    this._bd = operacoesBD;
    this._context = context;
  }

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

  montaErroPlacaNaoEncontrada(idPlaca) {
    return new Error(`Placa de id ${idPlaca} não encontrada!`);
  }

  // Método que calcula o valor em mA utilizando uma polinômio de grau 3, com base na leitura do sensor.
  // Os coeficientes foram obtidos por meio de testes
  async calculaMiliAmperesDaMedicao(medicao) {
    const {maMinimo, maMaximo} = await this._bd.obtemLimitesDeMiliampere();

    const coeficienteA = 1.853 * Math.pow(10, -9);
    const coeficienteB = -1.1 * Math.pow(10, -5);
    const coeficienteC = 0.02413;
    const coeficienteD = -1.847;

    let miliamperes = (coeficienteA * Math.pow(medicao, 3))
      + (coeficienteB * Math.pow(medicao, 2))
      + (coeficienteC * medicao)
      + coeficienteD;

    if (miliamperes < maMinimo)
      miliamperes = maMinimo;
    else if (miliamperes > maMaximo)
      miliamperes = maMaximo;

    // necessário arredondar para apenas uma casa decimal
    return parseFloat(miliamperes.toFixed(1));
  }

  montaErroNivelDeSiloNaoEncontrado(medicao, miliamperes) {
    return new Error('Não foi possível classificar o nível de silo atual!\n'
      + `valor do sensor: ${medicao}\n`
      + `miliamperes: ${miliamperes}`);
  }
}

module.exports = RegistroMedicao