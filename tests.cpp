#include <iostream>
#include <cassert>
#include <vector>

#include "proxy_containers.hpp"
#include "amount.hpp"

using namespace proxy_containers;


void main() {
    using Amount4 = Amount<4>;
    using string_view = proxy_containers::string_view;

    static_assert(Amount4::GetDigitsCount(1) == 1);
    static_assert(Amount4::GetDigitsCount(11) == 2);
    static_assert(Amount4::GetDigitsCount(112) == 3);
    static_assert(Amount4::GetDigitsCount(1123) == 4);

    static_assert(Amount4::GetFractionDelimiter(4) == 10000);
    static_assert(Amount4::GetFractionDelimiter() == 10000);

    static_assert(Amount4::AddMissingZeros(1, 4) == 1000);
    static_assert(Amount4::AddMissingZeros(11, 4) == 1100);
    static_assert(Amount4::AddMissingZeros(111, 4) == 1110);
    static_assert(Amount4::AddMissingZeros(1111, 4) == 1111);

    static_assert(Amount4::AddMissingZeros(1) == 1000);
    static_assert(Amount4::AddMissingZeros(11) == 1100);
    static_assert(Amount4::AddMissingZeros(111) == 1110);
    static_assert(Amount4::AddMissingZeros(1111) == 1111);

    static_assert(Amount4::Round(125673, 4) == 1257);
    static_assert(Amount4::Round(12567, 4) == 1257);
    static_assert(Amount4::Round(1256, 4) == 1256);
    static_assert(Amount4::Round(1256, 3) == 126);
    static_assert(Amount4::Round(1256, 2) == 13);
    static_assert(Amount4::Round(1256, 1) == 1);
    static_assert(Amount4::Round(12345678, 8) == 12345678);
    static_assert(Amount4::Round(12345678, 7) == 1234568);
    static_assert(Amount4::Round(12345678, 6) == 123457);
    static_assert(Amount4::Round(12345678, 5) == 12346);
    static_assert(Amount4::Round(12345678, 4) == 1235);
    static_assert(Amount4::Round(12345678, 3) == 123);
    static_assert(Amount4::Round(12345678, 2) == 12);
    static_assert(Amount4::Round(12345678, 1) == 1);

    static_assert(Amount4::RoundNumberFraction(12569867, 4, 4) == 12569867);
    static_assert(Amount4::RoundNumberFraction(12569867, 3, 4) == 12569870);
    static_assert(Amount4::RoundNumberFraction(12569867, 2, 4) == 12569900);
    static_assert(Amount4::RoundNumberFraction(12569867, 2, 4) == 12569900);
    static_assert(Amount4::RoundNumberFraction(43109989, 2, 4) == 43109900);
    static_assert(Amount4::RoundNumberFraction(43109989, 3, 4) == 43109990);
    static_assert(Amount4::RoundNumberFraction(43109989, 4, 4) == 43109989);
    static_assert(Amount4::RoundNumberFraction(43109900, 2, 4) == 43109900);
    static_assert(Amount4::RoundNumberFraction(43109990, 2, 4) == 43109900);
    static_assert(Amount4::RoundNumberFraction(43109990, 3, 4) == 43109990);
    static_assert(Amount4::RoundNumberFraction(43109999, 4, 4) == 43109999);
    static_assert(Amount4::RoundNumberFraction(43109999, 3, 4) == 43109990);
    static_assert(Amount4::RoundNumberFraction(43109999, 2, 4) == 43109900);
    static_assert(Amount4::RoundNumberFraction(43109999, 1, 4) == 43109000);

    static_assert(Amount4::RoundNumberFraction(12569867, 4) == 12569867);
    static_assert(Amount4::RoundNumberFraction(12569867, 3) == 12569870);
    static_assert(Amount4::RoundNumberFraction(12569867, 2) == 12569900);
    static_assert(Amount4::RoundNumberFraction(12569867, 2) == 12569900);

    static_assert(Amount4::GetDigitsCount(123456) == 6);

    static_assert(Amount4::GetFractionDelimiter(1) == 10);
    static_assert(Amount4::GetFractionDelimiter(2) == 100);
    static_assert(Amount4::GetFractionDelimiter(3) == 1000);
    static_assert(Amount4::GetFractionDelimiter(4) == 10000);
    static_assert(Amount4::GetFractionDelimiter() == 10000);

    static_assert(Amount4::GetUnits(123456) == 12);
    static_assert(Amount4::GetCents(123456) == 3456);

    static_assert(Amount4::AddMissingZeros(123456, 4) == 123456);
    static_assert(Amount4::AddMissingZeros(12345, 6) == 123450);
    static_assert(Amount4::AddMissingZeros(1234, 6) == 123400);
    static_assert(Amount4::AddMissingZeros(123, 6) == 123000);
    static_assert(Amount4::AddMissingZeros(12, 6) == 120000);
    static_assert(Amount4::AddMissingZeros(1, 6) == 100000);
    static_assert(Amount4::AddMissingZeros(0, 6) == 0);
    static_assert(Amount4::AddMissingZeros(123) == 1230);

    static_assert(Amount4::Round(123, 3) == 123);
    static_assert(Amount4::Round(1230, 3) == 123);
    static_assert(Amount4::Round(1234, 3) == 123);
    static_assert(Amount4::Round(1235, 3) == 124);
    static_assert(Amount4::Round(1239, 3) == 124);
    static_assert(Amount4::Round(12, 2) == 12);
    static_assert(Amount4::Round(123, 2) != 123);

    static_assert(Amount4::RoundFraction(9867, 4) == 9867);
    static_assert(Amount4::RoundFraction(9867, 3) == 9870);
    static_assert(Amount4::RoundFraction(9867, 2) == 9900);
    static_assert(Amount4::RoundFraction(12345678, 8, 8) == 12345678);
    static_assert(Amount4::RoundFraction(12345678, 7, 8) == 12345680);
    static_assert(Amount4::RoundFraction(12345678, 6, 8) == 12345700);
    static_assert(Amount4::RoundFraction(12345678, 5, 8) == 12346000);
    static_assert(Amount4::RoundFraction(12345678, 4, 8) == 12350000);
    static_assert(Amount4::RoundFraction(12345678, 3, 8) == 12300000);
    static_assert(Amount4::RoundFraction(12345678, 2, 8) == 12000000);
    static_assert(Amount4::RoundFraction(12345678, 1, 8) == 10000000);
    static_assert(Amount4::RoundFraction(12345678, 8, 6) == 123456);
    static_assert(Amount4::RoundFraction(12345678, 7, 6) == 123456);
    static_assert(Amount4::RoundFraction(12345678, 6, 6) == 123457);
    static_assert(Amount4::RoundFraction(12345678, 5, 6) == 123460);
    static_assert(Amount4::RoundFraction(12345678, 4, 6) == 123500);
    static_assert(Amount4::RoundFraction(12345678, 3, 6) == 123000);
    static_assert(Amount4::RoundFraction(12345678, 2, 6) == 120000);
    static_assert(Amount4::RoundFraction(12345678, 1, 6) == 100000);
    static_assert(Amount4::RoundFraction(12345678, 7, 7) == 1234568);
    static_assert(Amount4::RoundFraction(12345678, 6, 6) == 123457);
    static_assert(Amount4::RoundFraction(12345678, 5, 5) == 12346);
    static_assert(Amount4::RoundFraction(12345678, 4, 4) == 1235);
    static_assert(Amount4::RoundFraction(12345678, 3, 3) == 123);
    static_assert(Amount4::RoundFraction(12345678, 2, 2) == 12);
    static_assert(Amount4::RoundFraction(12345678, 1, 1) == 1);

    static_assert(Amount4::DivMoney(640000, 80000) == 80000);
    static_assert(Amount4::DivMoney(64000, 8000) == 80000);
    static_assert(Amount4::DivMoney(6400, 800) == 80000);
    static_assert(Amount4::DivMoney(640, 80) == 80000);
    static_assert(Amount4::DivMoney(64, 8) == 80000);

    static_assert(Amount4::IsAllDigitsIsNines(-1) == false);
    static_assert(Amount4::IsAllDigitsIsNines(0) == false);
    static_assert(Amount4::IsAllDigitsIsNines(123) == false);
    static_assert(Amount4::IsAllDigitsIsNines(923) == false);
    static_assert(Amount4::IsAllDigitsIsNines(993) == false);
    static_assert(Amount4::IsAllDigitsIsNines(999) == true);
    static_assert(Amount4::IsAllDigitsIsNines(99) == true);
    static_assert(Amount4::IsAllDigitsIsNines(9) == true);

    static_assert(string_view{"1"}.size() == 1);
    static_assert(string_view{"11"}.size() == 2);
    static_assert(string_view{"0"}[0] == '0');
    static_assert(string_view{"0"}.to_uint() == 0);
    static_assert(string_view{"1"}.to_uint() == 1);
    static_assert(string_view{"9"}.to_uint() == 9);
    static_assert(string_view{"11"}.to_uint() == 11);
    static_assert(string_view{"123"}.to_uint() == 123);

    static_assert(string_view{"123.45"}.find(".") == 3);
    static_assert(string_view{"123.45"}.find(";") == string_view::npos);

    static_assert(Amount4::StringToSize("44") == 44);
    static_assert(Amount4::StringToSize("0044") == 44);

    static_assert(Amount4::FindDelimiterPosition("696.9867") == 3);

    static_assert(Amount4::ParseMoneyUnits("696.9867") == 696);

    static_assert(Amount4::ParseMoneyCentsString("696.98676", 4) == "98676");

    static_assert(Amount4::ParseMoneyCents("696.98676") == 9868);
    static_assert(Amount4::ParseMoneyCents("696.9867") == 9867);
    static_assert(Amount4::ParseMoneyCents("1400.0044") == 44);

    static_assert(Amount4::ParseMoney("696.9867") == 6969867);
    static_assert(Amount4::ParseMoney("696.98676") == 6969868);

    static_assert(Amount4::ParseMoney("696.9867", 4, 4) == 6969867);
    static_assert(Amount4::ParseMoney("696.98676", 4, 4) == 6969868);
    static_assert(Amount4::ParseMoney("696.98676", 4, 2) == 6969900);

    static_assert(Amount4("48.819") == Amount4("48.8190"));
    static_assert(Amount4("48.81996") == Amount4("48.8200"));
    static_assert(Amount4("48.81996") == Amount4("48.820"));
    static_assert(Amount4("48.81996") == Amount4("48.82"));
    static_assert(Amount4("48.8156") == Amount4("48.8156"));
    static_assert(Amount4("48.81566") == Amount4("48.8157"));
    static_assert(Amount4("48.81565") == Amount4("48.8157"));
    static_assert(Amount4("48.81564") == Amount4("48.8156"));
    static_assert(Amount4("48.81") == Amount4("48.81"));
    static_assert(Amount4("118") == Amount4("118"));

    static_assert(Amount4::CountZeroes("1") == 0);
    static_assert(Amount4::CountZeroes("10") == 0);
    static_assert(Amount4::CountZeroes("100") == 0);
    static_assert(Amount4::CountZeroes("010") == 1);
    static_assert(Amount4::CountZeroes("0010") == 2);

    static_assert(Amount4::fromTwoDigitsFraction(12345) == Amount4("123.4500"));
    static_assert(Amount4::fromTwoDigitsFraction(12345) == Amount4("123.450"));
    static_assert(Amount4::fromTwoDigitsFraction(12345) == Amount4("123.45"));

    static_assert(Amount4("48.81").fixedpoint() == 488100);
    static_assert(Amount4("118").fixedpoint() == 1180000);
    static_assert(Amount4("48.8156").fixedpoint() == 488156);

    static_assert(Amount4("48.81") * Amount4("118") == Amount4("5759.58"));
    static_assert(Amount4("5759.58") / Amount4("18") == Amount4("319.9767"));
    static_assert(Amount4("319.9767") * Amount4("18.0") / Amount4("118.0") == Amount4("48.8100"));

    static_assert(Amount4::ParseMoneyCentsString("0.08", 4) == "08");
    static_assert(Amount4::StringToSize("08") == 8);
    static_assert(string_view("08").size() == 2);
    static_assert(Amount4::GetDigitsCount(8) == 1);

    static_assert(Amount4::AddMissingZeros(8000, 4) == 8000);
    static_assert(Amount4::AddMissingZeros(800, 4) == 8000);
    static_assert(Amount4::AddMissingZeros(80, 4) == 8000);
    static_assert(Amount4::AddMissingZeros(8, 4) == 8000);

    static_assert(Amount4::RoundFraction(8, 4) == 8);
    static_assert(Amount4::RoundFraction(80, 4) == 80);
    static_assert(Amount4::RoundFraction(800, 4) == 800);
    static_assert(Amount4::RoundFraction(8000, 4) == 8000);
    static_assert(Amount4::RoundFraction(80000, 4) == 8000);

    static_assert(Amount4::ParseMoney("0.0800", 4, 4) == 800);
    static_assert(Amount4::ParseMoney("0.080", 4, 4) == 800);
    static_assert(Amount4::ParseMoney("0.08", 4, 4) == 800);

    static_assert(Amount4("213.56") * Amount4("118") == Amount4("25200.08"));
    static_assert(Amount4("213.56") * Amount4("118") / Amount4("18") == Amount4("1400.0044"));
    static_assert(Amount4("1400.0044") * Amount4("18") / Amount4("118") == Amount4("213.56"));
    static_assert(Amount4("106.32") * Amount4("118") / Amount4("18") == Amount4("696.9867"));
    static_assert(Amount4("696.9867").roundFraction(2) == Amount4("696.99"));
    static_assert(Amount4("106.32") * Amount4("118") / Amount4("18") == Amount4("696.9867"));

    static_assert(Amount4("1176.0001").roundFraction(2) == Amount4("1176.00"));
    static_assert(Amount4("1176.0011").roundFraction(2) == Amount4("1176.00"));
    static_assert(Amount4("1176.0111").roundFraction(2) == Amount4("1176.01"));
    static_assert(Amount4("1176.1111").roundFraction(2) == Amount4("1176.11"));
    static_assert(Amount4("1176.1191").roundFraction(2) == Amount4("1176.12"));
    static_assert(Amount4("1176.1991").roundFraction(2) == Amount4("1176.20"));
    static_assert(Amount4("1176.1119").roundFraction(2) == Amount4("1176.11"));
    static_assert(Amount4("1176.1000").roundFraction(2) == Amount4("1176.10"));
    static_assert(Amount4("1176.1100").roundFraction(2) == Amount4("1176.11"));
    static_assert(Amount4("1176.1110").roundFraction(2) == Amount4("1176.11"));
    static_assert(Amount4("1176.1111").roundFraction(2) == Amount4("1176.11"));
    static_assert(Amount4("1176.1009").roundFraction(2) == Amount4("1176.10"));
    static_assert(Amount4("1176.1019").roundFraction(2) == Amount4("1176.10"));
    static_assert(Amount4("1176.1099").roundFraction(2) == Amount4("1176.11"));

    static_assert(Amount4("0") == Amount4("0"));
    static_assert(Amount4("0.0") == Amount4("0"));

    static_assert(Amount4::IsNegative("-1") == true);
    static_assert(Amount4("-1").fixedpoint() == -10000);
    static_assert(Amount4("-1") == Amount4("-1.0000"));
    static_assert(Amount4("-1.123") == Amount4("-1.1230"));

    return 0;
}
