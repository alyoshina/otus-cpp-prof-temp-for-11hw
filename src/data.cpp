#include "data.h"
#include "client.h"

ba::awaitable<bool> Data::cmdExec(std::shared_ptr<Client> client, std::shared_ptr<ICmd> cmd) {
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