# Git Workflow for GroundhogDay Project

This document outlines the Git workflow for our GroundhogDay game project.

## Repository Structure

Our Git repository follows a standard branching model:

- `main`: Production-ready code
- `develop`: Integration branch for development
- `feature/*`: Feature branches
- `bugfix/*`: Bug fix branches
- `release/*`: Release preparation branches

## Getting Started

### Initial Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/groundhog-day-game.git
   cd groundhog-day-game
   ```

2. Set up your Git user information (if not already set):
   ```bash
   git config --global user.name "Your Name"
   git config --global user.email "your.email@example.com"
   ```

### Daily Development Workflow

1. Always pull the latest changes before starting work:
   ```bash
   git checkout develop
   git pull origin develop
   ```

2. Create a new branch for your feature or bugfix:
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. Make your changes, then commit regularly:
   ```bash
   git add .
   git commit -m "Add feature: description of changes"
   ```

4. Push your branch to the remote repository:
   ```bash
   git push -u origin feature/your-feature-name
   ```

5. Create a Pull Request on GitHub/GitLab when your feature is complete.

## Commit Message Guidelines

Good commit messages help track changes and understand the project history. Use the following format:

```
[type]: Short description (50 chars or less)

More detailed explanation if necessary. Wrap at around 72 characters.
Explain what and why, not how (the code shows that).

Closes #123, #456
```

Where `[type]` is one of:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Formatting, missing semicolons, etc; no code change
- `refactor`: Refactoring code
- `test`: Adding tests, refactoring tests; no production code change
- `chore`: Updating build tasks, package manager configs, etc; no production code change

## Handling Large Files

We use `.gitignore` to avoid committing unnecessary files like:
- `node_modules/`
- Generated images and videos
- Temporary files
- API keys and sensitive data

If you need to track large binary files, consider using Git LFS.

## Best Practices

1. **Commit Often**: Make small, focused commits that do one thing well.
2. **Pull Before Push**: Always pull the latest changes before pushing.
3. **Use Meaningful Branch Names**: Name branches to reflect what they contain.
4. **Keep Branches Short-Lived**: Merge or delete feature branches once integrated.
5. **Write Good Commit Messages**: Be clear and descriptive.
6. **Don't Commit Generated Files**: Let users generate their own files based on source code.

## Troubleshooting

### Resolving Merge Conflicts

1. When a conflict occurs, Git will mark the files with conflict markers:
   ```
   <<<<<<< HEAD
   Your changes
   =======
   Their changes
   >>>>>>> branch-name
   ```

2. Edit the files to resolve conflicts, then:
   ```bash
   git add .
   git commit -m "Resolve merge conflicts"
   ```

### Reverting Mistakes

- Discard changes in working directory:
  ```bash
  git checkout -- filename
  ```

- Undo the last commit but keep the changes:
  ```bash
  git reset --soft HEAD~1
  ```

- Completely undo the last commit:
  ```bash
  git reset --hard HEAD~1
  ```

## Questions?

If you have any questions about the Git workflow, please contact the project maintainer.
