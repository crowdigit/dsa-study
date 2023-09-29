#include "./expression.h"

using namespace std;

using iterator = Expression::iterator;

namespace {

auto find_token(
        string const& input,
        ::iterator::size_type const from)
    -> ::iterator::size_type
{
    if (from == ::iterator::npos)
        return ::iterator::npos;
    for (auto i = from; i < input.length(); i += 1)
        if (input[i] != ' ')
            return i;
    return ::iterator::npos;
}

auto get_token_length(
        string const& input,
        ::iterator::size_type const from)
    -> ::iterator::size_type
{
    if (from == ::iterator::npos)
        return ::iterator::npos;
    for (auto i = from; i < input.length(); i += 1)
        if (input[i] == ' ')
            return i-from;
    return input.length() - from;
}

}

Expression::Expression(string const& input)
    : input(input)
{ }

auto Expression::begin() const -> iterator
{
    return Expression::iterator(input, 0);
}

auto Expression::end() const -> iterator
{
    return Expression::iterator(input, iterator::npos);
}

Expression::iterator::iterator(
        std::string const& input,
        size_type const cursor)
    : input(input)
    , cursor(find_token(input, cursor))
    , len(get_token_length(input, this->cursor))
{ }

auto ::iterator::operator != (iterator const& op) const -> bool
{
    return cursor != op.cursor;
}

auto ::iterator::operator ++ () -> iterator&
{
    cursor = find_token(input, cursor+len);
    len = get_token_length(input, cursor);
    return *this;
}

auto ::iterator::operator * () const -> std::string_view
{
    return string_view(input.c_str()+cursor, len);
}
