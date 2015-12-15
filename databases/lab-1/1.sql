SELECT TO_CHAR(sales_order.order_date, 'yyyy/mm') AS mnth, AVG(sales_order.total) AS avg_price
    FROM sales_order, customer
    WHERE sales_order.customer_id = customer.customer_id
        AND customer.state = 'CA'
        AND sales_order.order_date IS NOT NULL
    GROUP BY TO_CHAR(sales_order.order_date, 'yyyy/mm')
    ORDER BY 1;

