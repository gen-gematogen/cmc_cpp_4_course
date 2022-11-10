#include <iostream>

template <class T, class...>
struct are_same : std::true_type
{
};

template <class T, class U, class... TT>
struct are_same<T, U, TT...>
    : std::integral_constant<bool, std::is_same<std::remove_cvref_t<T>, std::remove_cvref_t<U>>{} && are_same<T, TT...>{}>
{
};

template <typename... Arguments>
class VariadicTemplate
{
    static_assert(are_same<Arguments...>{}, "compile assert");
};

int main()
{
    VariadicTemplate<int, int &, const int, const int &> v0{};
}