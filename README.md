CSE687_proj

Phase 4 README

Changes for Phase 4:

YOU MAY ENCOUNTER A WINDOWS FIREWALL POPUP WHEN THE PROGRAM RUNS FOR THE FIRST TIME.
ALLOW ALL CONNECTIONS FOR THE REPO.EXE APPLICATION AND RESTART THE WHOLE PROGRAM!
This is due to new socket connections binding to the localhost

Ports 127.0.0.1:10001-10100 should be free and available on your computer

New syntax to run. The user must only specify the input directory. The DLLs, temp, and output folder are now hard coded to be "repo/dlls", "repo/x64/Debug/temp", and "repo/output" respectively.
Example syntax: ".\repo.exe ../../input" from a powershell instance within repo\x64\Debug

At runtime, 'N' stub threads are created for 'N' number of input files. Each stub will be responsible for managing either a mapper or reducer subprocess.
It makes sense to only have 1 sub-process per stub as these sub-processes are designed to be run at full 100% CPU so threading out additional sub-processes per stub does not makes sense. There will be no performance gain.
The stubs act as provisioning managers for the sub-process children, handling all UDP comms and data transfers so the sub-process children can focus on actual work.
All inter-thread communication is done with datagram sockets (winsocks).
All UDP packets are transfered across the localhost (127.0.0.1) with varying ports. Each stub is assigned a unique port at creation.
A master listening server is started in the workflow (as detached) which acts as the main data control daemon.
This daemon is responsible for receiving the mapper/reducer status messages, starting/stopping these processes via socket communication as appropriate, and ending the program when the final reducer send a success message.
Each mapper and reducer themselves have daemon loops which are responsible for publishing heartbeat status messages back to the controller.
Task start and stop requests are routed through the stubs and then to the appropriate sub-processes as dictated by the controller.
No algorithms for map or reduce have fundamentally changed, just the inter-thread communication via sockets.
New for phase 4: there is only 1 temp file with all the merged ("words, 1") from all input files. Previously this merging happened at the reducer level, however now the mappers are responsible for merging this data

This repo does not come with a pre-compiled executable. Please download and REBUILD THE WHOLE SOLUTION. Don't just get latest from github and hit CTRL + B, the DLLs don't always rebuild in this case

Steps to run:

0. The master solution file is repo/repo.sln The external MapperDLL and ReducerDLL folders will be linked automatically.
1. After building, navigate to the build's output directory (repo/x64/Debug)
    -For phase 2, the output DLL's are built out to "repo/dlls" 
2. Open a powershell or cmd instance in this directory. This can be done with shift+right-click and "open Powershell here"
3. The executable, "repo.exe", needs 1 parameter: input dir.
4. All directories can be either absolute system paths (C:\*) or relative paths from the current directory.
    -The provided shakespear files have been include in "repo/input"
    -You may specify "reverse" directories such as "../../input" to access "repo/input" folder from "repo/x64/Debug"
    -If the program cannot find the input directory or cannot create the temp/output directories, an error will be thrown.
    -If the required DLL folder is empty or does not contain valid map and reduce DLLs, an error will be thrown.
5. The syntax for execution is ".\repo.exe <input dir>".
    -i.e. ".\repo.exe ../../input" 
    -this grabs the input sharespear folder in "repo/input", generates temp folders to "repo/x64/Debug", output folder to repo/output, and loads the dlls in "repo/dlls"
6. Unit tests are executed at runtime before the real program starts. Verbose output from the tests are logged.
7. The program will map and reduce all the input files -- populating the temp folder with the required temp files and generating a single output file.
    -DLL's are loaded at runtime upon request. This happens once during unit test, and again during core execution.
    -It will print the time taken to execute. For all the sharespear files, it will take only ~15-20 seconds depending on your machine.
        --I have implemented extremely efficient data structures for sorting and reducing the temp files. I encourage you to stress-test the program with obscenely large inputs :)
8. A single "repo/output/final_output.txt" file will be generated with an aggregated list of the word counts from all input files.
    -A blank SUCCESS or FAILURE file will also be generated to the output dir depending on the relative success of the program.
    -Note that residual temp files are not cleaned up upon program exit, they are cleaned up at program start.

Contributed by Brannon Henderson
