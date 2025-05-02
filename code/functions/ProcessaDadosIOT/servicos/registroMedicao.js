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

    // método mockado, substituir aqui pela função correta
  // retorna um valor aleatório entre 4mA e 20mA
  calculaMiliAmperesDaMedicao(medicao) {
    return 4 + (Math.random() * 16)
  }

  montaErroNivelDeSiloNaoEncontrado(medicao, miliamperes) {
    return new Error('Não foi possível classificar o nível de silo atual!\n'
      + `valor do sensor: ${medicao}\n`
      + `miliamperes: ${miliamperes}`);
  }
}

module.exports = RegistroMedicao