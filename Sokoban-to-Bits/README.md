# Sokoban to Bits

Convert Sokoban Level TXT file to 3-bit compressed level format

## Run

Fill level files to `levels/` and run using `node`

```sh
node index.js
```

It creates the C-Array in `levels.c.txt`. Every 38 bytes (1 line) is one of the level files

## Cell Types

| char | bits | description |
| :-: | :-: | :- |
| ` ` | `000` | Empty |
| `@` | `001` | Player |
| `#` | `010` | Wall |
| `.` | `011` | Goal |
| `$` | `100` | Box |
| `+` | `101` | Player on a Goal |
| `*` | `110` | Box on a Goal |

