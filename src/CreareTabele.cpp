#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void creare(char *sql, sqlite3 *db)
{
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    /* Execute SQL statement */
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else 
    {
        fprintf(stdout, "Table created successfully\n");
    }
}

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    rc = sqlite3_open("Librarie.db", &db);

    if( rc ) 
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else 
        fprintf(stderr, "Opened database successfully\n");

    /* Cream CARTE */
    sql = "CREATE TABLE CARTE("  \
            "ISBN           VARCHAR(14)    PRIMARY KEY     NOT NULL," \
            "titlu          VARCHAR(20)                    NOT NULL," \
            "an             INT                         NOT NULL," \
            "rating_s       INT," \
            "rating_n       INT," \
            "nr_descarcari  INT," \
            "fisier         VARCHAR(30) );";
    creare(sql,db);

    /* Cream AUTOR */
    sql = "CREATE TABLE AUTOR("  \
            "id_autor       INT         PRIMARY KEY     NOT NULL," \
            "nume           VARCHAR(20)                    NOT NULL," \
            "nr_carti       INT );";
    creare(sql,db);

    /* Cream GEN */
    sql = "CREATE TABLE GEN("  \
            "nume_gen       VARCHAR(20)    PRIMARY KEY     NOT NULL," \
            "gen_parinte    VARCHAR(20)                    NOT NULL );";
    creare(sql,db);

    sql = "CREATE TABLE PUBLICATII(" \
            "ISBN           VARCHAR(14)                    NOT NULL,"\
            "id_autor       INT                         NOT NULL );";
    creare(sql,db);

    sql = "CREATE TABLE CARTEGENURI("  \
            "ISBN           VARCHAR(14)                    NOT NULL," \
            "nume_gen       VARCHAR(20)                    NOT NULL );";
    creare(sql,db);

    sql = "CREATE TABLE AUTORGENURI("  \
            "id_autor       INT                         NOT NULL," \
            "nume_gen       VARCHAR(20)                    NOT NULL );";
    creare(sql,db);

    sql = "CREATE TABLE CLIENT("  \
            "nume           VARCHAR(20)    PRIMARY KEY     NOT NULL," \
            "parola         VARCHAR(20)                    NOT NULL," \
            "pref_gen1       VARCHAR(20)                    ," \
            "pref_gen2       VARCHAR(20)                    ," \
            "pref_aut1      VARCHAR(20)                    ," \
            "pref_aut2      VARCHAR(20)                    ," \
            "admin          INT                         NOT NULL );";
    creare(sql,db);

    sql = "CREATE TABLE CAUTARI("  \
            "client           VARCHAR(20)                  NOT NULL," \
            "ISBN             VARCHAR(14)                    ," \
            "titlu            VARCHAR(20)                    ,"\
            "autor            VARCHAR(20)                    ," \
            "an               VARCHAR(5)                         ," \
            "rating           VARCHAR(2)                         ," \
            "gen              VARCHAR(20)                    ," \
            "subgen           VARCHAR(20)                    );";
    creare(sql,db);
    
    sql = "CREATE TABLE DESCARCARI("  \
            "client       VARCHAR(20)                       NOT NULL," \
            "ISBN         VARCHAR(14)                       NOT NULL," \
            "rating       INT                          );";
    creare(sql,db);

    sqlite3_close(db);
}