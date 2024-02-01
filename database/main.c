//
// Created by raudy on 1/31/2024.
//
#include <stdio.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 10
#define MAX_COL_NAME_LEN 50
#define MAX_CELL_LEN 100

typedef struct {
    char name[MAX_COL_NAME_LEN];
} Column;

typedef struct {
    int id;
    char data[MAX_COLS][MAX_CELL_LEN];
} Row;

typedef struct {
    char name[MAX_COL_NAME_LEN];
    int num_columns;
    Column columns[MAX_COLS];
    int num_rows;
    Row rows[MAX_ROWS];
} Table;

typedef enum {
    Equal,
    Lower,
    Upper
} Operator;

typedef struct {
    char column_name[MAX_COL_NAME_LEN];
    Operator op;
    char value[MAX_CELL_LEN];
} Where;

void init_table(Table *table, const char *name, int num_columns, const char *column_names[]) {
    strncpy(table->name, name, MAX_COL_NAME_LEN - 1);
    table->num_columns = num_columns + 1;
    strncpy(table->columns[0].name, "Id", MAX_COL_NAME_LEN - 1);
    for (int i = 1; i < table->num_columns; i++) {
        strncpy(table->columns[i].name, column_names[i - 1], MAX_COL_NAME_LEN - 1);
    }
    table->num_rows = 0;
}

void insert_row(Table *table, const char *data[]) {
    if (table->num_rows >= MAX_ROWS) {
        printf("Table is full, cannot insert more rows.\n");
        return;
    }
    table->rows[table->num_rows].id = table->num_rows + 1;
    for (int i = 1; i < table->num_columns; i++) {
        strncpy(table->rows[table->num_rows].data[i], data[i - 1], MAX_CELL_LEN - 1);
    }
    table->num_rows++;
}

void print_table(Table *table) {
    printf("Table: %s\n", table->name);
    for (int i = 0; i < table->num_columns; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");
    for (int i = 0; i < table->num_rows; i++) {
        printf("%d\t", table->rows[i].id);
        for (int j = 1; j < table->num_columns; j++) {
            printf("%s\t", table->rows[i].data[j]);
        }
        printf("\n");
    }
}

void select_rows(Table *table, const char *columns[], int num_columns,  Where *where_conditions, int num_conditions) {
    for (int i = 0; i < table->num_rows; i++) {
        int match = 1;
        for (int j = 0; j < num_conditions; j++) {
            int col_index = -1;
            for (int k = 0; k < table->num_columns; k++) {
                if (strcmp(table->columns[k].name, where_conditions[j].column_name) == 0) {
                    col_index = k;
                    break;
                }
            }
            if (col_index == -1) {
                printf("Invalid column name: %s\n", where_conditions[j].column_name);
                return;
            }
            switch (where_conditions[j].op) {
                case Equal:
                    if (strcmp(table->rows[i].data[col_index], where_conditions[j].value) != 0) {
                        match = 0;
                    }
                    break;
                case Lower:
                    if (strcmp(table->rows[i].data[col_index], where_conditions[j].value) >= 0) {
                        match = 0;
                    }
                    break;
                case Upper:
                    if (strcmp(table->rows[i].data[col_index], where_conditions[j].value) <= 0) {
                        match = 0;
                    }
                    break;
            }
            if (!match) {
                break;
            }
        }
        if (match) {
            for (int j = 0; j < num_columns; j++) {
                int col_index = -1;
                for (int k = 0; k < table->num_columns; k++) {
                    if (strcmp(table->columns[k].name, columns[j]) == 0) {
                        col_index = k;
                        break;
                    }
                }
                if (col_index == -1) {
                    printf("Invalid column name: %s\n", columns[j]);
                    return;
                }
                printf("%s\t", table->columns[col_index].name);
            }
            printf("\n");

            for (int j = 0; j < num_columns; j++) {
                int col_index = -1;
                for (int k = 0; k < table->num_columns; k++) {
                    if (strcmp(table->columns[k].name, columns[j]) == 0) {
                        col_index = k;
                        break;
                    }
                }
                printf("%s\t", table->rows[i].data[col_index]);
            }
        }
    }
}

int main() {
    Table myTable;
    const char *column_names[] = {"Name", "Age", "City"};
    init_table(&myTable, "Person", 3, column_names);

    const char *row1_data[] = {"Alice", "25", "New York"};
    const char *row2_data[] = {"Bob", "30", "San Francisco"};

    insert_row(&myTable, row1_data);
    insert_row(&myTable, row2_data);

    print_table(&myTable);

    printf("\nFiltered rows:\n");
    const char *selected_columns[] = {"Name", };
    Where conditions[] = {{"City", Equal, "New York"}, {"Age", Upper, "0"}};
    select_rows(&myTable,selected_columns, 1,  conditions, 2);



    return 0;
}