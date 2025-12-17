# Syllabus and Material for High Performance Programming and Systems (HPPS)

Welcome to the course website for HPPS! All material and general
information will be provided here. Announcements, assignment handin,
and the discussion forum remains on Absalon. While this website is a
Git repository, you are not required or expected to use Git to
interact with it, but feel free to do so if convenient for you.

## Basics

<img src="https://github.com/user-attachments/assets/2966d1a8-d741-43e2-a686-aeab2b01a510" align="right">

HPPS takes place in block 2. There are two kinds of in-person teaching
activities: **lectures** and **exercises**.

**Lectures** are mostly conventional and serve as warmup for the
exercises.

**Exercises** are conducted in four smaller classes, each conducted by
a teaching assistant (TA).  An exercise session involves working on
problems that will help you learn the curriculum, and in many cases
directly train you in the skills you will need for the mandatory
assignments.

**This course website** is used for handing out material.

**Discord** can be used for asking questions. ([Invite
link](https://discord.gg/NgsQ4TdfNe)). Remember to use the same name on Discord
as on Absalon. You are not *required* or *expected* to use Discord. We continue
to monitor the Absalon discussion forum, and course announcements are posted
solely on Absalon.

**Absalon** is used for handing in assignments and for course
announcements.

## Textbook

The textbooks are as follows:

* **JG:** [Modern C](https://modernc.gforge.inria.fr/) ([CC-licensed PDF](https://hal.inria.fr/hal-02383654/document))

* **HPPS:** [HPPS course notes](https://github.com/diku-dk/hpps-notes)
  [PDF](https://github.com/diku-dk/hpps-notes/releases/download/latest/hpps-notes.pdf) -
  **these will be updated as the course progresses, so make sure to
  regularly check that you have the newest version**

## Schedule

Ignore the official class grouping made by KU. Go to whichever class
you prefer.

### Tuesday

* **Lecture:** 10:00-12:00 (Aud 04, HCØ).

### Thursday

* **Exercises:** 10:00-12:00 (see below)

* **Lecture:** 13:00-15:00 (Aud 02, HCØ)

* **Exercises:** 15:00-17:00 (see below)

## Exercise classrooms

**The information in this section is tentative.**

The Thursday exercise classes are held in various exciting locations around the
world - you can go where you wish, but please exercise a bit of manual load
balancing:

### Morning

- Karnapsalen, (54 pers.) Nørre Alle 53

- 4-0-02, Ole Maaløes Vej 5, Biocenter

- NBB 2.0.G.064/070, Jagtvej 155

### Afternoon

- øv - 4-0-02, Ole Maaløes Vej 5, Biocenter

- øv - 4-0-13, Ole Maaløes Vej 5, Biocenter

- øv - 4-0-32, Ole Maaløes Vej 5, Biocenter (no TA)

## Assignments

There are 5 assignment in total during the course with deadlines every
week. They overlap slightly to allow for more flexibility in your
scheduling, but think of them as weekly assignments. The assignments
are handed out at the beginning of the week. They are handed out on
this webpage, and handed in via Absalon.

The assignments will be graded with points from 0 to 4. It is not
possible to re-hand-in any of the assignments.

Assignments are made to be solved in groups of preferably three
students, but groups of two active students will also do. We strongly
encourage you not to work alone. Groups cannot be larger than three
students. Each group must make their own solutions and cannot share
implementations and report with other. You may however discuss
material and ideas.

**Do not store your assignments in public GitHub repositories.**

### General assignment rules

The following rules apply to all assignments. They are intended to
ease our correction process, and in particular to allow automated
testing. Consider the assignments to be a specification or API that
you are asked to implement.

1. Do not modify the types of any definitions in the handout, except
   when the assignment text explicitly instructs you to do so.

2. Do not rename or remove any definitions that are present in the
   handout, except when the assignment text explicitly instructs you
   to do so.

3. Do not remove anything from header files.

4. Do not rename files or otherwise modify the file tree. (You may add
   new files if you wish, although it is rarely necessary.)

5. Your code should compile without warnings. (Do not achieve this by
   disabling warnings.)

6. When handing in, you must hand in a complete workable program
   (including unmodified files from the handout).

7. When handing in, do not include temporary build files (such as
   `.o`), editor backup files, or various other computer detritus. Run
   e.g. the `tree` command and read the file listing and ponder for
   each file whether it is something it makes sense to hand to your
   TA. At a *minimum*, run `make clean` before handing in.

8. Your zip file, should contain a single top-level folder with an
   appropriate name (e.g. `handin`).

Violation of these rules will result in points deductions. If you
violate these rules at the exam, it will negatively influence your
grade.

## Languages

All written material will be in **English**.

Most oral teaching will be in **Danish** or **English**, depending on
the specific teacher or TA.

You will mainly be programming in **C** and **Python**.

## Software

You will be using a Unix command line and Unix tools for much of the
course.  [See the Unix software guide.](unix.md)

See also [this guide on the GDB
debugger](http://beej.us/guide/bggdb/), which is a very useful tool
for debugging C programs.

If you prefer an IDE, see the [VS Code installation and setup
guide](vscode.md).

### OpenMP on MacOS

MacOS users will run into a problem by week 5 where they cannot run `gcc
--fopenmp`. To solve this problem, check out [this guide](macos-openmp.md).

## People

The teachers are

* Troels Henriksen <athas@sigkill.dk>
* Philippe Bonnet <bonnet@di.ku.dk>

The TAs are:

- Kevin Mark Lock <zqw671@di.ku.dk>
- August Rehm <aure@di.ku.dk>
- Carl August Gjerris Hartmann <cgha@di.ku.dk>
- Márk Viski <twg751@di.ku.dk>
- Sofie Larsen <lzb674@alumni.ku.dk>

## Exam

### Format

The exam will be a take-home exam that will be very similar in form to
the mandatory assignments. See [examples of old exams
here](old-exams/). It has an estimated workload of 20 hours. It will
involve practical programming and performance analysis of programs.

### Qualification

To qualify for the exam you are required to achieve at least 50% of
the total number of points in the five assignments (that is, 10 points
at minimum). You also need to get *at least* one point in each of the
first five assignment.

### Date

The exam is handed out on the 12th of January at 8:00 and must be handed in by
the 14th of January 11:00.

## Course plan

Each week has an associated subdirectory here on GitHub that lists
expected reading and relevant exercises.

## AI Policy

You are allowed to use AI-generated code and text in assignments and
the exam, but you must *explicitly* indicate which parts have been
generated this way, and which tool you have used. If you do not cite
properly, then you are conducting academic dishonesty (i.e.,
plagiarism), which is treated very seriously by the university.
