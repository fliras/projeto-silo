class RegistroMedicao {
  constructor(operacoesBD) {
    this._bd = operacoesBD;
  }

  async handle(mensagemIOT) {
    const idSilo = await this._bd.obtemIdDoSiloDaPlaca(mensagemIOT.idPlaca);
    if (!idSilo)
      throw this.montaErroPlacaNaoEncontrada(mensagemIOT.idPlaca);

    const miliAmperesDaMedicao = this.calculaMiliAmperesDaMedicao(mensagemIOT.dados.medicao);
    const idNivelSilo = await this._bd.obtemIdDoNivelDeSiloPelosMiliamperes(miliAmperesDaMedicao);

    if (!idNivelSilo)
      throw this.montaErroNivelDeSiloNaoEncontrado(mensagemIOT.dado.medicao, miliAmperesDaMedicao);

    console.log(`mA: ${miliAmperesDaMedicao}\nid do Nivel: ${idNivelSilo}\n\n`);
    await this._bd.registraMedicao({
      idSilo,
      idNivelSilo,
      timestampDaMedicao: mensagemIOT.timestamp
    })
  }

  montaErroPlacaNaoEncontrada(idPlaca) {
    return new Error(`Placa de id ${idPlaca} não encontrada!`);
  }

  // Método que calcula o valor em mA com base na leitura do sensor em uma função de grau 2.
  // Os coeficientes foram obtidos por meio de testes
  calculaMiliAmperesDaMedicao(medicao) {
    const coeficienteA = -2.91501563 * Math.pow(10, -6);
    const coeficienteB = 1.44238234 * Math.pow(10, -2);
    const coeficienteC = 1.24496813;
    return (coeficienteA * Math.pow(medicao, 2)) + (coeficienteB * medicao) + coeficienteC;
  }

  montaErroNivelDeSiloNaoEncontrado(medicao, miliamperes) {
    return new Error('Não foi possível classificar o nível de silo atual!\n'
      + `valor do sensor: ${medicao}\n`
      + `miliamperes: ${miliamperes}`);
  }
}

module.exports = RegistroMedicao