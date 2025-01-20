#include <print>
#include <source_location>
#include <glm/vec4.hpp>

#define STRINGIFY(x) #x
#define TO_STR(x) STRINGIFY(x)

using vec4 = glm::vec4;

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

consteval auto enum_to_string(Uniform_t type) {
    return static_string("vec3");
}

consteval auto keyword_to_string(Keyword key) {
    return static_string("uniform");
}
    
constexpr auto space{ static_string{ " " } };
constexpr auto equal{ static_string{ "=" } };
constexpr auto plus{ static_string{ "+" } };
constexpr auto line_end{ static_string{ ";" } };

template <auto n, Uniform_t t>
struct uniform 
{
    static constexpr auto name{ n };
    static constexpr auto type{ t };
    static constexpr auto key{ Keyword::Uniform };
    static constexpr auto declaration{ concat(keyword_to_string(key), space, enum_to_string(type), space, name, line_end) };

    consteval auto operator=(const uniform& u) {
        return concat(name, space, equal, u.name, line_end);
    }
};

template <auto n1, auto n2, Uniform_t t>
consteval auto operator+(const uniform<n1, t>& st, const uniform<n2, t>& nd) {
    return concat(st.name, space, plus, space, nd.name, line_end);
}

auto print(const auto& thing) {
    for (auto e : thing) {
        std::print("{}", e);
    }
    std::println("");
}

auto main() -> int 
{
    constexpr auto ambient_name{ static_string("ambient") };
    constexpr auto ambient{ uniform<ambient_name, Uniform_t::vec3>() };

    constexpr auto diffuse_name{ static_string("diffuse") };
    constexpr auto diffuse{ uniform<diffuse_name, Uniform_t::vec3>() };
    
    constexpr auto res{ ambient + diffuse };

    print(res);
}
