# MarkovChain

This project is a simple Markov chain text generator in C++.

## Usage

Run the program from the command line:

```
./MarkovChainApp <input.txt> <output.txt> <length> [line length]
```

- `<input.txt>`: Path to the input file with training data (text).
- `<output.txt>`: Path to the output file for the generated text.
- `<length>`: Number of words to generate.
- `[line length]`: Maximum number of characters per line (optional, default is 80).

## Build

Compile the project in the project folder with:

```
make
```