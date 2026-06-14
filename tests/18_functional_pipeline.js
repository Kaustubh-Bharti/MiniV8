// Functional Pipeline — Map/Filter/Reduce chaining + Math + Callbacks

let data = [12, 45, 7, 89, 23, 56, 3, 78, 34, 91, 15, 67, 42, 8, 100];

// Pipeline: filter evens -> square them -> sum
let evenSquareSum = data
    .filter(x => x % 2 === 0)
    .map(x => x * x)
    .reduce((sum, x) => sum + x, 0);
console.log("Sum of squared evens:", evenSquareSum);

// Pipeline: filter > 30 -> sqrt -> round
let sqrtOfLarge = data
    .filter(x => x > 30)
    .map(x => Math.round(Math.sqrt(x) * 100) / 100);
console.log("Sqrt of large:", sqrtOfLarge.join(", "));

// Custom higher-order function
function transform(arr, filterFn, mapFn)
{
    return arr.filter(filterFn).map(mapFn);
}

let result1 = transform(
    data,
    x => x > 50,
    x => x - 50
);
console.log("Above 50, offset:", result1.join(", "));

let result2 = transform(
    data,
    x => x < 20,
    x => x * 10
);
console.log("Below 20, x10:", result2.join(", "));

// Compose operations manually
function pipe(value, fn1, fn2, fn3)
{
    let r1 = fn1(value);
    let r2 = fn2(r1);
    return fn3(r2);
}

let piped = pipe(
    data,
    arr => arr.filter(x => x % 2 !== 0),
    arr => arr.map(x => x + 1),
    arr => arr.reduce((sum, x) => sum + x, 0)
);
console.log("Piped odds+1 sum:", piped);

// Running statistics
let count = data.length;
let sum = data.reduce((s, x) => s + x, 0);
let mean = Math.round(sum / count);
let min = data.reduce((m, x) => x < m ? x : m, data[0]);
let max = data.reduce((m, x) => x > m ? x : m, data[0]);

console.log("Count:", count);
console.log("Sum:", sum);
console.log("Mean:", mean);
console.log("Min:", min);
console.log("Max:", max);
