#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <regex>

// system includes
#include <sstream>


Expression::Expression(bool tf) {
	head.type = BooleanType;
	head.value.bool_value = tf;
}

Expression::Expression(double num) {
	head.type = NumberType;
	head.value.num_value = num;
}

Expression::Expression(const std::string& sym) {
	head.type = SymbolType;
	head.value.sym_value = sym;
}

Expression::Expression(std::tuple<double, double> value) {
	head.type = PointType;
	head.value.start_coords = value;
}

Expression::Expression(std::tuple<double, double> start,
	std::tuple<double, double> end) {
	head.type = LineType;
	head.value.start_coords = start;
	head.value.end_coords = end;
	
}

Expression::Expression(std::tuple<double, double> center,
	std::tuple<double, double> start,
	double angle) {
	head.type = ArcType;
	head.value.num_value = angle;
	head.value.start_coords = center;
	head.value.end_coords = start;
}

bool Expression::operator==(const Expression& exp) const noexcept {
	bool ret = false;
	if ((head == exp.head) && (tail == exp.tail)) {
		ret = true;
	}
	return ret;
}

std::ostream& formatExpression(std::ostream& out, const Expression& exp) {
	out << "(";

	int rounded_double = 0;
	switch (exp.head.type) {
	case BooleanType:
		if (exp.head.value.bool_value) {
			out << "True";
		}
		else {
			out << "False";
		}
		break;
	case NumberType:
		rounded_double = static_cast<int>(round(exp.head.value.num_value));
		out << std::to_string(rounded_double);
		break;
	case SymbolType:
		out << exp.head.value.sym_value;
		break;
	case PointType:
		out << std::get<0>(exp.head.value.start_coords) <<","<< std::get<1>(exp.head.value.start_coords);
		break;
	default:
		out << "NoneTypeVal";
	}

	for (auto& a : exp.tail) {
		out << " " << a;
	}

	out << ")";
}

std::ostream& operator<<(std::ostream& out, const Expression& exp) {
	// TODO: implement this function
	formatExpression(out, exp);
	return out;
}

std::string expToString(const Expression& exp) {
	std::ostringstream oss;
	std::ostream& out = oss;
	formatExpression(out, exp);
	std::string str = oss.str();
	return str;
}

bool isValidNumber(const std::string& str) {
	std::regex numberRegex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");
	return std::regex_match(str, numberRegex);
}

bool isValidSymbol(const std::string& str) {
	std::regex symbolRegex("^[^0-9[:space:]].*$");
	return std::regex_match(str, symbolRegex);
}

bool token_to_atom(const std::string& token, Atom& atom) {

	bool isNumber = isValidNumber(token);
	bool isBool = (token == "True" || token == "False");
	bool isSymbol = isValidSymbol(token);

	if (isBool) {
		atom.type = BooleanType;
		if (token == "True") {
			atom.value.bool_value = true;
		}
		else {
			atom.value.bool_value = false;
		}
	}
	else if (isNumber) {
		double NumValue = std::stof(token);
		atom.type = NumberType;
		atom.value.num_value = NumValue;
	}
	else if (isSymbol) {
		std::string SymValue = token;

		atom.type = SymbolType;
		atom.value.sym_value = SymValue;
		if (SymValue == "pi") {
			atom.type = NumberType;
			atom.value.num_value = atan2(0, -1);
		}
	}
	else {
		return false;
	}


	return true;
}

