#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include <string>
#include <optional>

class Operator {
public:
    enum class type {
        Plus,
        Minus,
        Multiply,
        Divide,
        Power,
    };

    static auto is_operator(std::string_view input) -> std::optional<Operator>;

    Operator(Operator const& o);
    Operator(type const type);

    explicit operator char () const;

    auto precedence() const -> int;
    auto evaluate(int const a, int const b) const -> int;

private:
    type _type;
};

#endif
