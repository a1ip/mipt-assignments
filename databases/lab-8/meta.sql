COLUMN table_name FORMAT A20;
COLUMN column_name FORMAT A20;
COLUMN data_type FORMAT A20;
COLUMN constraint_name FORMAT A20;
COLUMN trigger_name FORMAT A20;
SET LONG 2048;

/*
Выбрать имена всех таблиц, созданных назначенным пользователем базы данных.
*/
SELECT all_tables.table_name
  FROM all_tables
  WHERE all_tables.owner = 'CHESS';

/*
Выбрать имя таблицы, имя столбца таблицы, признак того, допускает ли данный столбец null-значения, название типа данных столбца таблицы, размер этого типа данных - для всех таблиц, созданных назначенным пользователем базы данных и всех их столбцов.
*/
SELECT all_tab_columns.table_name,
       all_tab_columns.column_name,
       all_tab_columns.data_type,
       all_tab_columns.data_length,
       all_tab_columns.nullable
  FROM all_tab_columns, all_tables
  WHERE all_tab_columns.table_name = all_tables.table_name
    AND all_tables.owner = 'CHESS'
  ORDER BY 1;

/*
Выбрать название ограничения целостности (первичные и внешние ключи), имя таблицы, в которой оно находится, тип ограничения ('P' для первичного ключа и 'R' для внешнего) - для всех ограничений целостности, созданных назначенным пользователем базы данных.
*/
SELECT all_constraints.constraint_name,
       all_constraints.table_name,
       all_constraints.constraint_type
  FROM all_constraints, all_tables
  WHERE all_constraints.table_name = all_tables.table_name
    AND all_tables.owner = 'CHESS'
    AND (
      all_constraints.constraint_type = 'P'
      OR
      all_constraints.constraint_type = 'R'
    );

/*
Выбрать название внешнего ключа, имя таблицы, содержащей внешний ключ, имя таблицы, содержащей его родительский ключ - для всех внешних ключей, созданных назначенным пользователем базы данных.
*/
SELECT x.constraint_name,
       x.table_name,
       y.table_name
  FROM all_constraints x, all_constraints y, all_tables
  WHERE x.table_name = all_tables.table_name
    AND all_tables.owner = 'CHESS'
    AND x.constraint_type = 'R'
    AND x.r_constraint_name = y.constraint_name
  ORDER BY 2;

/*
Выбрать название представления, SQL-запрос, создающий это представление - для всех представлений, созданных назначенным пользователем базы данных.
*/
SELECT all_views.view_name, all_views.text
  FROM all_views
  WHERE all_views.owner = 'CHESS';

/*
Выбрать название триггера, имя таблицы, для которой определен триггер - для всех триггеров, созданных назначенным пользователем базы данных.
*/
SELECT all_triggers.trigger_name, all_triggers.table_name
  FROM all_triggers
  WHERE all_triggers.owner = 'CHESS';

