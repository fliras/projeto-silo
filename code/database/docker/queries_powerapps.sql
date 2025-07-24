/* Queries que podem ser úteis na consulta de medições. */

/* obtém a última medição de um silo de id X */
select * from vw_medicoes
	where id_silo = X
    order by timestamp_medicao desc
    limit 1;
    
/* obtém as medições de um silo de id X nos últimos Y dias */
select * from vw_medicoes
    where id_silo = X AND timestamp_medicao >= DATE_SUB(CURRENT_DATE, INTERVAL Y DAY)
    ORDER BY timestamp_medicao;