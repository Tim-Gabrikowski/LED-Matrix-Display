const fs = require("fs");
const path = require("path");

let levels = fs.readdirSync(path.join(__dirname, "levels"));

let parsedLevels = [];

for (let l = 0; l < levels.length; l++) {
  let p = path.join(levels[l]);
  let level = parseLevel(p);
  parsedLevels.push(level);
}
let s = "{\n\t" + parsedLevels.join(",\n\t") + "\n}";
fs.writeFileSync(path.join(__dirname, "level.c.txt"), s);

function parseLevel(l) {
  const level = fs.readFileSync(path.join(__dirname, "levels", l)).toString();

  let lines = level.split("\n").map((line) => line.split(""));

  lines = [...lines, ...Array(10).fill(Array(10).fill(" "))].splice(0, 10);

  lines = lines.map((l) => [...l, ...Array(10).fill(" ")].splice(0, 10));

  let out = "";

  for (let i = 0; i < lines.length; i++) {
    for (let j = 0; j < lines[i].length; j++) {
      switch (lines[i][j]) {
        case ".": // Goal
          out += "001";
          break;
        case "@": // Player
          out += "010";
          break;
        case "+": // Player on Goal
          out += "011";
          break;
        case "$": // Box
          out += "100";
          break;
        case "*": // Box on Goal
          out += "101";
          break;
        case "#": // Wall
          out += "111";
          break;
        default: // empty / default
          out += "000";
          break;
      }
    }
  }

  let bytes = out.match(/.{1,8}/g).map((byte) => parseInt(byte, 2));

  let hex = bytes.map((byte) => byte.toString(16).padStart(2, "0"));

  return hex.map((b) => "0x" + b).join(", ");
}
