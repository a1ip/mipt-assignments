SELECT product.description, price.list_price
    FROM product, price, (
        SELECT MIN(sales_order.order_date) AS the_date
            FROM sales_order, customer
            WHERE customer.customer_id = sales_order.customer_id
                AND customer.name = 'WOMENS SPORTS'
    ) temp
    WHERE product.product_id = price.product_id
        AND price.start_date <= temp.the_date
        AND (
            price.end_date > temp.the_date OR
            price.end_date IS NULL
        );

/*
SELECT MIN(sales_order.order_date) AS the_date
    FROM sales_order, customer
    WHERE customer.customer_id = sales_order.customer_id
        AND customer.name = 'WOMENS SPORTS';
*/
