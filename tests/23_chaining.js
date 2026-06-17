const str = "   JavaScript Hidden Test Case   ";

const result = str
    .trim()
    .toLowerCase()
    .replace("hidden", "secret")
    .split(" ")
    .filter(word => word.length > 3)
    .join("-")
    .toUpperCase();

console.log(result);