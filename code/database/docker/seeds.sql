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

INSERT INTO ingredientes (nome)
VALUES
  ('Milho'),
  ('Farelo de soja'),
  ('Ureia pecuária'),
  ('Núcleo mineral engorda'),
  ('Farelo de soja (saco 40kg)'),
  ('Núcleo mineral engorda (saco 30kg)'),
  ('Núcleo mineral pra bezerros (saco 30kg)'),
  ('Calcário fino'),
  ('Calcário grosso'),
  ('Núcleo mineral de postura'),
  ('Núcleo postura'),
  ('Núcleo mineral de leite');

INSERT INTO ingredientes_receitas (id_ingrediente, fator_de_proporcao, id_receita)
VALUES
  (1, 0.8500, 1),
  (2, 0.0928, 1),
  (3, 0.0172, 1),
  (4, 0.0400, 1),

  (1, 0.5573, 2),
  (5, 0.4327, 2),
  (6, 0.0100, 2),

  (1, 0.5540, 3),
  (5, 0.3960, 3),
  (7, 0.0500, 3),

  (1, 0.6300, 4),
  (2, 0.2240, 4),
  (8, 0.0300, 4),
  (9, 0.0910, 4),
  (10, 0.0250, 4),

  (1, 0.6580, 5),
  (2, 0.2080, 5),
  (8, 0.0340, 5),
  (9, 0.0750, 5),
  (11, 0.0250, 5),

  (1, 0.7400, 6),
  (2, 0.1700, 6),
  (3, 0.0200, 6),
  (4, 0.0700, 6),

  (1, 0.6100, 7),
  (2, 0.3500, 7),
  (3, 0.0100, 7),
  (12, 0.0300, 7);
