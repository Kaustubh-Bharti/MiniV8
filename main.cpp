#include <iostream>
#include <vector>
#include <string>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/Interpreter.h"

int main()
{
    std::vector<std::string> tests =
    {
R"(
let num = 7;

if (num % 2 === 0)
{
    console.log(num + " is Even");
}
else
{
    console.log(num + " is Odd");
}
)",

R"(
for (let i = 1; i <= 5; i++)
{
    let row = "";

    for (let j = 1; j <= i; j++)
    {
        row += "*";
    }

    console.log(row);
}
)",

R"(
function isArmstrong(num)
{
    let temp = num;
    let sum = 0;

    while (temp > 0)
    {
        let digit = temp % 10;
        sum += digit ** 3;
        temp = Math.floor(temp / 10);
    }

    return sum === num;
}

console.log(isArmstrong(153));
console.log(isArmstrong(123));
)",

R"(
let arr = [1,2,3,4,5];

let reversed = arr.reverse();

console.log(
    reversed.join(",")
);
)",


R"(
let str = "racecar";

let reversed =
    str.split("")
       .reverse()
       .join("");

if (str === reversed)
{
    console.log(
        str +
        " is a Palindrome"
    );
}
else
{
    console.log(
        str +
        " is not a Palindrome"
    );
}
)"

    };

    for (
        size_t i = 0;
        i < tests.size();
        i++
    )
    {
        std::cout
            << "\n===== TEST "
            << i + 1
            << " =====\n"
            << std::endl;

        Lexer lexer(
            tests[i]
        );

        auto tokens =
            lexer.tokenize();

        Parser parser(
            tokens
        );

        auto program =
            parser.parseProgram();

        Interpreter interpreter;

        interpreter.executeProgram(
            program.get()
        );
    }

    return 0;
}