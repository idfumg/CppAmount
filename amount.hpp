#pragma once

#include <proxy_containers.hpp>

#include <cmath>
#include <iomanip>

template<std::size_t FRACTION_SIZE>
class Amount {
    using string_view = proxy_containers::string_view;

public:
    constexpr Amount(const std::int64_t fixedpoint) noexcept
        : m_fixedpoint{fixedpoint}
    {

    }

    constexpr Amount(const proxy_containers::string_view& str) noexcept
        : m_fixedpoint{ParseMoney(IsNegative(str) ? str.substr(1) : str) * (IsNegative(str) ? -1 : 1)}
    {

    }

public:
    static constexpr Amount fromTwoDigitsFraction(const std::int64_t value) noexcept
    {
        return Amount(value * 100);
    }

    constexpr std::int64_t fixedpoint() const noexcept
    {
        return m_fixedpoint;
    }

    constexpr bool operator==(const Amount& right) const noexcept
    {
        return m_fixedpoint == right.m_fixedpoint;
    }

    constexpr bool operator!=(const Amount& right) const noexcept
    {
        return m_fixedpoint != right.m_fixedpoint;
    }

    constexpr bool operator<(const Amount& right) const noexcept
    {
        return m_fixedpoint - right.m_fixedpoint < 0;
    };

    constexpr bool operator<=(const Amount& right) const noexcept
    {
        return *this < right or *this == right;
    };

    constexpr bool operator>(const Amount& right) const noexcept
    {
        return not (*this < right);
    };

    constexpr bool operator>=(const Amount& right) const noexcept
    {
        return *this > right or *this == right;
    };

    constexpr Amount& operator+=(const Amount& right) noexcept
    {
        m_fixedpoint += right.m_fixedpoint; return *this;
    }

    constexpr Amount& operator-=(const Amount& right) noexcept
    {
        m_fixedpoint -= right.m_fixedpoint; return *this;
    }

    constexpr Amount operator+(const Amount& amount) const noexcept
    {
        return Amount{m_fixedpoint + amount.m_fixedpoint};
    };

    constexpr Amount operator-(const Amount& amount) const noexcept
    {
        return Amount{m_fixedpoint - amount.m_fixedpoint};
    };

    constexpr Amount operator*(const Amount& amount) const noexcept
    {
        return Amount{MulMoney(m_fixedpoint, amount.m_fixedpoint)};
    }

    constexpr Amount operator/(const Amount& amount) const noexcept
    {
        return Amount{DivMoney(m_fixedpoint, amount.m_fixedpoint)};
    }

    constexpr Amount operator-() const noexcept
    {
        return Amount(-m_fixedpoint);
    }

    constexpr Amount roundFraction(const std::int64_t digit_position) const noexcept
    {
        return Amount(RoundNumberFraction(m_fixedpoint, digit_position));
    }

    std::string fixedpointStr() const noexcept
    {
        return std::to_string(m_fixedpoint);
    }

    std::string str() const noexcept
    {
        const auto cents = GetCents(m_fixedpoint);

        std::ostringstream out;
        out << GetUnits(m_fixedpoint) << '.' << centsToString(cents);
        return out.str();
    }

    std::string noFractionalIfPossibleStr() const noexcept
    {
        const auto cents = GetCents(m_fixedpoint);

        std::ostringstream out;
        out << GetUnits(m_fixedpoint);
        if (cents > 0) {
            out << '.' << centsToString(cents);
        }
        return out.str();
    }

    std::string centsToString(const std::size_t cents) const noexcept
    {
        std::ostringstream out;
        out << std::setw(FRACTION_SIZE) << std::setfill('0') << cents;
        return out.str();
    }

public:
    static constexpr int GetDigitsCount(std::int64_t value) noexcept
    {
        int count = 0;
        while (value > 0) {
            value /= 10;
            ++count;
        }
        return count;
    }

    template<class T>
    static constexpr T abs(const T param) noexcept
    {
        return param > 0 ? param : -param;
    }

    static constexpr std::int64_t GetFractionDelimiter(
        const std::int64_t fraction_size) noexcept
    {
        return std::pow(10, fraction_size);
    }

