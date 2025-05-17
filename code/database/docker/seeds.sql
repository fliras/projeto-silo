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