#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>
#include <vector>
// module includes
#include "expression.hpp"

class Environment {
public:

    //Expression define(const std::vector<Atom>& args);

    //Expression begin(const std::vector<Atom>& args);
    void expToEnvmap(std::string input, Expression exp);

    void reset();

    Environment();

    enum EnvResultType { ExpressionType, ProcedureType };

    struct EnvResult {
        EnvResultType type;
        Expression exp;
        Procedure proc;
    };

    //std::map<Symbol, EnvResult> getEnvMap();

    std::map<Symbol, EnvResult> envmap;

private:

    // Environment is a mapping from symbols to expressions or procedures

    //std::map<Symbol,EnvResult> envmap;
};

#endif
