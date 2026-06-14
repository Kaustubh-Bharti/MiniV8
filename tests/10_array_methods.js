let arr = [10, 20, 30, 40, 50];
console.log("Length:", arr.length);

// push / pop
arr.push(60);
console.log("After push:", arr.join(","));
let popped = arr.pop();
console.log("Popped:", popped);
console.log("After pop:", arr.join(","));

// indexOf, includes
console.log("indexOf 30:", arr.indexOf(30));
console.log("includes 20:", arr.includes(20));
console.log("includes 99:", arr.includes(99));

// slice
let sliced = arr.slice(1, 3);
console.log("Slice(1,3):", sliced.join(","));

// concat
let other = [100, 200];
let merged = arr.concat(other);
console.log("Concat:", merged.join(","));

// reverse
let rev = arr.reverse();
console.log("Reversed:", rev.join(","));

// find
let nums = [1, 5, 10, 15, 20];
let found = nums.find(x => x > 8);
console.log("Find >8:", found);
