SELECT AVG(manager_avg_sal.avg_sal) AS avg_salary
    FROM (
        SELECT employee.employee_id AS id, employee.salary / manager_subs.subs_cnt AS avg_sal
            FROM employee, (
                SELECT employee.manager_id AS id, COUNT(*) AS subs_cnt
                    FROM employee
                    WHERE employee.manager_id IS NOT NULL
                    GROUP BY employee.manager_id
            ) manager_subs
            WHERE employee.employee_id = manager_subs.id
                AND employee.salary IS NOT NULL
    ) manager_avg_sal;

/*
SELECT employee.manager_id AS id, COUNT(*) AS subs_cnt
    FROM employee
    WHERE employee.manager_id IS NOT NULL
    GROUP BY employee.manager_id;
*/

/*
SELECT employee.employee_id AS id, employee.salary / manager_subs.subs_cnt AS avg_sal
    FROM employee, (
        SELECT employee.manager_id AS id, COUNT(*) AS subs_cnt
            FROM employee
            WHERE employee.manager_id IS NOT NULL
            GROUP BY employee.manager_id
    ) manager_subs
    WHERE employee.employee_id = manager_subs.id
        AND employee.salary IS NOT NULL;
*/

