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

  lines = centerLevel(lines);
  display(lines);

  let out = "";

  let playerX = 0;
  let playerY = 0;

  for (let i = 0; i < lines.length; i++) {
    for (let j = 0; j < lines[i].length; j++) {
      switch (lines[i][j]) {
        case ".": // Goal
          out += "001";
          break;
        case "@": // Player
          out += "000";
          playerX = j;
          playerY = i;
          break;
        case "+": // Player on Goal
          out += "001";
          playerX = j;
          playerY = i;
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

  let bytes = [
    playerX,
    playerY,
    ...out.match(/.{1,8}/g).map((byte) => parseInt(byte, 2)),
  ];

  let hex = bytes.map((byte) => byte.toString(16).padStart(2, "0"));

  return hex.map((b) => "0x" + b).join(", ");
}

// Function to center a level in a 10x10 grid
function centerLevel(level) {
  let offsetY = getOffsetY(level);
  let offsetX = getOffsetX(level);
  console.log("Offset X:", offsetX, "Offset Y: " + offsetY);
  return move(level, offsetX, offsetY);
}

function display(level) {
  let horizontal = "+" + Array(10).fill("-").join("") + "+\n";
  let s =
    horizontal +
    level.map((l) => "|" + l.join("") + "|").join("\n") +
    "\n" +
    horizontal;
  console.log(s);
}

function getOffsetY(level) {
  let h = getHeight(level);
  let o = 10 / 2 - h / 2;
  return Math.floor(o);
}
function getHeight(level) {
  let h = 0;
  for (let i = 0; i < level.length; i++) {
    if (level[i].some((l) => l !== " ")) h = i + 1;
  }
  return h;
}
function getOffsetX(level) {
  let w = getWidth(level);
  let o = 10 / 2 - w / 2;
  return Math.floor(o);
}
function getWidth(level) {
  let lines = level.map((l) => removeTrailingSpaces(l)).sort((a, b) => b - a);
  return lines[0];
}

function removeTrailingSpaces(line) {
  for (let i = line.length - 1; i >= 0; i--) {
    if (line[i] !== " ") {
      return i + 1;
    }
  }
}

function move(level, x, y) {
  for (let i = 0; i < y; i++) {
    level = [...arrayRotate(level, true)];
  }
  for (let i = 0; i < x; i++) {
    level = level.map((l) => [...arrayRotate(l, true)]);
  }
  return level;
}

function arrayRotate(arr, reverse) {
  if (reverse) arr.unshift(arr.pop());
  else arr.push(arr.shift());
  return arr;
}
