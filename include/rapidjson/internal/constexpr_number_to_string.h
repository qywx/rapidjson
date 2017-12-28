// Got from https://stackoverflow.com/questions/23999573/convert-a-number-to-a-string-literal-with-constexpr/24000041#24000041

namespace detail
{
    template<unsigned... digits>
    struct to_chars { static const char value[]; };

    template<unsigned... digits>
    constexpr char to_chars<digits...>::value[] = {('0' + digits)..., '\0'};

    template<unsigned rem, unsigned... digits>
    struct explode : explode<rem / 10, rem % 10, digits...> {};

    template<unsigned... digits>
    struct explode<0, digits...> : to_chars<digits...> {};
}


template<unsigned num>
struct num_to_string : detail::explode<num> {};


/*namespace detail
{
    template<uint8_t... digits> struct positive_to_chars { static const char value[]; };
    template<uint8_t... digits> constexpr char positive_to_chars<digits...>::value[] = {('0' + digits)..., '\0'};

    template<uint8_t... digits> struct negative_to_chars { static const char value[]; };
    template<uint8_t... digits> constexpr char negative_to_chars<digits...>::value[] = {'-', ('0' + digits)..., '\0'};

    template<bool neg, uint8_t... digits>
    struct to_chars : positive_to_chars<digits...> {};

    template<uint8_t... digits>
    struct to_chars<true, digits...> : negative_to_chars<digits...> {};

    template<bool neg, uintmax_t rem, uint8_t... digits>
    struct explode : explode<neg, rem / 10, rem % 10, digits...> {};

    template<bool neg, uint8_t... digits>
    struct explode<neg, 0, digits...> : to_chars<neg, digits...> {};

    template<typename T>
    constexpr uintmax_t cabs(T num) { return (num < 0) ? -num : num; }
}

template<typename Integer, Integer num>
struct string_from : detail::explode<(num < 0), detail::cabs(num)> {};*/