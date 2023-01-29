# Assignment 1 - File Open Program

## Due February 2, 2023

To build this program, run `make` in this directory.

To run this program, run `a1g3` in the `build` directory.

Accompanying Documentation:
- [Assignment Description](docs/fileopenassignment.pdf)
- [Rubric](docs/fileopenrubric.pdf)
- [Task List](docs/tasklist.docx)

---

Develop a program, written in C, that does the following:
- Accepts up to two command line parameters, input file name and output file name
    - `program <inputfile> <outputfile>`
    - Validates the input parameters
    - **Input File**:
        - If no file extension is given, use `.in`
    - **Output File**:
        - If no file extension is given, use `.out`
- If a valid parameter is not given, the program will prompt the user for input and then validate
    - If no input is given from the user, exit the program
- Create two extra files:
    - A **temporary** file, which does not share a name with existing files
    - A **listings** file, which uses the extension `.lis`
- Read symbols from the input file and write to the **output** and **listings** files. 
- Write code to create and delete the **temporary** file, but comment out the deletion code for this assignment.