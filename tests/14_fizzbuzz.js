// FizzBuzz — Loops + Modulo + Conditionals + String Concatenation

function fizzBuzz(n)
{
    let result = [];

    for (let i = 1; i <= n; i++)
    {
        if (i % 15 === 0)
        {
            result.push("FizzBuzz");
        }
        else if (i % 3 === 0)
        {
            result.push("Fizz");
        }
        else if (i % 5 === 0)
        {
            result.push("Buzz");
        }
        else
        {
            result.push(i);
        }
    }

    return result;
}

let output = fizzBuzz(20);
console.log(output.join(", "));

// Count how many of each type
let fizzCount = output.filter(x => x === "Fizz").length;
let buzzCount = output.filter(x => x === "Buzz").length;
let fbCount = output.filter(x => x === "FizzBuzz").length;

console.log("Fizz:", fizzCount);
console.log("Buzz:", buzzCount);
console.log("FizzBuzz:", fbCount);
