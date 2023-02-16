struct ques{
    int id;
    char q_text[100];
    char answers[100];
    int correct_ans;
} question;

int createDB(int last_id)
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("MyDB.db", &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql = "DROP TABLE IF EXISTS Quizz;"
    "CREATE TABLE Quizz(Id INT PRIMARY KEY, Question TEXT, A1 TEXT, A2 TEXT, A3 TEXT, Answer INT);"
    "INSERT INTO Quizz VALUES(1,'Acest operator „<>” semnifica:','Negare','Comparare','Concatenare', 2);"
    "INSERT INTO Quizz VALUES(2,'In Word, dimensiunea unui caracter se masoara in:','Puncte','Centimetri','Inches', 1);"
    "INSERT INTO Quizz VALUES(3,'In Excel, referintele de celule pot fi:','Relative','Constante','Simple', 1);"
    "INSERT INTO Quizz VALUES(4,'Ce reprezinta functia IsText in Excel?','O functie statistica','O functie de cautare','O functie de informare', 3);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    } else{
        fprintf(stdout, "Table Quizz created successfully\n");
    }

    //id-ul ultimului element inserat - in acest caz: 4
    last_id = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);

    return last_id;
}

//luam interogarea i din tabela Quizz
void i_query(int i_quizz_number){

    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMesg = 0;

    int ret = 0;
    ret = sqlite3_open("MyDB.db", &db);
    if (ret != SQLITE_OK) {
        printf("Database opening error\n");
    }

    char sql_stmt[100];
    strcpy(sql_stmt, "SELECT * FROM Quizz WHERE Id=");
    //convertim int to char
    char converted_number[5];
    sprintf(converted_number, "%d", i_quizz_number);
    strcat(sql_stmt, converted_number);
    strcat(sql_stmt, ";" );

    ret = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);
    if (ret != SQLITE_OK) {
        printf("\n Unable to fetch data");
        sqlite3_close(db);
    }

    char ans1[20], ans2[20], ans3[20];
    if((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        strcpy(question.q_text, sqlite3_column_text(stmt, 1));
        strcpy(ans1, sqlite3_column_text(stmt, 2));
        strcpy(ans2, sqlite3_column_text(stmt, 3));
        strcpy(ans3, sqlite3_column_text(stmt, 4));
        question.correct_ans = sqlite3_column_int(stmt, 5);
    }
    strcpy(question.answers, "\0");
    strcat(question.answers, "1-");
    strcat(question.answers, ans1);
    strcat(question.answers, " | 2-");
    strcat(question.answers, ans2);
    strcat(question.answers, " | 3-");
    strcat(question.answers, ans3);
    strcat(question.answers, "\0");

    //if (ret != SQLITE_DONE) {
    //fprintf(stderr, "Eroare la execuția interogării: %s\n", sqlite3_errmsg(db));
    //sqlite3_close(db);
    //}

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}