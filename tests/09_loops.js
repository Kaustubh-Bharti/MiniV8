// For loop
console.log("For loop:");
for (let i = 0; i < 5; i++)
{
    console.log(i);
}

// While loop
console.log("While loop:");
let count = 3;
while (count > 0)
{
    console.log(count);
    count--;
}

// Do-while loop
console.log("Do-while loop:");
let n = 1;
do
{
    console.log(n);
    n++;
} while (n <= 3);

// Break
console.log("Break:");
for (let i = 0; i < 10; i++)
{
    if (i === 5)
    {
        break;
    }
    console.log(i);
}

// Continue
console.log("Continue:");
for (let i = 0; i < 6; i++)
{
    if (i === 3)
    {
        continue;
    }
    console.log(i);
}
