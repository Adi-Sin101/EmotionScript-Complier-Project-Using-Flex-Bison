from pathlib import Path
lines = Path('test.ems').read_text().splitlines()
for i in [91,99,111,115,121,141,143,151,155,159,163,173,175,245,249,251,267,269,279,281]:
    if i <= len(lines):
        print(i, lines[i-1])
