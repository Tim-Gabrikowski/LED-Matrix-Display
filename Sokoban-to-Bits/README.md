# Sokoban to Bits

Convert Sokoban Level TXT file to 3-bit compressed level format

## Run

Fill level files to `levels/` and run using `node`

```sh
node index.js
```

It creates the C-Array in `levels.c.txt`. Every 38 bytes (1 line) is one of the level files

## Cell Types

| char | bits (RGB) | description |
| :-: | :-: | :- |
| ` ` | `000` | Empty |
| `.` | `001` | Goal |
| `@` | `010` | Player |
| `+` | `011` | Player on a Goal |
| `$` | `100` | Box |
| `*` | `101` | Box on a Goal |
| `#` | `111` | Wall |
