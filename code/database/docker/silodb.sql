USE silodb;

CREATE TABLE receitas
(
  id_receita INTEGER NOT NULL AUTO_INCREMENT,
  nome VARCHAR(100) NOT NULL,
  PRIMARY KEY(id_receita)
);

CREATE TABLE ingredientes
(
  id_ingrediente INTEGER NOT NULL AUTO_INCREMENT,
  nome VARCHAR(100) NOT NULL,
  fator_de_proporcao DECIMAL(4,3) NOT NULL,
  id_receita INTEGER NOT NULL,
  FOREIGN KEY(id_receita) REFERENCES receitas(id_receita),
  PRIMARY KEY(id_ingrediente)
);

CREATE TABLE acessos
(
  id_acesso INTEGER NOT NULL AUTO_INCREMENT,
  nome VARCHAR(100) NOT NULL,
  PRIMARY KEY(id_acesso)
);

CREATE TABLE usuarios
(
  id_usuario INTEGER NOT NULL AUTO_INCREMENT,
  nome VARCHAR(100) NOT NULL,
  email VARCHAR(100) NOT NULL,
  ativo BOOL NOT NULL DEFAULT 1,
  PRIMARY KEY(id_usuario)
);

CREATE TABLE usuarios_acessos
(
  id_usuario INTEGER NOT NULL,
  id_acesso INTEGER NOT NULL,
  FOREIGN KEY(id_usuario) REFERENCES usuarios(id_usuario),
  FOREIGN KEY(id_acesso) REFERENCES acessos(id_acesso),
  PRIMARY KEY(id_usuario, id_acesso)
);

CREATE TABLE placas_medidoras
(
  id_placa_medidora VARCHAR(20) NOT NULL,
  nome VARCHAR(100) NOT NULL,
  ip VARCHAR(15) NULL,
  timestamp_ultima_interacao TIMESTAMP NULL,
  PRIMARY KEY(id_placa_medidora)
);

CREATE TABLE fazendas
(
  id_fazenda INTEGER NOT NULL AUTO_INCREMENT,
  nome VARCHAR(100) NOT NULL,
  endereco VARCHAR(100) NOT NULL,
  PRIMARY KEY(id_fazenda)
);

CREATE TABLE fazendas_usuarios
(
  id_usuario INTEGER NOT NULL,
  id_fazenda INTEGER NOT NULL,
  FOREIGN KEY(id_usuario) REFERENCES usuarios(id_usuario),
  FOREIGN KEY(id_fazenda) REFERENCES fazendas(id_fazenda),
  PRIMARY KEY(id_usuario, id_fazenda)
);

CREATE TABLE silos
(
  id_silo INTEGER NOT NULL AUTO_INCREMENT,
  id_fazenda INTEGER NOT NULL,
  id_placa_medidora VARCHAR(20) NOT NULL,
  nome VARCHAR(100) NOT NULL,
  volume_fixo_em_m3 DECIMAL(5,2) NOT NULL,
  volume_variavel_em_m3 DECIMAL(5,2) NOT NULL,
  FOREIGN KEY(id_fazenda) REFERENCES fazendas(id_fazenda),
  FOREIGN KEY(id_placa_medidora) REFERENCES placas_medidoras(id_placa_medidora),
  PRIMARY KEY(id_silo)
);

CREATE TABLE niveis_de_silo
(
  id_nivel_de_silo INTEGER NOT NULL AUTO_INCREMENT,
  percentual_minimo DECIMAL(3,2) NOT NULL,
  percentual_maximo DECIMAL(3,2) NOT NULL,
  nivel VARCHAR(3) NOT NULL UNIQUE,
  PRIMARY KEY(id_nivel_de_silo)
);

CREATE TABLE medicoes_silos
(
  id_medicao_silo CHAR(36) NOT NULL,
  id_silo INTEGER NOT NULL,
  id_nivel_de_silo INTEGER NOT NULL,
  volume_em_m3 DECIMAL(5,2) NOT NULL,
  timestamp_medicao TIMESTAMP,
  FOREIGN KEY(id_silo) REFERENCES silos(id_silo),
  FOREIGN KEY(id_nivel_de_silo) REFERENCES niveis_de_silo(id_nivel_de_silo),
  PRIMARY KEY(id_medicao_silo)
);