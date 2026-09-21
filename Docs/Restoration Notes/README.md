# Restoration Notes

This directory is an **Obsidian vault** containing the working documentation for the restoration and modernization of the LithTech Jupiter 69 codebase.

The vault is intended to preserve and reconstruct the project's architecture, legacy behavior, implementation details, porting decisions, compatibility constraints, experiments, and other knowledge that may not be obvious from the source code alone.

## Documentation format

The vault uses:

- **Obsidian**
- **TagFolder** for hierarchical tag-based navigation
- **Wikilinks** (`[[...]]`) to represent explicit relationships between concepts, systems, files, and components

Tags and wikilinks are intentionally used as part of the documentation structure so the knowledge base can also be processed by external indexing and graph-analysis tools.

## Documentation quality

This documentation is being reconstructed incrementally.

Some material originated from previous AI-assisted development sessions, including sessions produced with low-cost LLMs. Parts of that material contain a mixture of my native Rioplatense Spanish with Chinese-like sentence structures, unclear wording, inconsistent terminology, or other artifacts introduced during AI-assisted development.

The original material is preserved under:

```text
RAW-BROKEN-SPANISH/
```

This directory is a **raw working source**, not authoritative documentation.

Its contents are being progressively reinterpreted, corrected, consolidated, and rewritten into clear technical English while preserving useful information about the restoration process.

## Documentation status

Documentation may represent different levels of certainty. In particular, distinctions should be made between:

- observed legacy behavior
- verified implementation details
- documented constraints
- reconstructed behavior
- hypotheses and ongoing investigations

When possible, documentation should preserve the evidence or source from which a conclusion was derived.

## Relationship with the source tree

`Restoration Notes` documents the source tree but does not replace it.

The repository source code, build system, tests, original documentation, and restoration notes should be considered complementary sources of information.

`../Original-Docs/` contains the original legacy documentation and reference material from the 2003-era project/toolchain. It is preserved separately from the reconstructed documentation in this vault.

## Intended use

The goal of this vault is not only to describe the current implementation, but to preserve enough context to allow future developers or AI-assisted tools to understand the original design and reproduce or modify the modernized implementation without unnecessarily introducing new architectural or stylistic deviations.