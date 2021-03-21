
# ding
### - A Programming Language for Customizable Network Traffic Handling

**Version** - ding_u2.1

**Developer & Maintainer** - [Rishi Tharun](https://www.github.com/rishitharun) - <<vrishitharunj@gmail.com>><br>
<br>
>NOTE:
> * Target system is **LINUX** only - Not portable
> * Root access is required to run
> * Requires **Python 3.x** and **GNU Compiler Collection**
> * Install python3 and gcc before proceeding to build

<br>

### About Release and Version Numbering
* Project is still under development.
* The release of this ding version does not correspond to an actual release.
* The concept of release corresponds to a snapshot of the project during development phase.
* Until development is complete, referral to the word **Release** corresponds to a snapshot.
* These snapshots are also version numbered.
* The version numbering method followed is **ding_(phase)(branch).(release/snapshot)**
* Current snapshot version is **ding_u2.1**
* A Comprehensive Developer's Guide is not yet prepared. Please contact the developer to know more.

<br>

## About ding:
  *ding* is a network-specific programming language, used for *customizable network traffic handling.*
  This can be used to create, send, receive customisable network packets, and perform customized
  real time traffic handling with other network devices. ding is written entirely in C. Support tools
  are written in python. Currently, as of this release, the project is still under development.

> *Handle the traffic on you own, as you wish !*

<br>

### Translator Type:
  INTERPRETER

<br>

### To Build:
  `python build.py`
<br> Copy to /bin/: `cp ./ding /bin/`

<br>

### To Run:
  `ding <sourcefile>`

<br>

### To invoke Shell:
  `ding`

<br>

### Program Strcuture:
1. Three blocks
    * **".decl:"** --> To declare variables; No definitions allowed
    * **".func:"** --> To define functions; _**NOT HANDLED**_
    * **".flow:"** --> Program execution entrypoint; _**PARTIALLY HANDLED**_
2. Comments
    - Characters succeeding the **!** character in a line are treated as Comments;
    - __NO__ Multi-line comments

<br>

### Sample ding Codes:
  Check `/scripts/` folder

<br>

### How to code:
  Check `howto.txt` and `/scripts/*.ding`


