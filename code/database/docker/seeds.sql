USE silodb;

insert into fazendas values (default, 'fazenda', 'endereco');

insert into placas_medidoras values
	('esp32-etasa-0001', 'placa 01', current_timestamp),
  ('esp32-etasa-0002', 'placa 02', current_timestamp),
  ('esp32-etasa-0003', 'placa 03', current_timestamp);

insert into silos values
	(default, 1, 'esp32-etasa-0001', 'silo01', 12, 43.0, 1.24),
  (default, 1, 'esp32-etasa-0002', 'silo01', 12, 43.0, 1.24),
  (default, 1, 'esp32-etasa-0003', 'silo01', 12, 43.0, 1.24);

insert into niveis_de_silo
VALUES
	(default, 0, 0.19, 'N0'),
  (default, 0.20, 0.34, 'N1'),
  (default, 0.35, 0.49, 'N2'),
  (default, 0.5, 0.64, 'N3'),
  (default, 0.65, 0.79, 'N4'),
  (default, 0.8, 1, 'N5');