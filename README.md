Sports Event Management System in C
This is a first-year C console project that manages sportsman and event data by reading from text files, performing various data operations, and saving the changes back to the files.

📜 Overview
The program acts as a simple database system for an athletic league or club. It builds an in-memory database from two text files: SportsmanData.txt and EventData.txt. Users can interact with the data through a console menu to add, delete, query, and perform simple analyses, such as finding the top-performing club. All changes are saved back to the files upon exiting the program.

✨ Features
The program provides a comprehensive menu to interact with the data:

Add Sportsman: Adds a new sportsman to the database, including ID, first name, last name, club, and gender.

Add Event: Adds a new event to an existing sportsman by their ID.

Print Sportsman's Events: Displays all events a specific sportsman has participated in, searched by last name.

Count Sportsmen in Event: Counts and displays the number of sportsmen who participated in a specific event (by name and year).

Find Best Club: Calculates and displays the club with the highest total number of event participations across all its members.

Check Shared Events: Shows which events a specific sportsman has participated in with other sportsmen.

Print Club Events (Sorted): Displays all events for a specific club, sorted by year.

Delete Event: Removes an event (by name and year) from all sportsmen who participated in it.

Create Combined Club File: Generates a new file named Club.txt that lists all unique events attended by members of two specified clubs.

🛠️ Technologies Used
Language: C

Standard Libraries: stdio.h, stdlib.h, string.h, assert.h

Core Concepts:

Dynamic Memory Management (malloc, realloc, free)

File I/O (fopen, fscanf, fprintf, fclose)

Data Structures (structs, dynamic arrays)

String Manipulation

Basic Algorithms (searching, sorting)

🚀 How to Compile and Run
To compile and run this project, you will need a C compiler (like GCC).

Prerequisites
Make sure you have GCC or a compatible C compiler installed.

Create two data files in the same directory as your code:

SportsmanData.txt: This should contain the sportsman data. The first line must be a header, followed by the data.

format:sportsman_id;first_name;last_name;club_name;gender
101;John;Doe;Maccabi;0
102;Jane;Smith;Hapoel;1
EventData.txt: This should contain the event data. The first line must be a header, followed by the data.

format:sportsman_id,event_name,location,year
101,Running 100m,Tel Aviv,2024
102,High Jump,Haifa,2023
Compilation
Open a terminal or command prompt.

Navigate to the directory where your .c file is located.

Run the following command to compile the program. Assuming your file is named main.c:

Bash

gcc main.c -o sport_manager
Execution
After a successful compilation, an executable file named sport_manager (or sport_manager.exe on Windows) will be created.

Run the program from your terminal:

Bash

./sport_manager
The console menu will appear, and you can start interacting with the program.
