#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"


#include <string>
#include <sstream>

#include "tokenize.hpp"

#include "expression.hpp"

TEST_CASE("Test Tokenizer with expected input [1]", "[tokenize]") {

	std::string program = "(begin (define r 10) (* pi (* r r)))";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 17);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == "begin");
	REQUIRE(tokens[2] == "(");
	REQUIRE(tokens[3] == "define");
	REQUIRE(tokens[4] == "r");
	REQUIRE(tokens[5] == "10");
	REQUIRE(tokens[6] == ")");
	REQUIRE(tokens[7] == "(");
	REQUIRE(tokens[8] == "*");
	REQUIRE(tokens[9] == "pi");
	REQUIRE(tokens[10] == "(");
	REQUIRE(tokens[11] == "*");
	REQUIRE(tokens[12] == "r");
	REQUIRE(tokens[13] == "r");
	REQUIRE(tokens[14] == ")");
	REQUIRE(tokens[15] == ")");
	REQUIRE(tokens[16] == ")");
}

TEST_CASE("Test Tokenizer with truncated input [1]", "[tokenize]") {

	std::string program = "(f";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 2);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == "f");
}

TEST_CASE("Test Tokenizer with single non-keyword [1]", "[tokenize]") {

	std::string program = "hello";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 1);
	REQUIRE(tokens[0] == "hello");
}

TEST_CASE("Test Tokenizer with empty input [1]", "[tokenize]") {

	std::string program;

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 0);
}

TEST_CASE("Test Tokenizer with empty expression [1]", "[tokenize]") {

	std::string program = "( )";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 2);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == ")");
}

TEST_CASE("Test Type Inference [1]", "[types]") {

	Atom a;

	std::string token = "True";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == BooleanType);
	REQUIRE(a.value.bool_value == true);

	token = "False";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == BooleanType);
	REQUIRE(a.value.bool_value == false);

	token = "1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == NumberType);
	REQUIRE(a.value.num_value == 1);

	token = "-1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == NumberType);
	REQUIRE(a.value.num_value == -1);

	token = "var";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == SymbolType);
	REQUIRE(a.value.sym_value == "var");

	token = "1abc";
	REQUIRE(!token_to_atom(token, a));

	token = "var1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == SymbolType);
	REQUIRE(a.value.sym_value == token);

}

TEST_CASE("Test Expression Constructors [1]", "[types]") {

	Expression exp1;

	REQUIRE(exp1 == Expression());
}


