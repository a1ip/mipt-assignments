WITH department_stats AS (
    SELECT employee.department_id AS dep_id, (SUM(employee.salary) + SUM(employee.commission)) / COUNT(*) AS avg_income
        FROM employee
        GROUP BY employee.department_id
)
SELECT department_stats.dep_id, department_stats.avg_income
    FROM department_stats
    WHERE department_stats.avg_income = (
        SELECT MIN(department_stats.avg_income)
            FROM department_stats
    );

