# Mirkiel: Learning and Crafting Interpreters

Welcome to my repository dedicated to the journey of understanding and building programming language interpreters. In short [Crafting interpeters](https://craftinginterpreters.com/).

## Git Repository Management Tips

### How to Remove an Accidental Git Repository

If you accidentally initialized a Git repository in your root directory or any other wrong location, follow these steps to safely remove it:

1. Navigate to the directory that was accidentally made into a Git repository
   ```bash
   cd path/to/accidental/repo
   ```

2. Delete the hidden `.git` directory (this removes Git tracking without affecting your files)
   - On Windows:
     ```bash
     rmdir /s /q .git
     ```
   - On macOS/Linux:
     ```bash
     rm -rf .git
     ```

3. Verify that the repository has been removed:
   ```bash
   git status
   ```
   You should see a message like "fatal: not a git repository"

**Note:** Be extremely careful when using `rm -rf` commands. Double-check your current directory before executing.
