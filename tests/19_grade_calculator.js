// Grade Calculator — Objects + Arrays + Callbacks + Ternary + Math

let students = [
    {name: "Alice", scores: [85, 92, 78, 95, 88]},
    {name: "Bob", scores: [72, 68, 75, 80, 65]},
    {name: "Charlie", scores: [95, 98, 92, 97, 100]},
    {name: "Diana", scores: [60, 55, 70, 45, 58]},
    {name: "Eve", scores: [88, 90, 85, 92, 87]}
];

// Calculate average for each student
function getAverage(scores)
{
    let sum = scores.reduce((s, x) => s + x, 0);
    return Math.round(sum / scores.length);
}

// Assign letter grade
function getGrade(avg)
{
    if (avg >= 90)
    {
        return "A";
    }
    else if (avg >= 80)
    {
        return "B";
    }
    else if (avg >= 70)
    {
        return "C";
    }
    else if (avg >= 60)
    {
        return "D";
    }
    else
    {
        return "F";
    }
}

// Process each student
console.log("--- Report Card ---");

let averages = [];

students.forEach(student => {
    let avg = getAverage(student.scores);
    let grade = getGrade(avg);
    let status = avg >= 60 ? "PASS" : "FAIL";

    averages.push(avg);

    console.log(student.name + ": avg=" + avg + " grade=" + grade + " " + status);
});

// Class statistics
let classAvg = Math.round(
    averages.reduce((s, x) => s + x, 0) / averages.length
);

let highest = averages.reduce((m, x) => x > m ? x : m, averages[0]);
let lowest = averages.reduce((m, x) => x < m ? x : m, averages[0]);

console.log("--- Class Stats ---");
console.log("Class average:", classAvg);
console.log("Highest:", highest);
console.log("Lowest:", lowest);

// Find honor roll (B or above)
let honorRoll = [];
for (let i = 0; i < students.length; i++)
{
    if (averages[i] >= 80)
    {
        honorRoll.push(students[i].name);
    }
}
console.log("Honor roll:", honorRoll.join(", "));

// Count by grade
let gradeA = averages.filter(a => a >= 90).length;
let gradeB = averages.filter(a => a >= 80 && a < 90).length;
let gradeC = averages.filter(a => a >= 70 && a < 80).length;
let gradeD = averages.filter(a => a >= 60 && a < 70).length;
let gradeF = averages.filter(a => a < 60).length;

console.log("A:", gradeA, "B:", gradeB, "C:", gradeC, "D:", gradeD, "F:", gradeF);
