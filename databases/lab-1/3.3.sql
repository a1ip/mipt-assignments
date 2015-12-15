WITH department_stats AS (
    SELECT employee.department_id AS dep_id, AVG(
            NVL2(employee.salary, employee.salary, 0)
            +
            NVL2(employee.commission, employee.commission, 0)
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
