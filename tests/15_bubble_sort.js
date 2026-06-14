// Bubble Sort — Arrays + Loops + Functions + Comparison

function bubbleSort(arr)
{
    let n = arr.length;

    for (let i = 0; i < n - 1; i++)
    {
        for (let j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap using temp
                let temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    return arr;
}

let nums = [64, 34, 25, 12, 22, 11, 90];
console.log("Before:", nums.join(", "));

let sorted = bubbleSort(nums);
console.log("After:", sorted.join(", "));

// Verify it's sorted
let isSorted = true;
for (let i = 0; i < sorted.length - 1; i++)
{
    if (sorted[i] > sorted[i + 1])
    {
        isSorted = false;
        break;
    }
}
console.log("Sorted correctly:", isSorted);

// Find min and max from sorted
console.log("Min:", sorted[0]);
console.log("Max:", sorted[sorted.length - 1]);
