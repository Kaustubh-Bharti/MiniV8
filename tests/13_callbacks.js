// Map
let nums = [1, 2, 3, 4, 5];
let doubled = nums.map(x => x * 2);
console.log("Map:", doubled.join(","));

// Filter
let evens = nums.filter(x => x % 2 === 0);
console.log("Filter evens:", evens.join(","));

// Reduce
let sum = nums.reduce((acc, x) => acc + x, 0);
console.log("Reduce sum:", sum);

// Callback function
function applyOp(a, b, op)
{
    return op(a, b);
}

let result = applyOp(10, 5, (a, b) => a + b);
console.log("Callback add:", result);

result = applyOp(10, 5, (a, b) => a * b);
console.log("Callback mul:", result);

// forEach
console.log("forEach:");
nums.forEach(x => console.log(x));

// Chained operations
let processed = nums.filter(x => x > 2).map(x => x * 10);
console.log("Chained:", processed.join(","));
