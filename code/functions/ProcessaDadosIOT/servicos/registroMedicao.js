class RegistroMedicao {
  constructor(operacoesBD) {
    this._bd = operacoesBD;
  }

  async handle(mensagemIOT) {
    const dadosDoSilo = await this._bd.obtemDadosDoSiloDaPlaca(mensagemIOT.idPlaca);
    if (!dadosDoSilo)
      throw new Error(`Silo de id ${mensagemIOT.idPlaca} não encontrado!`);

    const volumeAtualDoSilo = this.calculaVolumeAtualDoSilo(dadosDoSilo, mensagemIOT.dados.medicao);
    const idNivelSilo = await this.defineNivelAtualDoSilo(dadosDoSilo.volumeTotal, volumeAtualDoSilo);
    const medicao = this.montaRegistroMedicao({
      dadosDoSilo,
      volumeAtualDoSilo,
      idNivelSilo,
      timestampDaMedicao: mensagemIOT.timestamp
    })

    await this._bd.registraMedicao(medicao);
  }

  calculaVolumeAtualDoSilo(dadosDoSilo, percentualVazioDoSilo) {
    const { volumeFixo, volumeVariavel } = dadosDoSilo;
    const fatorDoVolumeVariavelAtual = (100 - percentualVazioDoSilo) / 100;
    return volumeFixo + (volumeVariavel * fatorDoVolumeVariavelAtual);
  }

  async defineNivelAtualDoSilo(volumeTotal, volumeAtual) {
    const percentualVolumeAtual = parseFloat((volumeAtual / volumeTotal).toFixed(2));
    const nivelAtual = await this._bd.obtemNivelDeSiloPeloPercentualDeVolume(percentualVolumeAtual);
    return nivelAtual.id_nivel_de_silo;
  }

  montaRegistroMedicao({ dadosDoSilo, volumeAtualDoSilo, idNivelSilo, timestampDaMedicao }) {
    const idMedicao = crypto.randomUUID();
    return {
      id_medicao_silo: idMedicao,
      id_silo: dadosDoSilo.idSilo,
      volume_em_m3: volumeAtualDoSilo,
      id_nivel_de_silo: idNivelSilo,
      timestamp_medicao: timestampDaMedicao
    }
  }
}

module.exports = RegistroMedicao