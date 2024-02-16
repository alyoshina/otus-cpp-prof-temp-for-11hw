#pragma once

#include <iostream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

/** @brief Converting string received from client into a command type
*
*/
class Lexer {
public:
    Lexer(std::istream &in) : c_in(in) {}
    virtual ~Lexer() = default;
    const std::vector<std::string> &getCmd() const { return cmd; }

    /** @enum CmdType
    * @brief Command type
    */
    enum class CmdType {
        NoCmd, /**< is no command */
        Insert, /**< is command insert data to db */
        Truncate, /**< is clear table to db */
        Intersection, /**< is intersection join to db */
        SymmetricDifference, /**< is symmetric difference join to db */ 
        End, /**< is input end */
    };

    /** @brief Read line from console and convert into a command type
    * @return command type
    */
    CmdType nextLine() {
        if (!readLine(strCmd)) {
            return CmdType::End;
        }
        cmd.clear();
        boost::split(cmd, strCmd, boost::is_any_of(" "));
        if (сmdType.contains(cmd[0])) {
            return сmdType[cmd[0]];
        }
        return CmdType::NoCmd;
    }

protected:
    std::istream &c_in;
    std::string strCmd;
    std::vector<std::string> cmd;
    std::unordered_map<std::string, CmdType> сmdType {
        {"INSERT", CmdType::Insert},
        {"TRUNCATE", CmdType::Truncate},
        {"INTERSECTION", CmdType::Intersection},
        {"SYMMETRIC_DIFFERENCE", CmdType::SymmetricDifference},
        {"End", CmdType::End}
    };

    virtual bool readLine(std::string &str) {
        return bool(std::getline(c_in, str));
    }
};