CSE687_proj

Phase 1 README

This repo does not come with a pre-compiled executable. Please download and compile the project.

Steps to run:
1. After compiling, navigate to the build's output directory (Phase 1/repo/x64/Debug)
2. Open a powershell or cmd instance in this directory. This can be done with shift+right-click and "open Powershell here"
3. The executable, "repo.exe", needs 3 parameters: input dir, temp dir, and output dir respectively.
4. All directories can be either absolute system paths (C:\*) or relative paths from the current directory.
    -The provided shakespear files have been include in "Phase 1/repo/input"
    -You may specify "reverse" directories such as "../../input" to access "Phase 1/repo/input" folder from "Phase 1/repo/x64/Debug"
    -If either the temp or output directories do not already exist, they will be created for you at runtime. They will also be cleared of any residual files.
    -If the program cannot find the input directory or cannot create the temp/output directories, an error will be thrown.
5. The syntax for execution is ".\repo.exe <input dir> <temp dir> <output dir>".
    -i.e. ".\repo.exe ../../input temp output" to grab the input sharespear folder in "Phase 1/repo/input" and generate temp/output folders to "Phase 1/repo/x64/Debug"
6. Unit tests are executed at runtime before the real program starts. Verbose output from the tests are logged to the comsole.
7. The program will map and reduce all the input files -- populating the temp folder with the required temp files and generating a single output file.
    -It will print the time taken to execute. For all the sharespear files, it will take only ~10-15 seconds depending on your machine.
        --I have implemented extremely efficient data structures for sorting and reducing the temp files. I encourage you to stress-test the program with obscenely large inputs :)
8. A single "final_output.txt" file will be generated to the specified output dir with an aggregated list of the word counts from all input files.
    -A blank SUCCESS or FAILURE file will also be generated to the output dir depending on the relative success of the program.
    -Note that residual temp files are not cleaned up upon program exit, they are cleaned up at program start.

Contributed by Brannon Henderson
