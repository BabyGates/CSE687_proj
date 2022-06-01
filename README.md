CSE687_proj

Phase 3 README

No code changes for this phase. All phase 3 requirements have been implemented since phase 1. No new functionality.

This repo does not come with a pre-compiled executable. Please download and compile the project.

Steps to run:

0. The master solution file is repo/repo.sln The external MapperDLL and ReducerDLL folders will be linked automatically.
1. After compiling, navigate to the build's output directory (repo/x64/Debug)
    -For phase 2, the output DLL's are built out to "repo/dlls" 
2. Open a powershell or cmd instance in this directory. This can be done with shift+right-click and "open Powershell here"
3. The executable, "repo.exe", needs 4 parameters: input dir, temp dir, and output dir, and DLL dir respectively.
4. All directories can be either absolute system paths (C:\*) or relative paths from the current directory.
    -The provided shakespear files have been include in "repo/input"
    -You may specify "reverse" directories such as "../../input" to access "repo/input" folder from "repo/x64/Debug"
    -If either the temp or output directories do not already exist, they will be created for you at runtime. They will also be cleared of any residual files.
    -If the program cannot find the input directory or cannot create the temp/output directories, an error will be thrown.
    -If the provided DLL folder is empty or does not contain valid map and reduce DLLS, an error will be thrown.
5. The syntax for execution is ".\repo.exe <input dir> <temp dir> <output dir> <DLL dir>".
    -i.e. ".\repo.exe ../../input temp output ../../dlls" 
    -this grabs the input sharespear folder in "repo/input", generates temp/output folders to "repo/x64/Debug", and loads the dlls in "repo/dlls"
6. Unit tests are executed at runtime before the real program starts. Verbose output from the tests are logged.
7. The program will map and reduce all the input files -- populating the temp folder with the required temp files and generating a single output file.
    -DLL's are loaded at runtime upon request. This happens once during unit test, and again during core execution.
    -It will print the time taken to execute. For all the sharespear files, it will take only ~10-15 seconds depending on your machine.
        --I have implemented extremely efficient data structures for sorting and reducing the temp files. I encourage you to stress-test the program with obscenely large inputs :)
8. A single "final_output.txt" file will be generated to the specified output dir with an aggregated list of the word counts from all input files.
    -A blank SUCCESS or FAILURE file will also be generated to the output dir depending on the relative success of the program.
    -Note that residual temp files are not cleaned up upon program exit, they are cleaned up at program start.

Contributed by Brannon Henderson
