

/*
void play_capstone() {

    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
        return -1;

    cs_insn* insn;
    size_t count = cs_disasm(handle,
        shellcode, sizeof(shellcode)-1,
        0x1000, 0, &insn);

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            printf("0x%x:\t%s\t%s\n", insn[i].address, insn[i].mnemonic,
            insn[i].op_str);
        }

        cs_free(insn, count);
    } else
        printf("ERROR: Failed to disassemble given code!\n");

    cs_close(&handle);
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {

    for(int i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void play_sqlite() {

    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);;
    if (rc) {
        fprintf(stderr, "Can't open db: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    printf("Opended the database!\n");

    char* sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) " \
                "VALUES (1, 'Paul', 32, 'California', 20000.00); "  \
                "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) " \
                "VALUES (2, 'Allen', 25, 'Texass', 200000.00); "    \
                "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) " \
                "VALUES (3, 'Poe', 32, 'China', 2.00); "            \
                "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) " \
                "VALUES (4, 'Markus', 32, 'Sparta', 2.00); ";

    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    else {
        fprintf(stderr, "Records created successfully\n");
    }

    sqlite3_close(db);
}

void create_assembly_db() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);;
    if (rc) {
        fprintf(stderr, "Can't open db: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    "CREATE TABLE instructions (" \
    "line    INT           PRIMARY KEY  NOT NULL," \
    "size    INT," \
    "bytes   varbinary(16) f,"
    "opcode  INT," \

}
*/
