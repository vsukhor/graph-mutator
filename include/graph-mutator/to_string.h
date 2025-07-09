#ifndef GRAPH_MUTATOR_TO_STRING_H
#define GRAPH_MUTATOR_TO_STRING_H

// https://stackoverflow.com/questions/6713420/c-convert-integer-to-string-at-compile-time?noredirect=1&lq=1

#include <array>
#include <concepts>
#include <cstdint>


namespace graph_mutator {


template<std::intmax_t N>
class to_string_t {

    constexpr static auto buflen() noexcept
    {
        unsigned int len = N > 0 ? 1 : 2;
        for (auto n = N; n; ++len, n /= 10);
        return len;
    }

    char buf[buflen()] = {};

public:
    constexpr to_string_t() noexcept
    {
        auto ptr = buf + buflen();
        *--ptr = '\0';

        if constexpr (N != 0) {
            for (auto n = N; n; n /= 10)
                *--ptr = "0123456789"[(N < 0 ? -1 : 1) * (n % 10)];
            if (N < 0)
                *--ptr = '-';
        } else {
            buf[0] = '0';
        }
    }

    constexpr operator const char *() const { return buf; }
};


template<std::uintmax_t N>
class uint_to_string {

    static consteval auto buflen() noexcept
    {
        unsigned int len = N > 0 ? 1 : 3;

        for (auto n = N; n; ++len, n /= 10);

        return len;
    }

    static consteval auto _buf() noexcept
    {
        std::array<char, buflen()> buf;
        auto ptr = buf.data() + buflen();
        *--ptr = '\0';

        if constexpr (N != 0)
            for (auto n = N; n; n /= 10)
                *--ptr = "0123456789"[n % 10];
        else {
            buf[0] = '0';
            buf[1] = '0';
        }

        return buf;
    }

public:

    static constexpr std::array<char, buflen()> bufa {_buf()};
    static constexpr const char* buf = bufa.data();
};

template<std::uintmax_t N>
constexpr auto str1 = uint_to_string<N>::buf;

template<std::uintmax_t N1,
         std::uintmax_t N2>
constexpr auto str2 = uint_to_string<10*N1 + N2>::buf;

template<int i>
constexpr auto sign_str
{
    i > 0 ? std::array<char, 2> {'+', '0'}
          : i < 0
          ? std::array<char, 2> {'-', '0'}
          : std::array<char, 2> {'0', '0'}
};


template<const char* d, std::size_t N, char... c>
struct ShortName {

    static consteval auto short_name() noexcept
    {
        if constexpr (N == 1)
            return std::array<char, sizeof...(c)+N+1> {{c..., d[0], '\0'}};
        else if constexpr (N == 2)
            return std::array<char, sizeof...(c)+N+1> {{c..., d[0], d[1], '\0'}};
        else if constexpr (N == 3)
            return std::array<char, sizeof...(c)+N+1> {{c..., d[0], d[1], d[2], '\0'}};
        else if constexpr (N == 4)
            return std::array<char, sizeof...(c)+N+1> {{c..., d[0], d[1], d[2], d[3], '\0'}};
        else
            static_assert(false);
    }

    static constexpr auto sna {short_name()};
    static constexpr const char* sn = sna.data();
};

template<const char* d, std::size_t N, char... c>
constexpr auto shName = ShortName<d, N, c...>::sn;


template<const char* d,
         std::size_t N,
         char... c>
struct Concatenate {

    static consteval auto conc() noexcept
    {
        std::array<char, N + sizeof...(c) + 1> a;

        for (std::size_t i {}; i < N; ++i)
            a[i] = d[i];

        std::size_t i {N};
        ((a[i++] = c), ...);

        a.back() = '\0';

        return a;
    }

    static constexpr auto sna {conc()};
    static constexpr const char* bf = sna.data();
};

template<const char* d,
         std::size_t N,
         char... c>
constexpr auto concatenate = Concatenate<d, N, c...>::bf;


template<std::array c,
         const char* d,
         std::size_t N>
struct Concat {

    static consteval auto conc() noexcept
    {
        constexpr auto s = c.size();

        std::array<char, s + N + 1> a;

        for (std::size_t i {}; i < s; ++i)
            a[i] = c[i];

        for (std::size_t i {}; i < N; ++i)
            a[i+s] = d[i];

        a.back() = '\0';

        return a;
    }

    static constexpr auto sna {conc()};
    static constexpr const char* bf = sna.data();
};

template<std::array c,
         const char* d,
         std::size_t N>
constexpr auto concat = Concat<c, d, N>::bf;


}  // namespace graph_mutator

#endif  // GRAPH_MUTATOR_TO_STRING_H
