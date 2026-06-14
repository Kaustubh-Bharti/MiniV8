// String Analyzer — Strings + Loops + Arrays + Objects + Math

function analyzeString(str)
{
    let chars = str.split("");
    let len = str.length;
    let upper = 0;
    let lower = 0;
    let spaces = 0;
    let digits = 0;
    let other = 0;

    for (let i = 0; i < len; i++)
    {
        let c = chars[i];

        if (c === " ")
        {
            spaces++;
        }
        else if (c >= "A" && c <= "Z")
        {
            upper++;
        }
        else if (c >= "a" && c <= "z")
        {
            lower++;
        }
        else if (c >= "0" && c <= "9")
        {
            digits++;
        }
        else
        {
            other++;
        }
    }

    return {
        length: len,
        uppercase: upper,
        lowercase: lower,
        spaces: spaces,
        digits: digits,
        other: other
    };
}

let text = "Hello World 123!";
let result = analyzeString(text);

console.log("Text:", text);
console.log("Length:", result.length);
console.log("Uppercase:", result.uppercase);
console.log("Lowercase:", result.lowercase);
console.log("Spaces:", result.spaces);
console.log("Digits:", result.digits);
console.log("Other:", result.other);

// Word analysis
let words = text.trim().split(" ");
console.log("Word count:", words.length);

let wordLengths = words.map(w => w.length);
console.log("Word lengths:", wordLengths.join(", "));

let longest = wordLengths.reduce((max, len) => len > max ? len : max, 0);
console.log("Longest word length:", longest);

// Case conversion round-trip
let upper = text.toUpperCase();
let lower = text.toLowerCase();
console.log("Upper:", upper);
console.log("Lower:", lower);
console.log("Round-trip match:", upper.toLowerCase() === lower);
