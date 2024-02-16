#pragma once

#include "lexer.h"
#include "dbCommands.h"
#include "utils.h"

#include <memory>

/** @brief Convert received command type into a command.
*
*/
class Parser {
public:
    Parser(std::shared_ptr<Lexer> l) : lexer(l) {}
    virtual ~Parser() = default;
    Lexer::CmdType nextCmd() { return lexer->nextLine(); }
    template<typename T>
    ba::awaitable<bool> parse(std::shared_ptr<T> client) {
        auto initiate = [this, client]<typename Handler>(Handler&& handler) mutable {
            ba::post(client->getParserContext().get_executor()
                        , [handler = std::forward<Handler>(handler), this, client]() mutable {
                handler(this->expr());
            });
        };
        return ba::async_initiate<decltype(ba::use_awaitable), void(bool)>(initiate, ba::use_awaitable);
    }

    /** @brief Creates a command according to received type.
    * @return false if end
    */
    bool expr();
    std::shared_ptr<ICmd> getCmd() { return cmd; }
private:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<ICmd> cmd;
};