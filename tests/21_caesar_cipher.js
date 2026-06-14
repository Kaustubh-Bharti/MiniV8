// Caesar Cipher — Strings + Loops + Math + Functions + Callbacks

function caesarEncrypt(text, shift)
{
    let chars = text.split("");

    let encrypted = chars.map(c => {
        if (c >= "A" && c <= "Z")
        {
            let code = c.charAt(0);
            let offset = code.charCodeAt(0) - 65;
            let shifted = (offset + shift) % 26;
            return String.fromCharCode(shifted + 65);
        }
        else if (c >= "a" && c <= "z")
        {
            let code = c.charAt(0);
            let offset = code.charCodeAt(0) - 97;
            let shifted = (offset + shift) % 26;
            return String.fromCharCode(shifted + 97);
        }
        return c;
    });

    return encrypted.join("");
}

// Simpler approach without charCodeAt (which we don't support)
// Use a lookup table instead

let upperAlpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
let lowerAlpha = "abcdefghijklmnopqrstuvwxyz";

function encrypt(text, shift)
{
    let chars = text.split("");
    let result = "";

    for (let i = 0; i < chars.length; i++)
    {
        let c = chars[i];
        let upperIdx = upperAlpha.indexOf(c);
        let lowerIdx = lowerAlpha.indexOf(c);

        if (upperIdx >= 0)
        {
            let newIdx = (upperIdx + shift) % 26;
            result += upperAlpha.charAt(newIdx);
        }
        else if (lowerIdx >= 0)
        {
            let newIdx = (lowerIdx + shift) % 26;
            result += lowerAlpha.charAt(newIdx);
        }
        else
        {
            result += c;
        }
    }

    return result;
}

function decrypt(text, shift)
{
    return encrypt(text, 26 - shift);
}

let message = "Hello World";
let shift = 3;

let encoded = encrypt(message, shift);
console.log("Original:", message);
console.log("Encrypted:", encoded);

let decoded = decrypt(encoded, shift);
console.log("Decrypted:", decoded);
console.log("Round-trip:", message === decoded);

// Encrypt with different shifts
let shifts = [1, 5, 13, 25];
shifts.forEach(s => {
    let enc = encrypt("ABC", s);
    let dec = decrypt(enc, s);
    console.log("Shift " + s + ": ABC -> " + enc + " -> " + dec);
});
