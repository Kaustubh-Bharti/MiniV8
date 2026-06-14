// Matrix Operations — Nested Arrays + Loops + Functions + Math

// Create a matrix from flat array
function createMatrix(rows, cols, values)
{
    let matrix = [];
    let idx = 0;

    for (let r = 0; r < rows; r++)
    {
        let row = [];
        for (let c = 0; c < cols; c++)
        {
            row.push(values[idx]);
            idx++;
        }
        matrix.push(row);
    }

    return matrix;
}

// Print matrix row by row
function printMatrix(label, mat)
{
    console.log(label);
    for (let r = 0; r < mat.length; r++)
    {
        console.log("  " + mat[r].join("\t"));
    }
}

// Add two matrices
function addMatrices(a, b)
{
    let result = [];
    for (let r = 0; r < a.length; r++)
    {
        let row = [];
        for (let c = 0; c < a[r].length; c++)
        {
            row.push(a[r][c] + b[r][c]);
        }
        result.push(row);
    }
    return result;
}

// Multiply two matrices (2x3 * 3x2 = 2x2)
function multiplyMatrices(a, b)
{
    let rowsA = a.length;
    let colsA = a[0].length;
    let colsB = b[0].length;
    let result = [];

    for (let r = 0; r < rowsA; r++)
    {
        let row = [];
        for (let c = 0; c < colsB; c++)
        {
            let sum = 0;
            for (let k = 0; k < colsA; k++)
            {
                sum += a[r][k] * b[k][c];
            }
            row.push(sum);
        }
        result.push(row);
    }

    return result;
}

// Transpose
function transpose(mat)
{
    let rows = mat.length;
    let cols = mat[0].length;
    let result = [];

    for (let c = 0; c < cols; c++)
    {
        let row = [];
        for (let r = 0; r < rows; r++)
        {
            row.push(mat[r][c]);
        }
        result.push(row);
    }

    return result;
}

// Test with 2x3 matrices
let A = createMatrix(2, 3, [1, 2, 3, 4, 5, 6]);
let B = createMatrix(2, 3, [7, 8, 9, 10, 11, 12]);

printMatrix("Matrix A:", A);
printMatrix("Matrix B:", B);

let sum = addMatrices(A, B);
printMatrix("A + B:", sum);

let At = transpose(A);
printMatrix("Transpose of A:", At);

// Multiply A (2x3) * At (3x2) = 2x2
let product = multiplyMatrices(A, At);
printMatrix("A * A^T:", product);

// Sum of all elements
let total = 0;
for (let r = 0; r < A.length; r++)
{
    for (let c = 0; c < A[r].length; c++)
    {
        total += A[r][c];
    }
}
console.log("Sum of A:", total);
