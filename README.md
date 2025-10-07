# BBS Encryption Program

This program encrypts and decrypts a message using a stream cipher based on the Blum-Blum-Shub (BBS) pseudo-random number generator.

## Prerequisites

- A C++ compiler that supports C++11 (e.g., g++)
- `make` build automation tool

## How to Compile

To compile the program, navigate to the `indep_2` directory in your terminal and run the `make` command:

```bash
make
```

This will generate an executable file named `encryptor`.

## How to Run

After successful compilation, you can run the program with the following command:

```bash
./encryptor
```

The program will then encrypt and decrypt a hardcoded message ("YourLastName") and print the original, encrypted (in hex), and decrypted versions to the console.

### Customizing the Message

To encrypt a different message, you need to edit the `main.cpp` file. Find this line:

```cpp
std::string message = "YourLastName";
```

And replace `"YourLastName"` with the message of your choice. After editing, you'll need to recompile the program using `make`.

## How to Clean

To remove the compiled object files and the executable, run:

```bash
make clean
```
