class RegistroMedicao {
  constructor(operacoesBD) {
    this._bd = operacoesBD;
  }

  async handle(mensagemIOT) {
    const dadosDoSilo = await this._bd.obtemDadosDoSiloDaPlaca(mensagemIOT.idPlaca);
    if (!dadosDoSilo)
      throw new Error(`Silo de id ${mensagemIOT.idPlaca} não encontrado!`);

    const volumeAtualDoSilo = this.calculaVolumeAtualDoSilo(dadosDoSilo, mensagemIOT.dados.medicao);
    const pesoAtualDoSilo = this.calculaPesoAtualDoSilo(dadosDoSilo, volumeAtualDoSilo);
    const idNivelSilo = await this.defineNivelAtualDoSilo(dadosDoSilo.volumeTotal, volumeAtualDoSilo);
    
    await this._bd.registraMedicao({
      dadosDoSilo,
      volumeAtualDoSilo,
      pesoAtualDoSilo,
      idNivelSilo,
      timestampDaMedicao: mensagemIOT.timestamp
    })
  }

  calculaVolumeAtualDoSilo(dadosDoSilo, percentualVazioDoSilo) {
    const { volumeFixo, volumeVariavel } = dadosDoSilo;
    const fatorDoVolumeVariavelAtual = (100 - percentualVazioDoSilo) / 100;
    return volumeFixo + (volumeVariavel * fatorDoVolumeVariavelAtual);
  }

  calculaPesoAtualDoSilo(dadosDoSilo, volumeAtualDoSilo) {
    const { densidade } = dadosDoSilo;
    return densidade * volumeAtualDoSilo;
  }

  async defineNivelAtualDoSilo(volumeTotal, volumeAtual) {
    const percentualVolumeAtual = parseFloat((volumeAtual / volumeTotal).toFixed(2));
    const nivelAtual = await this._bd.obtemNivelDeSiloPeloPercentualDeVolume(percentualVolumeAtual);
    return nivelAtual.id_nivel_de_silo;
  }
}

module.exports = RegistroMedicao