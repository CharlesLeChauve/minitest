# Minitest

## Introduction

Minishell is a collaborative project developed by **Tgibert** (me) and **Anporced** as part of our curriculum at 42. 
This project aims to build a little interactive shell written in C and inspired by Bash. 

## Features

- **Redirections and Heredoc**: Basic file redirections and heredocs are handled.
- **Priority Operators and Pipes**: Command lines can contains `|`, `&&`, `||`or `()`.
- **Wildcards**: Possibility to use `*` to get all corresponding patterns.
- **Builtins**: echo, env, export, cd, pwd. (export and env are simplified) 

## Project Structure

The project is organized into the following components:

- **Source Files**:
  - [`srcs/`](./srcs/): Contains the main source code for the tests.
  - [`minishell.h`](./minishell.h): Header file with necessary declarations and includes.

- **Library**:
  - [`libft/`](./libft/): Custom C library made as first project and used throughout the 42 school curriculum.

- **Configuration**:
  - [`Makefile`](./Makefile): Build configuration for compiling the tests.
  - [`valgrind.supp`](./valgrind.supp): Suppression file for Valgrind to manage known, non-critical memory issues during testing.

## Getting Started

### Prerequisites

- Unix-based system (Linux or macOS recommended)
- GCC compiler
- Make utility
- Valgrind (for memory leak testing)

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/CharlesLeChauve/minitest.git
   cd minitest
   ```

2. **Compile the Project**:
   ```bash
   make
   ```

### Usage

After compilation, run the tests using:

```bash
./minishell
```

To check for memory leaks using Valgrind you can use this suppression file to ignore readline leaks:

```bash
valgrind --suppressions=valgrind.supp ./minishell
```

## Contributing

As this is a school project, external contributions are not being accepted.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more details.

