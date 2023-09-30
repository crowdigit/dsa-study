#include "./operator.h"

using namespace std;

auto Operator::is_operator(std::string_view input) -> std::optional<Operator>
{
    if (input == "+") return Operator(type::plus);
    if (input == "-") return Operator(type::minus);
    if (input == "*") return Operator(type::multiply);
    if (input == "/") return Operator(type::divide);
    if (input == "^") return Operator(type::power);
    if (input == "(") return Operator(type::bracket_open);
    if (input == ")") return Operator(type::bracket_close);
    return std::nullopt;
}

Operator::Operator(Operator const& o)
    : _type(o._type)
{ }

Operator::Operator(enum type const type)
    : _type(type)
{ }

Operator::operator char () const {
    switch (_type) {
        case type::plus: return '+';
        case type::minus: return '-';
        case type::multiply: return '*';
        case type::divide: return '/';
        case type::power: return '^';
        case type::bracket_open: return '(';
        case type::bracket_close: return ')';
    }
    throw runtime_error("invalid operator");
}

auto Operator::operator < (Operator const& rhs) const -> bool
{
    auto const l_prec = precedence();
    auto const r_prec = rhs.precedence();
    if (l_prec != r_prec) return l_prec < r_prec;
    return rhs.associativity() == associativity::left;
}


auto Operator::precedence() const -> int
{
    switch (_type) {
        case type::plus: return 1;
        case type::minus: return 1;
        case type::multiply: return 2;
        case type::divide: return 2;
        case type::power: return 3;
        case type::bracket_open: return 4;
        case type::bracket_close: return 4;
    }
    throw runtime_error("invalid operator");
}

auto Operator::associativity() const -> enum Operator::associativity
{
    switch (_type) {
        case type::plus: return associativity::right;
        case type::minus: return associativity::right;
        case type::multiply: return associativity::right;
        case type::divide: return associativity::right;
        case type::power: return associativity::left;
        case type::bracket_open: return associativity::right;
        case type::bracket_close: return associativity::right;
    }
    throw runtime_error("invalid operator");
}


auto Operator::evaluate(int const a, int const b) const -> int
{
    switch (_type) {
    case type::plus: return a + b;
    case type::minus: return a - b;
    case type::multiply: return a * b;
    case type::divide: return a / b;
    case type::power: return pow(a, b);
    case type::bracket_open:
    case type::bracket_close:
        throw runtime_error("invalid operator");
    }
}

auto Operator::type() const -> enum type
{
    return _type;
}