    static constexpr std::int64_t GetFractionDelimiter() noexcept
    {
        return GetFractionDelimiter(FRACTION_SIZE);
    }

    static constexpr std::int64_t GetUnits(const std::int64_t value) noexcept
    {
        return value / GetFractionDelimiter(FRACTION_SIZE);
    }

    static constexpr std::int64_t GetCents(const std::int64_t value) noexcept
    {
        return abs(value % GetFractionDelimiter(FRACTION_SIZE));
    }

    static constexpr std::int64_t AddMissingZeros(
        const std::int64_t value,
        const std::int64_t digits_count) noexcept
    {
        const auto value_digits_count = GetDigitsCount(value);
        const auto multiplier = value_digits_count > digits_count
            ? 1
            : std::pow(10, digits_count - value_digits_count);

        return value * multiplier;
    }

    static constexpr std::int64_t AddMissingZeros(
        const std::int64_t value) noexcept
    {
        return AddMissingZeros(value, FRACTION_SIZE);
    }

    static constexpr std::int64_t Round(
        const double param,
        const std::int64_t digit_position,
        const std::int64_t fraction_size) noexcept
    {
        const auto number_digits = GetDigitsCount(param);
        if (number_digits < digit_position) {
            if (fraction_size > digit_position) {
                return 0;
            }
            return param;
        }

        const auto digits_to_cut =
            number_digits >= fraction_size ? number_digits - digit_position : std::min<int>(number_digits, digit_position);

        const auto number_for_round = static_cast<double>(param) / GetFractionDelimiter(digits_to_cut);
        const auto rounded = std::roundf(number_for_round);

        const auto i = fraction_size != 0 ? digits_to_cut : 0;
        auto result = static_cast<std::int64_t>(rounded);

        if (GetDigitsCount(result) < fraction_size)
            result *= std::pow(10, i);

        while (fraction_size != 0 and GetDigitsCount(result) > fraction_size) {
            result /= 10;
        }

        return result;
    }

    static constexpr std::int64_t Round(
        const double param,
        const std::int64_t digit_position) noexcept
    {
        return Round(param, digit_position, 0);
    }

    static constexpr std::int64_t RoundFraction(
        const std::int64_t fraction,
        const std::int64_t digit_position,
        const std::int64_t fraction_size) noexcept
    {
        return Round(fraction, digit_position, fraction_size);
    }

    static constexpr std::int64_t RoundFraction(
        const std::int64_t fraction,
        const std::int64_t digit_position) noexcept
    {
        return RoundFraction(fraction, digit_position, FRACTION_SIZE);
    }

    static constexpr bool IsAllDigitsIsNines(
        std::int64_t x) noexcept
    {
        if (x <= 0) {
            return false;
        }

        while (x > 0) {
            if (x % 10 != 9) {
                return false;
            }
            x /= 10;
        }

        return true;
    }

    static constexpr std::int64_t GetRoundedFractionPart(
        const std::int64_t cents,
        const std::int64_t digit_position,
        const std::int64_t fraction_size) noexcept
    {
        const auto count = GetDigitsCount(cents) - digit_position;
        const auto howMuchWeWantToErase = std::pow(10, count > 0 ? count : 0);
        const auto erasedFraction = cents / howMuchWeWantToErase;

        std::int64_t rounded{};

        if (IsAllDigitsIsNines(erasedFraction)) {
            rounded = erasedFraction;
            rounded *= howMuchWeWantToErase;
        }
        else {
            rounded = RoundFraction(cents, digit_position, fraction_size);
        }

        return rounded;
    }

    static constexpr std::int64_t RoundNumberFraction(
        const std::int64_t number,
        const std::int64_t digit_position,
        const std::int64_t fraction_size) noexcept
    {
        const auto number_with_fraction = static_cast<double>(number) / GetFractionDelimiter(fraction_size);
        const auto number_without_fraction = static_cast<std::int64_t>(number_with_fraction);
        const auto units = number_without_fraction * GetFractionDelimiter(fraction_size);
        const auto cents = number - units;
        const auto roundedFraction = GetRoundedFractionPart(cents, digit_position, fraction_size);
        const auto result = units + roundedFraction;
        return result;
    }

