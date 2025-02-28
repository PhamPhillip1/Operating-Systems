# Operating Systems Project 1: Multi-threading and IPC

## Overview
This project implements two separate programs: a basic banking system and IPC using pipes in C++. The banking system stimulates a multi-threaded environment where different operations like deposits, withdrawals, and transfers are performed concurrently on different bank accounts. The program uses C++ standard library's <thread> to create and manage threads (transactions) at once. Additionally the inter-process communication is done by using pipes for the communication between two processors. You will be using the programs in a Linux environment.

The main features of the project include:
- Users can deposit and withdraw money from multiple bank accounts
- Operations on bank accounts are done concurrently using threads
- Transfers between different accounts are also done concurrently in a multi-threaded environment
- A child process reads from the parent process using pipes for IPC

## Step-by-Step instructions in building and running the program
### Setting up the environment

- If you are using a mac or windows laptop, you would need to download a virtual machine to run on a linux environment. VMware and ubuntu is recommended. Instructions to download are posted on their respective websites. Although you can run it on other os systems, it is preferred to be run on Linux to get familiar with the tools and workflow it provides.
- Vscode is recommended as the code IDE. To download Vscode, go to the Vscode website and follow the instructions.

Once you have the environment set up, download the necessary extensions:
- C/C++ 
- C/C++ extension pack
- C/C++ Themes
- Code Runner
- CMake and CMake Tools
  
These will help with running C/C++ programs as well as making a more convenient workflow. Make sure you are using the g++ compiler and not gcc since it is a C++ file. Now the environment is ready.

### Runnnig the Program
To interact with the banking system:
- Enter amount to start off in the bank accounts
- Enter amount of transactions and transfer between accounts
- The program will then simulate multiple transactions concurrently using threads
- The IPC implementation is not user dependent as it only shows the child process receiving a message from the parent process

## Dependencies (On ubuntu)
To install the needed dependencies, run the following command in the terminal:
- sudo apt update
- sudo apt install g++


