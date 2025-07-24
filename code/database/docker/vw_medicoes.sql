/* View que exibe de maneira formatada os registros de medição dos silos */

CREATE VIEW `vw_medicoes` AS
select `s`.`id_silo`,
       `ms`.`timestamp_medicao`,
       `ns`.`nivel`,
       (`ns`.`percentual_minimo` * 100) AS `percentual_minimo`,
       (`ns`.`percentual_maximo` * 100) AS `percentual_maximo`,
       (`s`.`volume_total` * `ns`.`percentual_minimo`) AS `volume_minimo`,
       (`s`.`volume_total` * `ns`.`percentual_maximo`) AS `volume_maximo`,
       (`s`.`densidade_armazenada` * `s`.`volume_total` * `ns`.`percentual_minimo`) AS `peso_minimo`,
       (`s`.`densidade_armazenada` * `s`.`volume_total` * `ns`.`percentual_maximo`) AS `peso_maximo` 
       from `silodb`.`medicoes_silos` `ms` 
       join `silodb`.`niveis_de_silo` `ns` on `ns`.`id_nivel_de_silo` = `ms`.`id_nivel_de_silo`
       join `silodb`.`silos` `s` on `ms`.`id_silo` = `s`.`id_silo`