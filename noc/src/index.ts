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
  GiveUp = "give up",
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

interface GiveUpAction {
  type: ActionType.GiveUp;
}

type Action = WaitAction | MoveAction | GiveUpAction;

enum PacketStatus {
  GivenUp = "given up",
  Active = "active",
  Completed = "completed",
}

class Packet {
  history: Array<Action> = [];
  position: Coordinate;
  subsequentRoundsWaiting = 0;
  status = PacketStatus.Active;
  constructor(
    public origin: Coordinate,
    public destination: Coordinate,
    public data: number,
    public id: number
  ) {
    this.position = { ...origin };
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
  let activePackets = packets.filter(
    (packet) => packet.status === PacketStatus.Active
  );
  while (activePackets.length > 0) {
    for (let packet of activePackets) {
      const nextDirection = calculateNextDirection(packet);
      const nextPosition = move(packet.position, nextDirection);
      const { hasPacketInIt, packetCulprit } = cellHasPacketInIt(
        nextPosition,
        packets
      );
      if (hasPacketInIt) {
        packet.history.push({
          type: ActionType.Wait,
          packetCulprit: packetCulprit!,
        });
        packet.subsequentRoundsWaiting++;
        if (packet.subsequentRoundsWaiting === 10) {
          packet.history.push({
            type: ActionType.GiveUp,
          });
          packet.status = PacketStatus.GivenUp;
        }
      } else {
        packet.history.push({
          type: ActionType.Move,
          direction: nextDirection,
        });
        packet.position = nextPosition;
        if (equalCoordinates(packet.position, packet.destination)) {
          packet.status = PacketStatus.Completed;
        }
      }
    }
    activePackets = packets.filter(
      (packet) => packet.status === PacketStatus.Active
    );
  }

  for (let packet of packets) {
    console.log(`Stats for packet ${packet.id}`);
    console.log(
      `\tTotal amount of hops: ${
        packet.history.filter((action) => action.type === ActionType.Move)
          .length
      }`
    );
    console.log("\tHistory:");
    let pos = packet.origin;
    for (let action of packet.history) {
      if (action.type === ActionType.Move) {
        let nextPos = move(pos, action.direction);
        console.log(
          `\t - Move from (${pos.x},${pos.y}) to (${nextPos.x},${nextPos.y})`
        );
        pos = nextPos;
      } else if (action.type === ActionType.Wait) {
        console.log(
          `\t - Waiting at (${pos.x},${pos.y}) because of packet ${action.packetCulprit}`
        );
      } else if (action.type === ActionType.GiveUp) {
        console.log(`\t - Gave up`);
      }
    }
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
      packet.status === PacketStatus.Active
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

function equalCoordinates(a: Coordinate, b: Coordinate) {
  return a.x === b.x && a.y === b.y;
}

const config = JSON.parse(
  fs.readFileSync(path.join(__dirname, "config.json")).toString()
) as Config;

main(config);
