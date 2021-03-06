/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <array>
#include <string>

#include "common/flags/parse_flags.h"
#include "common/unit_test_util/unit_test_util.h"

bool ReadFlagTest() {
    int const kArgc = 3;
    std::string program_name = "flag_test";
    std::string arg1 = "--flag_a=101";
    std::string arg2 = "--flag_b=str";
    std::array<char *, kArgc> argv{program_name.data(), arg1.data(), arg2.data()};

    e8::Argv(kArgc, argv.data());

    int flag_a = e8::ReadFlag<int>("flag_a", int(), e8::FromString<int>);
    TEST_CONDITION(flag_a == 101);

    std::string flag_b =
        e8::ReadFlag<std::string>("flag_b", std::string(), e8::FromString<std::string>);
    TEST_CONDITION(flag_b == "str");

    std::string non_existent_flag =
        e8::ReadFlag<std::string>("non_existent_flag", /*default_flag_value=*/"non_existent_value",
                                  e8::FromString<std::string>);
    TEST_CONDITION(non_existent_flag == "non_existent_value");

    return true;
}

int main() {
    e8::BeginTestSuite("flags");
    e8::RunTest("ReadFlagTest", ReadFlagTest);
    e8::EndTestSuite();
    return 0;
}
