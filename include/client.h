#pragma once

#include "parser.h"
#include "data.h"

#include <boost/algorithm/string/trim_all.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

#include <iostream>
#include <memory>

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(tcp::socket s, Data& d)
        : socket(std::move(s))
        , data(d) {
        parser = std::make_shared<Parser>(std::make_shared<Lexer>(input));
    }
    ~Client() = default;
    void start() {
        ba::co_spawn(socket.get_executor()
                    , [self = shared_from_this()] { return self->read(); }
                    , ba::detached);
    }
    
private:
    tcp::socket socket;
    Data& data;
    ba::streambuf buffer;
    std::stringstream input;
    std::shared_ptr<Parser> parser;

    ba::awaitable<void> read() {
        for (;;) {
            buffer.consume(buffer.size());
            try {
                co_await ba::async_read_until(socket, buffer, "\n", ba::use_awaitable);
                input << std::string(ba::buffer_cast<const char*>(buffer.data()), buffer.size());
                if (parser->parse()) {
                    std::shared_ptr<ICmd> cmd = parser->getCmd();
                    std::string response;
                    if (co_await data.cmdExec(shared_from_this(), cmd)) {
                        response = std::format("{}OK\n", cmd->getResult());
                    } else {
                        response = std::format("ERR {}\n", cmd->getResult());
                    }
                    co_await ba::async_write(socket
                                    , ba::buffer(response.data(), response.size())
                                    , ba::use_awaitable);
                }
            } catch (const boost::system::system_error& ex) {
                if (ex.code() != ba::error::eof) {
                    throw;
                }
                break;
            }
        }
    }
};