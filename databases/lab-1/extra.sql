/*
название города
количество аналитиков в каждом городе (ANALYST)
*/
/*
SELECT location.regional_group AS name, COUNT(*) AS cnt
    FROM employee, job, department, location
    WHERE employee.job_id = job.job_id
        AND employee.department_id = department.department_id
        AND department.location_id = location.location_id
        AND job.function = 'ANALYST'
    GROUP BY location.regional_group;
*/
/*
SELECT department.location_id AS l_id
    FROM employee, job, department, location
    WHERE employee.job_id = job.job_id
        AND employee.department_id = department.department_id
        AND department.location_id = location.location_id
        AND job.function = 'ANALYST';
*/

WITH
    analyst_location AS (
        SELECT department.location_id AS l_id
            FROM employee, job, department, location
            WHERE employee.job_id = job.job_id
                AND employee.department_id = department.department_id
                AND department.location_id = location.location_id
                AND job.function = 'ANALYST'
    )
    SELECT location.location_id AS city, COUNT(*)
        FROM analyst_location, location
        WHERE analyst_location.l_id = location.location_id
        GROUP BY location.location_id;
     
/*
SELECT location.location_id AS id, COUNT(*)
    FROM location, 
    WHERE location.location_id = analyst_location.l_id;
*/

/*
WITH city_analyst AS (
    SELECT location.regional_group AS name, COUNT(*) AS cnt
        FROM employee, job, department, location
        WHERE employee.job_id = job.job_id
            AND employee.department_id = department.department_id
            AND department.location_id = location.location_id
            AND job.function = 'ANALYST'
        GROUP BY location.regional_group
)
(SELECT city_analyst.name, city_analyst.cnt
    FROM city_analyst)
UNION
(SELECT location.regional_group AS city, 0
    FROM location
    WHERE location.regional_group NOT IN (
        SELECT city_analyst.name
            FROM city_analyst
    ));
*/

