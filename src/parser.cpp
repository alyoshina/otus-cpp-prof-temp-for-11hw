#include "parser.h"

bool Parser::expr() {
    bool retVal = true;
    switch (nextCmd()) {
        case Lexer::CmdType::NoCmd:
            retVal = true;
            cmd = std::make_shared<NoCmd>();
        break;
        case Lexer::CmdType::Insert:
            cmd = std::make_shared<InsertCmd>(lexer->getCmd());
        break;
        case Lexer::CmdType::Truncate:
            cmd = std::make_shared<TruncateCmd>(lexer->getCmd());
        break;
        case Lexer::CmdType::Intersection:
            cmd = std::make_shared<IntersectionCmd>(lexer->getCmd());
        break;
        case Lexer::CmdType::SymmetricDifference:
            cmd = std::make_shared<SymmetricDifferenceCmd>(lexer->getCmd());
        break;
        case Lexer::CmdType::End:
            retVal = false;
            break;
        default:
            retVal = false;
        break;
    }
    return retVal;
}