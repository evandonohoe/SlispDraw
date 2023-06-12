#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

Interpreter::Interpreter() {
    env = Environment();
}


void incrementParens(std::string& token, int& openCount, int& closeCount) {
    if (token == ")") {
        closeCount++;
    }
    else if (token == "(") {
        openCount++;
    }
}

Expression parseTokenSequence(TokenSequenceType& input, bool& flag) noexcept {

    Expression node = Expression();

    if (!(input.empty())) {

        //pop first token
        std::string token = input.front();
        input.erase(input.begin());

        if (token == ";") {
            while (input.front() != "\n")
            {
                input.erase(input.begin());
            }
        }

        Atom add;

        if (token == "(") { //assumes at least 2 tokens to follow

            if (input.size() < 2) {
                flag = false;
                return node;
            }

            //pop head of node
            token = input.front();
            input.erase(input.begin());

            //set as head of node expression
            if (token_to_atom(token, add)) {
                node.head = add;
            }
            else {
                flag = false;
                return node;
            }

            //get next arg
            token = input.front();

            while (token != ")") {

                node.tail.push_back(parseTokenSequence(input, flag));
                if (!input.empty()) {
                    token = input.front();
                }
                else {
                    flag = false;
                    return node;
                }

            }
            input.erase(input.begin());

        }
        else {

            token_to_atom(token, add);
            node.head = add;
        }
    }
    return node;

}

bool findSymbolInMap(Environment env, const Expression& e) {
    bool found = false;
    if (e.head.type == SymbolType) {
        //qDebug() << "In find: " << QString::fromStdString(std::string(e.head.value.sym_value));
        auto it = env.envmap.find(e.head.value.sym_value);
        if (it != env.envmap.end()) {
            found = true;
        }
    }
    return found;
}


bool Interpreter::parse(std::istream& expression) noexcept {

    //get rid of comment lines
    std::string line;
    std::string conc;
    while (getline(expression, line))
    {
        size_t comment = line.find(";");
        line = line.substr(0, comment);
        conc += line;
    }
    std::istringstream str(conc);

    TokenSequenceType tokenized_input = tokenize(str);
    int openCount = 0;
    int closeCount = 0;
    bool flag = true;
    bool flag2 = true;
    Atom test_atom;

    if (tokenized_input.empty()) {
        return false;
    }

    for (auto& tk : tokenized_input) {
        incrementParens(tk, openCount, closeCount);
    }

    if (tokenized_input.size() == 1 || (openCount + closeCount) == 0) {
        flag = token_to_atom(tokenized_input.front(), test_atom);
        if (flag) {
            flag2 = findSymbolInMap(env, Expression(test_atom));

        }
        if (!flag || !flag2) {
            return false;
        }
    }

    ast = parseTokenSequence(tokenized_input, flag);
    if (((openCount - closeCount) != 0)) {
        flag = false;
    }
    if (!tokenized_input.empty()) {
        flag = false;
    }


    return flag;
};

Expression Interpreter::eval() {

    Expression result = evalExp(ast);

    return result;
}

//searches for expression in environment map, returns
Expression searchAndReturn(Environment env, const Expression& exp, std::vector<Atom>& args) {
    Expression result;
    if (exp.head.type == SymbolType) {
        bool found = findSymbolInMap(env, exp);
        if (found) {
            Environment::EnvResult res = env.envmap[exp.head.value.sym_value];
            if (res.type == Environment::ProcedureType) {
                result = res.proc(args);
            }
            else if (res.type == Environment::ExpressionType) {
                result = Expression(res.exp);

            }
        }
        else {
            throw InterpreterSemanticError("Error in call to searchAndReturn, can't find variable");
        }
    }
    else {
        result = Expression(exp.head);
    }
    return result;
}

//throws error if invalid use of define is attempted
void checkDefineSemantics(std::string symbol, std::size_t size, Environment& env, Expression exp) {
    if (size != 2) {
        throw InterpreterSemanticError("Error in call to define, too many arguments");
    }
    if (symbol == "pi" || symbol == "if" || symbol == "begin" || symbol == "define") {
        throw InterpreterSemanticError("Error in call to define, can't redefine built in variable");
    }
    bool found = findSymbolInMap(env, exp);
    if (found) {
        throw InterpreterSemanticError("Error in call to define, can't redefine variable");
    }
}

