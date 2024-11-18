# Contributing to the Project

Thank you for your interest in contributing to this project! This guide outlines the standards and best practices to follow for contributing effectively and maintaining code quality.

---

## Commit Standards

- `ADD`: adding a new feature.
- `FIX`: fixing a bug.
- `RM`: remove functionnality.
- `DOCS`: updating or adding documentation.
- `REFACT`: refactoring code without changing functionality.
- `TEST`: adding or updating tests.
- `MERGE`: merge branch with pull request

---

## Description Standards

- Provide a clear and detailed description of your changes.
- **What ?** Describe what has been changed.
- **Why ?** Explain why this change is necessary.
- **How ?** Provide technical details if needed.
- Be concise and professional in your explanations.

---

## Pull Request (PR) Standards

1. **PR Title**:
- Follow the commit message format.
- Example: `MERGE: {{TICKET-REF}} {{TICKET-NAME}}`.

2. **PR Description**:
- **Context**: Why is this change important?
- **Major Changes**: List the key changes made.
- **Tests Performed**: Indicate if you’ve added or executed tests.
- **Link to Issues**: Use `Closes #<issue number>` to associate the PR with an issue.
- Example:
  ```
  ### Context
  This PR add new features to the server.

  ### Major Changes
  - Added multiple client acceptation.
  - Fixed server performance.

  ### Tests
  - Verified multiple client acceptation.
  ```