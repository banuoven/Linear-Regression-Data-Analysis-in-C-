# Linear Regression & Data Analysis in C++

This project implements **simple linear regression** and basic data analysis using C++.  
It reads a dataset with three features and one target from a text file, splits the data into training and test sets, selects the most correlated feature, trains a linear model, and evaluates the error.

##  Files
- **main.cpp** — C++ source code that:
  - Reads data from a text file with **four columns**: `x1 x2 x3 y`
  - Randomly splits the data into **70% training** and **30% test**
  - Computes correlations between each feature (`x1`, `x2`, `x3`) and the target `y` on the **training set**
  - **Selects the single feature** with the largest absolute correlation with `y`
  - Fits a **linear regression** model `y = a + b * x` using the selected feature (training set)
  - Computes **SSE (Sum of Squared Errors)** on training and test sets
  - Prints predicted values (ŷ) for both training and test sets
- **data.txt** — Sample dataset (space-separated), each row is: `x1 x2 x3 y`

##  Build & Run
Open a terminal in the project folder and run:
```bash
g++ main.cpp -o regression
./regression
```

> **Important:** In the current code, the input file path is hardcoded as:
> ```c
> FILE *dosya = fopen("/Users/banuoven/Desktop/data.txt", "r");
> ```
> If you keep `data.txt` in the **same folder** as `main.cpp`, change that line to:
> ```c
> FILE *dosya = fopen("data.txt", "r");
> ```
> Otherwise, set it to the **full path** where your `data.txt` is located.

##  What the Program Does (Step-by-Step)
1. **Read data** from `data.txt` into arrays `x1`, `x2`, `x3`, `y`.
2. **Shuffle indices** with Fisher–Yates and `srand(time(NULL))` so the split is random each run.
3. **Split** into `70%` training and `30%` test.
4. On **training data**:
   - Compute correlations `corr(x1, y)`, `corr(x2, y)`, `corr(x3, y)`.
   - **Pick the feature** with the highest `|correlation|`.
   - Fit **simple linear regression** `y = a + b * x` with that feature.
5. **Evaluate**:
   - Predict `ŷ` for training and test sets.
   - Report **SSE (Sum of Squared Errors)** for training and test.
   - Print all predicted values (ŷ).

##  Sample (Illustrative) Console Output
```text
Total number of data points: 100

=== TRAINING DATA ===
Correlations:
 x1 - y: 0.81
 x2 - y: 0.54
 x3 - y: 0.76

Selected feature: x1
Linear Regression: y = a + b * x1
SSE (Train): 123.456
Predicted ŷ values (Train):
ŷ: 12.3456
ŷ: 10.9876
...

=== TEST DATA ===
SSE (Test): 56.7890
Predicted ŷ values (Test):
ŷ: 11.2345
ŷ: 13.2100
...
```

> **Note on Reproducibility:** Because the split is randomized via `srand(time(NULL))`, your results change on each run.  
> For a fixed split during debugging, replace `srand((unsigned)time(NULL));` with e.g. `srand(42);`.

##  Data Format
- File: `data.txt`  
- Each row contains **four** space-separated numbers: `x1 x2 x3 y`  
- Example:
```text
8.4442 7.5795 4.2057 35.3543
4.0493 7.8380 3.0331 29.7743
...
```

##  Author
- Banu Oven
- Computer Engineering Student
