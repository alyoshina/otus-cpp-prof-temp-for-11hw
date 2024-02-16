#pragma once

#include "db.h"
#include "utils.h"
#include "dbCommands.h"

#include <memory>
#include <list>

class Client;

class Data {
public:
    Data(ba::io_context& io_c) : ioContext(io_c) , db(std::make_shared<Db>("db")) { }
    ba::awaitable<bool> cmdExec(std::shared_ptr<Client> client, std::shared_ptr<ICmd> cmd);
private:
    ba::io_context& ioContext;
    std::shared_ptr<Db> db;
};