import fs from "fs";
import path from "path";

interface Coordinate {
  x: number;
  y: number;
}
interface Config {
  blockedCells: Array<Coordinate>;
  packets: Array<{
    origin: Coordinate;
    destination: Coordinate;
    payload: number;
  }>;
}

enum ActionType {
  Move = "move",
  Wait = "wait",
}

enum Direction {
  North = "north",
  South = "south",
  East = "east",
  West = "west",
}
interface MoveAction {
  type: ActionType.Move;
  direction: Direction;
}

interface WaitAction {
  type: ActionType.Wait;
  packetCulprit: number;
}

type Action = WaitAction | MoveAction;

class Packet {
  history: Array<Action> = [];
  position: Coordinate;
  constructor(
    public origin: Coordinate,
    public destination: Coordinate,
    public data: number,
    public id: number
  ) {
    this.position = { ...origin };
  }

  hasReachedDestination() {
    return (
      this.position.x === this.destination.x &&
      this.position.y === this.destination.y
    );
  }
}

function main(config: Config) {
  const blockedCells = new Set<string>(
    config.blockedCells.map((cell) => `${cell.x},${cell.y}`)
  );
  const packets = config.packets.map(
    (packet, i) =>
      new Packet(packet.origin, packet.destination, packet.payload, i)
  );
  while (packets.some((packet) => !packet.hasReachedDestination())) {
    for (
      let i = 0;
      i < packets.filter((packet) => !packet.hasReachedDestination()).length;
      i++
    ) {
      const packet = packets[i];
      const nextDirection = calculateNextDirection(packet);
      const nextPosition = move(packet.position, nextDirection);
      const { hasPacketInIt, packetCulprit } = cellHasPacketInIt(
        nextPosition,
        packets
      );
      if (hasPacketInIt)
        packet.history.push({
          type: ActionType.Wait,
          packetCulprit: packetCulprit!,
        });
      else {
        packet.history.push({
          type: ActionType.Move,
          direction: nextDirection,
        });
        packet.position = nextPosition;
      }
    }
  }

  for (let packet of packets) {
    console.log(packet.history);
  }
}

function calculateNextDirection(packet: Packet): Direction {
  if (packet.position.x < packet.destination.x) {
    return Direction.East;
  }
  if (packet.position.x > packet.destination.x) {
    return Direction.West;
  }
  if (packet.position.y < packet.destination.y) {
    return Direction.South;
  }
  return Direction.North;
}

function cellHasPacketInIt(cell: Coordinate, packets: Array<Packet>) {
  const packetCulprit = packets.find(
    (packet) =>
      packet.position.x === cell.x &&
      packet.position.y === cell.y &&
      !packet.hasReachedDestination()
  );
  return {
    hasPacketInIt: packetCulprit !== undefined,
    packetCulprit: packetCulprit?.id,
  };
}

function move(position: Coordinate, direction: Direction): Coordinate {
  if (direction === Direction.East) {
    return { x: position.x + 1, y: position.y };
  }
  if (direction === Direction.West) {
    return { x: position.x - 1, y: position.y };
  }
  if (direction === Direction.South) {
    return { x: position.x, y: position.y + 1 };
  }
  return { x: position.x, y: position.y - 1 };
}

const config = JSON.parse(
  fs.readFileSync(path.join(__dirname, "config.json")).toString()
) as Config;

main(config);
