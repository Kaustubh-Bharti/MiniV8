#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    std::cout << "===== TEST 1 =====" << std::endl;

    int num = 7;

    if (num % 2 == 0)
    {
        std::cout << num << " is Even" << std::endl;
    }
    else
    {
        std::cout << num << " is Odd" << std::endl;
    }

    std::cout << std::endl;

    std::cout << "===== TEST 2 =====" << std::endl;

    for (int i = 1; i <= 5; i++)
    {
        std::string row = "";

        for (int j = 1; j <= i; j++)
        {
            row += "*";
        }

        std::cout << row << std::endl;
    }

    std::cout << std::endl;

    std::cout << "===== TEST 3 =====" << std::endl;

    auto isArmstrong =
        [](int num)
        {
            int temp = num;
            int sum = 0;

            while (temp > 0)
            {
                int digit =
                    temp % 10;

                sum +=
                    digit * digit * digit;

                temp =
                    static_cast<int>(
                        std::floor(
                            temp / 10.0
                        )
                    );
            }

            return sum == num;
        };

    std::cout
        << isArmstrong(153)
        << std::endl;

    std::cout
        << isArmstrong(123)
        << std::endl;

    std::cout << std::endl;

    std::cout << "===== TEST 4 =====" << std::endl;

    std::vector<int> arr =
    {
        1, 2, 3, 4, 5
    };

    std::vector<int> reversed =
        arr;

    std::reverse(
        reversed.begin(),
        reversed.end()
    );

    std::cout
        << "Original: ";

    for (size_t i = 0;
         i < arr.size();
         i++)
    {
        if (i)
        {
            std::cout << ", ";
        }

        std::cout
            << arr[i];
    }

    std::cout
        << std::endl;

    std::cout
        << "Reversed: ";

    for (size_t i = 0;
         i < reversed.size();
         i++)
    {
        if (i)
        {
            std::cout << ", ";
        }

        std::cout
            << reversed[i];
    }

    std::cout
        << std::endl;

    std::cout << std::endl;

    std::cout << "===== TEST 5 =====" << std::endl;

    std::string str =
        "racecar";

    std::string reversedStr =
        str;

    std::reverse(
        reversedStr.begin(),
        reversedStr.end()
    );

    if (str == reversedStr)
    {
        std::cout
            << str
            << " is a Palindrome"
            << std::endl;
    }
    else
    {
        std::cout
            << str
            << " is not a Palindrome"
            << std::endl;
    }

    return 0;
}