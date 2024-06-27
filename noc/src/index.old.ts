type Direction = "NORTH" | "SOUTH" | "EAST" | "WEST";

interface Coordinate {
  x: number;
  y: number;
}

interface Packet {
  source: Coordinate;
  destination: Coordinate;
}

const GRID_SIZE = 8;

function isBlocked(cell: Coordinate, blockedCells: Set<string>): boolean {
  return blockedCells.has(`${cell.x},${cell.y}`);
}

function isWithinGrid(cell: Coordinate): boolean {
  return cell.x >= 0 && cell.x < GRID_SIZE && cell.y >= 0 && cell.y < GRID_SIZE;
}

function getNextDirection(
  current: Coordinate,
  destination: Coordinate,
  hasMovedWest: boolean,
  blockedCells: Set<string>
): Direction | null {
  const possibleDirections: Direction[] = [];

  if (
    !hasMovedWest &&
    !isBlocked({ x: current.x - 1, y: current.y }, blockedCells)
  ) {
    possibleDirections.push("WEST");
  }
  if (!isBlocked({ x: current.x, y: current.y + 1 }, blockedCells)) {
    possibleDirections.push("NORTH");
  }
  if (!isBlocked({ x: current.x, y: current.y - 1 }, blockedCells)) {
    possibleDirections.push("SOUTH");
  }
  if (!isBlocked({ x: current.x + 1, y: current.y }, blockedCells)) {
    possibleDirections.push("EAST");
  }

  if (possibleDirections.length === 0) {
    return null;
  }

  if (destination.x < current.x && possibleDirections.includes("WEST")) {
    return "WEST";
  }
  if (destination.y > current.y && possibleDirections.includes("NORTH")) {
    return "NORTH";
  }
  if (destination.y < current.y && possibleDirections.includes("SOUTH")) {
    return "SOUTH";
  }
  if (destination.x > current.x && possibleDirections.includes("EAST")) {
    return "EAST";
  }

  // If none of the above conditions are met, return a random direction
  return possibleDirections[
    Math.floor(Math.random() * possibleDirections.length)
  ];
}

function move(current: Coordinate, direction: Direction): Coordinate {
  switch (direction) {
    case "NORTH":
      return { x: current.x, y: current.y + 1 };
    case "SOUTH":
      return { x: current.x, y: current.y - 1 };
    case "EAST":
      return { x: current.x + 1, y: current.y };
    case "WEST":
      return { x: current.x - 1, y: current.y };
    default:
      return current;
  }
}

function westFirstRouting(
  packet: Packet,
  blockedCells: Set<string>
): Coordinate[] {
  const path: Coordinate[] = [];
  let current: Coordinate = { ...packet.source };
  let hasMovedWest = false;

  while (
    current.x !== packet.destination.x ||
    current.y !== packet.destination.y
  ) {
    const direction = getNextDirection(
      current,
      packet.destination,
      hasMovedWest,
      blockedCells
    );

    console.log(current);

    if (direction === "WEST") {
      hasMovedWest = true;
    }

    if (direction) {
      current = move(current, direction);
      path.push({ ...current });
    } else {
      break; // No valid direction found, could be blocked
    }
  }

  return path;
}

// Example Usage
const packet: Packet = {
  source: { x: 4, y: 4 },
  destination: { x: 1, y: 7 },
};

const blockedCells = new Set<string>([
  "3,4", // Example blocked cells
  "2,4",
  "2,5",
  "1,6",
]);

const path = westFirstRouting(packet, blockedCells);
console.log("Path:", path);
