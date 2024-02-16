#pragma once

#include <format>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <sqlite3.h>
#include <iostream>

class Db : public std::enable_shared_from_this<Db> {
public:
    Db(const char *dbName) {
        if (connect(dbName)) {
            createTables("A");
            createTables("B");
        }
    }
    ~Db() {
        disconnect();
    }
    std::shared_ptr<Db> getDbPtr() { return shared_from_this(); }
    bool connect(const char *dbName) {
        name = std::string(dbName);
        return check(sqlite3_open(name.data(), &db)) == SQLITE_OK;
    }
    int check(int code, int expected = SQLITE_OK) {
        if (code != expected) {
            //throw std::runtime_error{sqlite3_errstr(code)};
            ;
        }
        return code;
    }
    int disconnect() { return sqlite3_close(db); }
    bool createTables(std::string_view nameTable) {
        std::string sql = std::format("CREATE TABLE IF NOT EXISTS {}(id INTEGER PRIMARY KEY, "
                                        "name TEXT);\n"
                                        , nameTable);
        return sqlExec(sql);
    }

    bool sqlExec(std::string_view sql) {
        std::stringstream out;
        return sqlExec(sql, out);
    }

    bool sqlExec(std::string_view sql, std::stringstream &out) {
        auto printer = [] (void* stream, int col, char** strData, char** colName) {
            std::stringstream &os = (*reinterpret_cast<std::stringstream*>(stream));
            for (int i = 0; i < col; i++) {
                if (strData[i]) {
                    os << strData[i];
                }
                os << std::string(",");
            }
            os.seekp(-1, std::ios_base::end);
            os << "\n";
            return 0;
        };
        char* errmsg{};
        if (check(sqlite3_exec(db, sql.data(), printer, &out, &errmsg)) != SQLITE_OK) {
            out << std::string(errmsg);
            sqlite3_free(errmsg);
            return false;
        }
        return true;
    }
private:
    std::string name;
    sqlite3* db;
};