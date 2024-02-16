#pragma once

#include "client.h"

template <typename T>
class Server {
public:
    Server(T& io_c, Data& d)
        : ioContext(io_c)
        , data(d) { }
    ~Server() = default;
    void listen(uint16_t port) {
        ba::co_spawn(ioContext, startAccept(port), ba::detached);
    }
    
private:
    T& ioContext;
    Data& data;
    
    /** @brief  Initiates an asynchronous accept operation to wait for a new connection 
    */
    ba::awaitable<void> startAccept(uint16_t port) {
        const auto executor = co_await ba::this_coro::executor;
        tcp::acceptor acceptor{executor, {tcp::v4(), port}};
        for (;;) {
            auto socket = co_await acceptor.async_accept(ba::use_awaitable);
            std::make_shared<Client>(std::move(socket), data)->start();
        }
    }
};