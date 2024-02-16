#pragma once

#include "db.h"
#include "commands.h"

#include <format>
#include <sstream>

class DbCmd : public ICmd {
public:
    DbCmd(const std::vector<std::string> &v) : inData(v){}
    virtual ~DbCmd() = default;
    virtual bool sqlConstruction() = 0;
    void setDb(std::shared_ptr<Db> ptr) { db = ptr; }
    bool exec() override {
        if (db) {
            if (sqlConstruction()) {
                std::string_view sv(sql.data());
                return db->sqlExec(sv, out);
            } else {
                out << "sql construction error\n";
            }
        } else {
            out << "db dosen't connect\n";
        }
        return false;
    }
    std::string getResult() override {
        return out.str();
    }
protected:
    std::string sql;
    std::vector<std::string> inData;
    std::stringstream out;
private:
    std::shared_ptr<Db> db;
};

class InsertCmd : public DbCmd {
public:
    InsertCmd(const std::vector<std::string> &v) : DbCmd(v) {}
    virtual ~InsertCmd() = default;
    bool sqlConstruction() override {
        if (inData.size() > 3) {
            sql = std::format("INSERT INTO {} VALUES ({}, '{}');\0", inData[1], inData[2], inData[3]);
            return true;
        }
        return false;
    }
};

class TruncateCmd : public DbCmd {
public:
    TruncateCmd(const std::vector<std::string> &v) : DbCmd(v) {}
    virtual ~TruncateCmd() = default;
    bool sqlConstruction() override {
        if (inData.size() > 1) {
            sql = std::format("DELETE FROM {};\0", inData[1]);
            return true;
        }
        return false;
    }
};

class IntersectionCmd : public DbCmd {
public:
    IntersectionCmd(const std::vector<std::string> &v) : DbCmd(v) {}
    virtual ~IntersectionCmd() = default;
    bool sqlConstruction() override {
        if (inData.size() > 0) {
            sql = std::format("SELECT a.id, a.name, b.name FROM A AS a"
                                " INNER JOIN B AS b ON a.id = b.id;\0");
            return true;
        }
        return false;
    }
};

class SymmetricDifferenceCmd : public DbCmd {
public:
    SymmetricDifferenceCmd(const std::vector<std::string> &v) : DbCmd(v) {}
    virtual ~SymmetricDifferenceCmd() = default;
    bool sqlConstruction() override {
        if (inData.size() > 0) {
            sql = std::string("SELECT CASE WHEN a.id IS NULL THEN b.id ELSE a.id END"
                                ", a.name, b.name FROM A AS a FULL OUTER JOIN B AS b ON a.id = b.id"
                                " WHERE a.id IS NULL OR b.id IS NULL;\0");
            return true;
        }
        return false;
    }
};