//checks if the expressions of the tail of the given expression are have empty tails or not, 
//if not the expression can be evalutated directly(return true), otherwise, the tails must be simplified (return false)
bool checkTails(const Expression& exp, std::vector<Atom>& args, Environment& env) {
    for (auto& e : exp.tail) {
        if (e.tail.empty()) {
            bool found = findSymbolInMap(env, e);
            if (found) {
                Expression retrieve = env.envmap[e.head.value.sym_value].exp;
                args.push_back(retrieve.head);
            }
            else {
                args.push_back(e.head);
            }
        }
        else {
            return false;
        }
    }
    return true;
}

std::vector<Expression> Interpreter::getDrawList() {
    return draw_list;
}

//recursive evaluation of given expression
Expression Interpreter::evalExp(const Expression& exp) {
    Expression result;
    std::vector<Atom> args;

    bool canEvaluate = checkTails(exp, args, env);

    if (canEvaluate) {
        
        if (exp.head.value.sym_value == "draw") {
            // get current draw list from envmap, add things to draw
            for (auto& e : exp.tail) {
                
                if (e.head.type == SymbolType) {
                    bool found = findSymbolInMap(env, e);
                    if (found) {
                        Expression retrieve = env.envmap[e.head.value.sym_value].exp;
                        draw_list.push_back(retrieve);
                    }
                    else {
                        throw InterpreterSemanticError("Error in call to draw, argument not found");
                    }
                } else if ((e.head.type != PointType) && (e.head.type != LineType) && (e.head.type != ArcType)) {
                    throw InterpreterSemanticError("Error in call to draw, argument not a graphics item");
                }
                else {
                    draw_list.push_back(e);
                }
            }

            result = Expression("expression");
        } 
        else if (exp.head.value.sym_value == "define") {

            std::string symbol = exp.tail[0].head.value.sym_value; //add mapping of symbol to expression

            checkDefineSemantics(symbol, exp.tail.size(), env, Expression(exp.tail[0].head));

            if (exp.tail[1].head.type == SymbolType) {
                env.expToEnvmap(symbol, Expression(env.envmap[exp.tail[1].head.value.sym_value].exp));
            }
            else {
                env.expToEnvmap(symbol, Expression(exp.tail[1].head));
            }

            result = env.envmap[symbol].exp;

        }
        else if (exp.head.value.sym_value == "begin") {

            Expression last_exp; // = exp.tail.back();
            for (auto& e : exp.tail) {
                last_exp = evalExp(e);
            }
            result = searchAndReturn(env, last_exp, args);

        }
        else if (exp.head.value.sym_value == "if") {

            if (exp.tail.size() != 3) {
                throw InterpreterSemanticError("Error in call to define, too many / not enough arguments");
            }
            Expression conditional = evalExp(exp.tail[0]);
            if (conditional.head.value.bool_value) {
                result = evalExp(exp.tail[1]);
            }
            else {
                result = evalExp(exp.tail[2]);
            }
            result = searchAndReturn(env, result, args);

        }
        else {
            //qDebug() << "Before search and return";
            result = searchAndReturn(env, exp, args);
        }
    }
    else {
        Expression simplified_exp;
        simplified_exp.head = exp.head;

        if (exp.head.value.sym_value == "define") {

            std::string symbol = exp.tail[0].head.value.sym_value;

            checkDefineSemantics(symbol, exp.tail.size(), env, Expression(exp.tail[0].head));
            Expression add_exp = evalExp(exp.tail[1]);
            env.expToEnvmap(symbol, add_exp);
            result = env.envmap[symbol].exp;

        }
        else if (exp.head.value.sym_value == "begin") {

            Expression last_exp;
            for (auto& e : exp.tail) {
                last_exp = evalExp(e);
            }
            result = last_exp;
        }
        else {
            for (auto& e : exp.tail) {
                if (!e.tail.empty()) {
                    simplified_exp.tail.push_back(evalExp(e));
                }
                else {
                    simplified_exp.tail.push_back(e);
                }
            }
            Expression simplified_result = evalExp(simplified_exp);
            return simplified_result;
        }
    }
    //qDebug() << "(return)";
    return result;
}