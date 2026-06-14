let person = {name: "Alice", age: 25, city: "NYC"};

console.log("Name:", person.name);
console.log("Age:", person.age);
console.log("City:", person.city);

// Modify property
person.age = 26;
console.log("New age:", person.age);

// Add new property
person.email = "alice@example.com";
console.log("Email:", person.email);

// Object.keys and Object.values
let keys = Object.keys(person);
console.log("Keys:", keys.join(","));

let values = Object.values(person);
console.log("Values:", values.join(","));
