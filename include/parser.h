#pragma once

#include "lexer.h"
#include "dbCommands.h"

#include <memory>

/** @brief Convert received command type into a command.
*
*/
class Parser {
public:
    Parser(std::shared_ptr<Lexer> l) : lexer(l) {}
    virtual ~Parser() = default;
    Lexer::CmdType nextCmd() { return lexer->nextLine(); }
    bool parse() { return expr(); }

    /** @brief Creates a command according to received type.
    * @return false if end
    */
    bool expr();
    std::shared_ptr<ICmd> getCmd() { return cmd; }
private:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<ICmd> cmd;
};