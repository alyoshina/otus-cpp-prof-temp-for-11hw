#pragma once

#include "db.h"
#include "utils.h"
#include "dbCommands.h"

#include <memory>
#include <list>

class Data {
public:
    Data(ba::io_context& io_c, ba::io_context& pio_c)
        : ioContext(io_c)
        , parserIoContext(pio_c)
        , db(std::make_shared<Db>("db"))
    { }
    template<typename T>
    ba::awaitable<bool> cmdExec(std::shared_ptr<T> client, std::shared_ptr<ICmd> cmd) {
        auto initiate = [this, cmd, client]<typename Handler>(Handler&& handler) mutable {
            ba::post(ioContext.get_executor()
                        , [handler = std::forward<Handler>(handler), this, cmd, client]() mutable {
                auto ptr = std::dynamic_pointer_cast<DbCmd>(cmd);
                if (ptr) ptr->setDb(db->getDbPtr());
                handler(cmd->exec());
            });
        };
        return ba::async_initiate<decltype(ba::use_awaitable), void(bool)>(initiate, ba::use_awaitable);
    }
    auto& getParserContext() { return parserIoContext; }
private:
    ba::io_context& ioContext;
    ba::io_context& parserIoContext;
    std::shared_ptr<Db> db;
};