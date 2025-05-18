USE silodb;

insert into fazendas values (default, 'fazenda', 'endereco');

insert into placas_medidoras values
    ('esp32-etasa-0001', 'placa 01', current_timestamp),
    ('esp32-etasa-0002', 'placa 02', current_timestamp),
    ('esp32-etasa-0003', 'placa 03', current_timestamp);

insert into silos values
	(default, 'silo01', 55, 1.24, 1, 'esp32-etasa-0001'),
    (default, 'silo02', 55, 1.24, 1, 'esp32-etasa-0002'),
    (default, 'silo03', 55, 1.24, 1, 'esp32-etasa-0003');
    
insert into niveis_de_silo values
    (default, 'N0', 0, 0.24),
    (default, 'N1', 0.25, 0.49),
    (default, 'N2', 0.5, 0.74),
    (default, 'N3', 0.75, 1);
    
insert into faixas_miliampere values
    (default, 16.1, 20, 1),
    (default, 12.1, 16, 2),
    (default, 8.1, 12, 3),
    (default, 4, 8, 4);

INSERT INTO receitas (nome)
VALUES 
  ('Ração de Confinamento'),
  ('Ração de Leiteiras'),
  ('Ração de Bezerros'),
  ('Ração de Postura'),
  ('Ração de Postura - P3 (75 semanas)'),
  ('Ração de Pasto'),
  ('Ração Leiteiras 2');

INSERT INTO ingredientes (nome, fator_de_proporcao, id_receita)
VALUES
  ('Milho', 0.8500, 1),
  ('Farelo de soja', 0.0928, 1),
  ('Ureia pecuária', 0.0172, 1),
  ('Núcleo mineral engorda', 0.0400, 1),

  ('Milho', 0.5573, 2),
  ('Farelo de soja (saco 40kg)', 0.4327, 2),
  ('Núcleo mineral engorda (saco 30kg)', 0.0100, 2),

  ('Milho', 0.5540, 3),
  ('Farelo de soja (saco 40kg)', 0.3960, 3),
  ('Núcleo mineral pra bezerros (saco 30kg)', 0.0500, 3),

  ('Milho', 0.6300, 4),
  ('Farelo de soja', 0.2240, 4),
  ('Calcário fino', 0.0300, 4),
  ('Calcário grosso', 0.0910, 4),
  ('Núcleo mineral de postura', 0.0250, 4),

  ('Milho', 0.6580, 5),
  ('Farelo de soja', 0.2080, 5),
  ('Calcário fino', 0.0340, 5),
  ('Calcário grosso', 0.0750, 5),
  ('Núcleo postura', 0.0250, 5),

  ('Milho', 0.7400, 6),
  ('Farelo de soja', 0.1700, 6),
  ('Ureia pecuária', 0.0200, 6),
  ('Núcleo mineral engorda', 0.0700, 6),

  ('Milho', 0.6100, 7),
  ('Farelo de soja', 0.3500, 7),
  ('Ureia pecuária', 0.0100, 7),
  ('Núcleo mineral de leite', 0.0300, 7);
