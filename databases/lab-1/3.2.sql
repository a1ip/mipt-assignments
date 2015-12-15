WITH department_stats AS (
    SELECT employee.department_id AS dep_id, AVG (
        (CASE
            WHEN employee.salary IS NULL THEN 0
            ELSE employee.salary
        END)
        +
        (CASE
            WHEN employee.commission IS NULL THEN 0
            ELSE employee.commission
        END)
    ) AS avg_income
        FROM employee
        GROUP BY employee.department_id
)
SELECT department_stats.dep_id, department_stats.avg_income
    FROM department_stats
    WHERE department_stats.avg_income = (
        SELECT MIN(department_stats.avg_income)
            FROM department_stats
    );

