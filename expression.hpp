#ifndef TYPES_HPP
#define TYPES_HPP

// system includes
#include <string>
#include <vector>
#include <tuple>

// A Type is a literal boolean, literal number, or symbol
enum Type { NoneType, PointType, LineType, ArcType, BooleanType, NumberType, ListType, SymbolType };

// A Boolean is a C++ bool
typedef bool Boolean;

// A Number is a C++ double
typedef double Number;

// A Symbol is a string
typedef std::string Symbol;

// A Value is a boolean, number, or symbol
// cannot use a union because symbol is non-POD
// this wastes space but is simple 
struct Value {
    Boolean bool_value = false;
    Number num_value = 0.0;
    Symbol sym_value = "";
    std::tuple<Number, Number>start_coords;
    std::tuple<Number, Number>end_coords;

    bool operator==(const Value& val) const {
        bool ret = false;
        if ((bool_value == val.bool_value) ||
            (num_value == val.num_value) ||
            (start_coords == val.start_coords) ||
            (end_coords == val.end_coords) ||
            (sym_value == val.sym_value)) {
            ret = true;
        }
        return ret;
    }
};

// An Atom has a type and value
struct Atom {
    Type type = NoneType;
    Value value;
    bool operator==(const Atom& val) const {
        bool ret = false;
        if (type == val.type && value == val.value) {
            ret = true;
        }
        return ret;
    }
};

// An expression is an atom called the head
// followed by a (possibly empty) list of expressions
// called the tail
struct Expression {
    Atom head;
    std::vector<Expression> tail;

    Expression() {
        head.type = NoneType;
    };

    Expression(const Atom& atom) : head(atom) {};
    Expression(bool tf);
    Expression(double num);
    Expression(const std::string& sym);

    // Construct an Expression with a single Point atom with value
    Expression(std::tuple<double, double> value);

    // Construct an Expression with a single Line atom with starting
    // point start and ending point end
    Expression(std::tuple<double, double> start,
        std::tuple<double, double> end);

    // Construct an Expression with a single Arc atom with center
    // point center, starting point start, and spanning angle angle in radians
    Expression(std::tuple<double, double> center,
        std::tuple<double, double> start,
        double angle);

    

    bool operator==(const Expression& exp) const noexcept;
};


std::string expToString(const Expression& exp);

// A Procedure is a C++ function pointer taking
// a vector of Atoms as arguments
typedef Expression(*Procedure)(const std::vector<Atom>& args);  //expresion = env.envmap["+"].proc(args)

//typedef Expression(*Expression)(const std::vector<Atom>& args);

// format an expression for output
std::ostream& operator<<(std::ostream& out, const Expression& exp);

// map a token to an Atom
bool token_to_atom(const std::string& token, Atom& atom);
#endif
