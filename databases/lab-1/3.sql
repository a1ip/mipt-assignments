WITH department_stats AS (
    SELECT dep_income_mixed.dep_id as dep_id, SUM(dep_income_mixed.income) / SUM(dep_income_mixed.cnt) AS avg_income
        FROM (
            (SELECT employee.department_id AS dep_id, SUM(employee.salary + employee.commission) AS income, COUNT(*) AS cnt
                FROM employee
                WHERE employee.salary IS NOT NULL
                    AND employee.commission IS NOT NULL
                GROUP BY employee.department_id)
            UNION
            (SELECT employee.department_id AS dep_id, SUM(employee.salary) AS income, COUNT(*) AS cnt
                FROM employee
                WHERE employee.salary IS NOT NULL
                    AND employee.commission IS NULL
                GROUP BY employee.department_id)
            UNION
            (SELECT employee.department_id AS dep_id, SUM(employee.commission) AS income, COUNT(*) AS cnt
                FROM employee
                WHERE employee.salary IS NULL
                    AND employee.commission IS NOT NULL
                GROUP BY employee.department_id)
        ) dep_income_mixed
        GROUP BY dep_id
)
SELECT department_stats.dep_id, department_stats.avg_income
    FROM department_stats
    WHERE department_stats.avg_income = (
        SELECT MIN(department_stats.avg_income)
            FROM department_stats
    );

/*
SELECT employee.department_id AS dep_id, SUM(employee.salary + employee.commission) AS income, COUNT(*) AS cnt
    FROM employee
    WHERE employee.salary IS NOT NULL
        AND employee.commission IS NOT NULL
    GROUP BY employee.department_id;
*/

/*
SELECT dep_income_mixed.dep_id as dep_id, SUM(dep_income_mixed.income) / SUM(dep_income_mixed.cnt) AS avg_income
    FROM (
        SELECT employee.department_id AS dep_id, SUM(employee.salary + employee.commission) AS income, COUNT(*) AS cnt
            FROM employee
            WHERE employee.salary IS NOT NULL
                AND employee.commission IS NOT NULL
            GROUP BY employee.department_id
    ) dep_income_mixed
    GROUP BY dep_id;
*/

