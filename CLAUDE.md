# Project Instructions

## Repository understanding

Graphify is the primary mechanism for understanding the structure
and relationships of this repository.

Before exploring source files directly, prefer Graphify to establish
the relevant architecture and dependencies.

Preferred investigation order:

1. Graphify
2. Read the specific files identified by Graphify
3. Grep / rg / find / Glob when Graphify is insufficient
4. Broad repository exploration only when necessary

Do not avoid grep, rg, find, Glob, Read, or other filesystem tools.
They remain valid fallback mechanisms.

Use Graphify especially for:

- architecture questions
- locating relevant components
- callers and callees
- dependency relationships
- tracing execution paths
- locating implementations of a feature
- determining which files are relevant to a task

When Graphify and the current source disagree, the current source
takes precedence.

## Tool usage

Prefer the smallest tool and smallest amount of source required
to answer the question.

Do not read large numbers of unrelated files merely to reconstruct
repository structure when Graphify can provide that structure.

Use direct source inspection when exact implementation details,
current state, generated code, or unsupported constructs are involved.

## Changes

Before modifying code:

1. Understand the relevant architecture.
2. Identify the smallest set of affected files.
3. Inspect the relevant implementation.
4. Make the smallest coherent change.
5. Verify the result.

After significant source changes, update the Graphify index.

## Verification

Do not claim that code works without verification.

Prefer project-native build, test, lint, or static-analysis commands.

When verification cannot be performed, state what was and was not verified.
