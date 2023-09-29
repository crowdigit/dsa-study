#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>

class Expression {
public:
    class iterator {
    public:
        using size_type = std::string::size_type;
        static auto const npos = std::string::npos;

        iterator(std::string const& input, size_type const cursor);

        auto operator != (iterator const& op) const -> bool;
        auto operator ++ () -> iterator&;
        auto operator * () const -> std::string_view;

    private:
        std::string const& input;
        size_type cursor;
        size_type len;
    };

    Expression(std::string const& input);

    auto begin() const -> iterator;
    auto end() const -> iterator;

private:
    std::string const input;
};

#endif
