# SystemLibraryLoader

## Purpose

Loads the real `version` implementation for the proxy:

1. Prefer `version_orig.dll` next to this module (no Wine name collision).
2. Fall back to system32 `version.dll` with `LOAD_LIBRARY_SEARCH_SYSTEM32` when possible.
