let lvl = [
  ["#", "#", "#", "#", "#", " ", " ", " ", " ", " "],
  ["#", "@", "$", ".", "#", " ", " ", " ", " ", " "],
  ["#", "#", "#", "#", "#", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
  [" ", " ", " ", " ", " ", " ", " ", " ", " ", " "],
];

let moved = centerLevel(lvl);
display(moved);

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
  return Math.round(o) - 1;
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
  return Math.round(o) - 1;
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
