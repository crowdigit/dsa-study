#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include <string>
#include <optional>

class Operator {
public:
    enum class type {
        plus,
        minus,
        multiply,
        divide,
        power,
    };

    enum class associativity {
        left,
        right,
    };

    static auto is_operator(std::string_view input) -> std::optional<Operator>;

    Operator(Operator const& o);
    Operator(type const type);

    explicit operator char () const;
    auto operator < (Operator const& rhs) const -> bool;

    auto precedence() const -> int;
    auto associativity() const -> associativity;
    auto evaluate(int const a, int const b) const -> int;

private:
    type _type;
};

#endif
