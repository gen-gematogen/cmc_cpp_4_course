#include <string>
#include <sstream>
#include <iostream>
#include <set>
#include <vector>
#include <tuple>

struct Printer
{
    std::stringstream s;

    Printer() {}

    std::string str() const
    { // возвращает строковое представление
        return s.str();
    }

    // далее перегруженные/шаблонные функции вида:
    template <size_t I = 0, typename... Types>
    Printer &format(std::tuple<Types...> data)
    {
        if constexpr(I == 0){
            s << "(";
        }

        if constexpr (I == sizeof...(Types) - 1)
        {
            Printer::format(std::get<I>(data));
            s << ")";

            return *this;
        }
        else
        {
            Printer::format(std::get<I>(data));

            s << ", ";

            Printer::format<I + 1>(data);
        }

        return *this;
    }

    Printer &format(long long data)
    {
        s << std::to_string(data);

        return *this;
    }

    Printer &format(std::string data)
    {
        s << data;
        return *this;
    }

    template <typename T, typename V>
    Printer &format(std::pair<T, V> &data)
    {
        s << "(";
        Printer::format(data.first);
        s << ", ";
        Printer::format(data.second);
        s << ")";

        return *this;
    }

    template <typename T>
    Printer &format(std::vector<T> &data)
    {
        s << "[";

        for (int i = 0; i < data.size(); ++i)
        {
            Printer::format(data[i]);

            if (i < data.size() - 1)
            {
                s << ", ";
            }
        }

        s << "]";

        return *this;
    }

    template <typename T>
    Printer &format(std::set<T> &data)
    {
        s << "{";

        for (int i = 0; i < data.size(); ++i)
        {
            Printer::format(data[i]);

            if (i < data.size() - 1)
            {
                s << ", ";
            }
        }

        s << "}";

        return *this;
    }
};

template <typename T>
std::string format(const T &t)
{
    return Printer().format(t).str();
}

int main()
{
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int>> v = {{1, 4}, {5, 6}};
    std::string s1 = Printer().format(" vector: ").format(v).str();
    std::cout << s1 << "\n";
    // " vector: [ (1, 4), (5, 6) ]"
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::cout << s2 << "\n";

    std::tuple<std::tuple<int, int>, int> p = {{1, 2}, 3};
    std::string s3 = Printer().format("!!! ").format(p).str();
    std::cout << s3 << "\n";

    // "( xyz, 1, 2 ) ! 0"
}