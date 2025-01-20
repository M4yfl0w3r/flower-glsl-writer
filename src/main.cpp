#include <print>
#include <type_traits>
#include <magic_enum/magic_enum.hpp>

#define STRINGIFY(x) #x
#define TO_STR(x) STRINGIFY(x)

enum class Uniform_t
{
    vec3
};

enum class Keyword 
{
    Uniform
};

template <std::size_t N>
struct static_string 
{
    static constexpr int size = N - 1;

    consteval static_string(const char (&str)[N])  {
        std::copy_n(str, N, value);
    }

    consteval static_string(const std::array<char, N>& str) {
        std::copy_n(std::begin(str), N, value);
    }

    consteval auto data() const {
        return std::string_view{ value };
    }

    constexpr auto begin() const { return value; }
    constexpr auto end() const { return value + size; }

    char value[N] {};
};

// constexpr auto space{ static_string{ " " } };
// constexpr auto equal{ static_string{ "=" } };
// constexpr auto plus{ static_string{ "+" } };
// constexpr auto line_end{ static_string{ ";" } };

template <std::size_t... len>
consteval auto concat(const static_string<len>&... strings)
{
    constexpr std::size_t N{ (... + len) - sizeof...(len) };
    std::array<char, N+1> result{};
    std::size_t index{ 0 };
    ([&] {
        const auto sv{ strings.data() };
        std::copy(std::begin(sv), std::end(sv), std::begin(result) + index);
        index += std::size(sv);
    }(), ...);

    result[N] = '\0';
    return static_string{ result };
}

consteval auto keyword_to_string(Keyword key) {
    return static_string("uniform");
}

auto print(const auto& thing) {
    for (auto e : thing) {
        std::print("{}", e);
    }
    std::println("");
}

template <typename T> requires std::is_enum_v<T>
auto to_static_string(T enum_value) {
    return static_string{ magic_enum::enum_name(enum_value) };
}

// template <auto static_name, Uniform_t t>
// struct uniform 
// {
//     static constexpr auto name{ static_name };
//     static constexpr auto type{ t };
//     static constexpr auto key{ Keyword::Uniform };
//     static constexpr auto init_expression{ concat(to_static_string(key), space, to_static_string(type), space, name, line_end) };
// };
//
// template <auto n1, auto n2, Uniform_t t>
// consteval auto add(const uniform<n1, t>& st, const uniform<n2, t>& nd) {
//     return concat(st.name, space, plus, space, nd.name, line_end);
// }
//
// template <auto n1, auto n2, Uniform_t t>
// consteval auto assign(const uniform<n1, t>& dest, const uniform<n2, t>& src) {
//     return concat(dest.name, space, equal, space, src.name, line_end);
// }
//
// template <auto n1, auto N, Uniform_t t>
// consteval auto assign(const uniform<n1, t>& dest, const static_string<N>& src) {
//     return concat(dest.name, space, equal, space, src);
// }

auto main() -> int 
{
    // constexpr auto ambient_name{ static_string{ "ambient" } };
    // constexpr auto ambient{ uniform<ambient_name, Uniform_t::vec3>() };
    //
    // constexpr auto diffuse_name{ static_string{ "diffuse" } };
    // constexpr auto diffuse{ uniform<diffuse_name, Uniform_t::vec3>() };
    //
    // constexpr auto color_name{ static_string{ "color" } };
    // constexpr auto color{ uniform<color_name, Uniform_t::vec3>() };
    //
    // constexpr auto add_res{ add(ambient, diffuse) };
    // constexpr auto ass_res{ assign(color, diffuse) };
    //
    // constexpr auto test{ assign(color, add_res) };
    //
    // print(add_res);
    // print(ass_res);
    // print(test);

    std::println("{}", to_static_string(Keyword::Uniform));
}
