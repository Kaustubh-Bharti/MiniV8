// Shopping Cart — Objects + Arrays + Map/Reduce + Callbacks

let cart = [
    {name: "Laptop", price: 999, qty: 1},
    {name: "Mouse", price: 25, qty: 2},
    {name: "Keyboard", price: 75, qty: 1},
    {name: "Monitor", price: 350, qty: 1},
    {name: "USB Cable", price: 10, qty: 3}
];

// Calculate line totals using map
let lineItems = cart.map(item => item.price * item.qty);
console.log("Line totals:", lineItems.join(", "));

// Grand total using reduce
let total = lineItems.reduce((sum, val) => sum + val, 0);
console.log("Grand total:", total);

// Filter expensive items (> $50 each)
let expensive = cart.filter(item => item.price > 50);
let expensiveNames = expensive.map(item => item.name);
console.log("Expensive items:", expensiveNames.join(", "));

// Count total items in cart
let totalQty = cart.reduce((sum, item) => sum + item.qty, 0);
console.log("Total items:", totalQty);

// Average price per item type
let avgPrice = Math.round(total / cart.length);
console.log("Avg per type:", avgPrice);

// Find the most expensive item
let maxPrice = 0;
let maxItem = "";
cart.forEach(item => {
    if (item.price > maxPrice)
    {
        maxPrice = item.price;
        maxItem = item.name;
    }
});
console.log("Most expensive:", maxItem, "at", maxPrice);

// Apply 10% discount on items over $100
let discounted = cart.map(item => {
    let newPrice = item.price > 100 ? Math.round(item.price * 0.9) : item.price;
    return newPrice * item.qty;
});
let discountedTotal = discounted.reduce((sum, val) => sum + val, 0);
console.log("After discount:", discountedTotal);
