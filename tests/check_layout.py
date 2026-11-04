#!/usr/bin/env python3
"""Enforce lowercase snake_case source files and independent smvk-owned paths."""
from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parents[1]
errors = []
allowed = re.compile(r"^[a-z][a-z0-9_]*\.(?:h|cpp|inl|hlsli)$")
for directory in ("include", "src", "shaders"):
    for path in (root / directory).rglob("*"):
        if not path.is_file():
            continue
        relative = path.relative_to(root)
        if any(part == "Engine" for part in relative.parts):
            errors.append(f"legacy Engine path: {relative}")
        if path.suffix in (".h", ".cpp", ".inl", ".hlsli"):
            if not allowed.fullmatch(path.name):
                errors.append(f"non-snake_case source name: {relative}")

# Our standalone public umbrella and RHI-owned header paths must never
# depend on the old Engine/smvk.h or Engine/RHI/ paths.
for path in (root / "include").rglob("*.h"):
    body = path.read_text(encoding="utf-8")
    for legacy in ("<Engine/smvk.h>", "<Engine/RHI/", "<Engine/Rendergraph/"):
        if legacy in body:
            errors.append(f"legacy include {legacy} in {path.relative_to(root)}")

if errors:
    print("\n".join(errors), file=sys.stderr)
    sys.exit(1)
print("smvk layout checks passed")
