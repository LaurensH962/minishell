# minishell

A Unix shell implementation written in C, built as part of the 42 curriculum. Minishell replicates core features of bash, including pipes, redirections, environment variable expansion, heredocs, and built-in commands.

## Features

### Built-in Commands

| Command | Description |
|---------|-------------|
| `echo [-n]` | Print arguments to stdout; `-n` suppresses trailing newline |
| `pwd` | Print the current working directory |
| `cd [path]` | Change directory; supports `-` for previous directory and `~` for home |
| `export [VAR=value]` | Set or display exported environment variables |
| `unset VAR` | Remove a variable from the environment |
| `env` | Display all environment variables |
| `exit [code]` | Exit the shell with an optional numeric exit code |

### Shell Features

- **Pipes** — Chain commands with `|`
- **Redirections** — `<` (input), `>` (output), `>>` (append)
- **Heredocs** — `<<` reads input until a delimiter is matched
- **Variable expansion** — `$VAR`, `$?` (last exit status), `$$` (shell PID)
- **Quoted strings** — Single quotes preserve literals; double quotes allow expansion
- **Command history** — Navigate previous commands with arrow keys (via readline)
- **Signal handling** — `Ctrl+C` cancels current input; `Ctrl+\` is ignored; `Ctrl+D` exits

## Requirements

- `gcc` or `clang`
- GNU `readline` library

On macOS with Homebrew:
```sh
brew install readline
```

## Build

```sh
make        # Build the minishell executable
make re     # Rebuild from scratch
make clean  # Remove object files
make fclean # Remove object files and the binary
```

## Usage

```sh
./minishell
```

Once running, you get an interactive prompt. Examples:

```sh
# Pipes
ls -la | grep ".c" | wc -l

# Redirections
echo "hello" > file.txt
cat < file.txt >> output.txt

# Heredoc
cat << EOF
line one
line two
EOF

# Variable expansion
export NAME=world
echo "Hello, $NAME! Exit: $?"

# Built-ins
cd -          # go to previous directory
export FOO=bar
unset FOO
exit 0
```

## Project Structure

```
minishell/
├── src/
│   ├── main/       # Entry point, readline loop
│   ├── lexing/     # Tokenizer and variable expansion
│   ├── parsing/    # AST construction and syntax validation
│   ├── command/    # Execution engine (fork, pipe, execve, redirections)
│   ├── builtins/   # Built-in command implementations
│   ├── heredoc/    # Heredoc input handling
│   ├── path/       # PATH resolution for external commands
│   └── utils/      # Error handling and memory utilities
├── libft/          # Custom C library
└── minishell.h     # Shared types and function declarations
```

## Implementation Overview

1. **Lexer** — Tokenizes raw input, handling quotes and variable expansion
2. **Parser** — Builds an Abstract Syntax Tree (AST) from tokens, validates syntax
3. **Heredoc scan** — Collects heredoc content into temp files before execution
4. **Executor** — Walks the AST; forks child processes for pipes and external commands, runs built-ins in the parent process
5. **Cleanup** — Frees all allocated memory and closes file descriptors after each command

## Authors

Built as a group project at 42 (Hive Helsinki).