    static constexpr std::int64_t RoundNumberFraction(
        const std::int64_t value,
        const std::int64_t digit_position) noexcept
    {
        return RoundNumberFraction(value, digit_position, FRACTION_SIZE);
    }

    static constexpr std::int64_t MulMoney(
        const std::int64_t param1,
        const std::int64_t param2) noexcept
    {
        const auto product = param1 * param2;
        const auto delimiter = GetFractionDelimiter();
        const auto units = product / (delimiter * delimiter);
        const auto cents = static_cast<double>(product % (delimiter * delimiter)) / delimiter;

        return units * delimiter + Round(cents, FRACTION_SIZE);
    }

    static constexpr std::int64_t DivMoney(
        const std::int64_t param1,
        const std::int64_t param2) noexcept
    {
        const auto delimiter = GetFractionDelimiter();
        const auto units = param1 / param2;
        const auto cents = static_cast<double>(param1 % param2) / param2 * delimiter;

        return units * delimiter + Round(cents, FRACTION_SIZE);
    }

    static constexpr std::int64_t StringToSize(
        const string_view& value) noexcept
    {
        return value.to_uint();
    }

    static constexpr std::size_t FindDelimiterPosition(
        const string_view& value) noexcept
    {
        if (const auto found = value.find(".")) {
            if (found != string_view::npos) {
                return found;
            }
        }

        if (const auto found = value.find(",")) {
            if (found != string_view::npos) {
                return found;
            }
        }

        if (const auto found = value.find("`")) {
            if (found != string_view::npos) {
                return found;
            }
        }

        return string_view::npos;
    }

    static constexpr string_view ParseMoneyUnitsString(
        const string_view& value) noexcept
    {
        const auto found = FindDelimiterPosition(value);

        if (found == string_view::npos) {
            return value;
        }

        if (found == 0) {
            return "0";
        }

        return value.substr(0, found);
    }

    static constexpr std::int64_t ParseMoneyUnits(
        const string_view& value) noexcept
    {
        return StringToSize(ParseMoneyUnitsString(value));
    }

    static constexpr string_view ParseMoneyCentsString(
        const string_view& value,
        const std::int64_t fraction_size) noexcept
    {
        const auto found = FindDelimiterPosition(value);
        if (found == std::string::npos) {
            return "0";
        }
        return value.substr(found + 1);
    }

    static constexpr std::size_t CountZeroes(
        const string_view& value,
        const std::size_t i,
        const std::size_t count) noexcept
    {
        return (i < value.size() and value[i] == '0')
            ? CountZeroes(value, i + 1, count + 1)
            : count;
    }

    static constexpr std::size_t CountZeroes(
        const string_view& value) noexcept
    {
        return CountZeroes(value, 0, 0);
    }

    static constexpr std::int64_t ParseMoneyCents(
        const string_view& value,
        const std::int64_t fraction_size,
        const std::int64_t round_digits) noexcept
    {
        const auto cents_string = ParseMoneyCentsString(value, fraction_size);

        return AddMissingZeros(
            RoundFraction(cents_string.to_uint(), round_digits),
            fraction_size - CountZeroes(cents_string));
    }

    static constexpr std::int64_t ParseMoneyCents(
        const string_view& value) noexcept
    {
        return ParseMoneyCents(value, FRACTION_SIZE, FRACTION_SIZE);
    }

    static constexpr std::int64_t ParseMoney(
        const string_view& value,
        const std::int64_t fraction_size,
        const std::int64_t round_digits) noexcept
    {
        const auto units = ParseMoneyUnits(value);
        const auto cents = ParseMoneyCents(value, fraction_size, round_digits);

        return units * GetFractionDelimiter(fraction_size) + cents;
    }

    static constexpr std::int64_t ParseMoney(
        const string_view& value) noexcept
    {
        return ParseMoney(value, FRACTION_SIZE, FRACTION_SIZE);
    }

    static constexpr bool IsNegative(const string_view& value) noexcept
    {
        return not value.empty() and value[0] == '-';
    }

private:
    std::int64_t m_fixedpoint;
};
