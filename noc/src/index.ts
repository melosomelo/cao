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
  targetRouter: Coordinate;
  targetRouterPort: Direction;
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

class Router {
  blocked: boolean = false;
  buffersCapacity: Record<Direction, number> = {
    [Direction.East]: 0,
    [Direction.West]: 0,
    [Direction.North]: 0,
    [Direction.South]: 0,
  };
}

function main(config: Config) {
  const routers: Array<Array<Router>> = new Array(8).fill(
    new Array(8).fill(new Router())
  );
  const packets = config.packets.map(
    (packet, i) =>
      new Packet(packet.origin, packet.destination, packet.payload, i)
  );
  packets.forEach(
    (packet) =>
      routers[packet.position.y][packet.position.x].buffersCapacity[
        Direction.North
      ]++
  );
  let activePackets = packets.filter(
    (packet) => packet.status === PacketStatus.Active
  );
  while (activePackets.length > 0) {
    for (let packet of activePackets) {
      console.log(routers);
      const nextDirection = calculateNextDirection(packet);
      const nextPosition = move(packet.position, nextDirection);
      const currentRouter = routers[packet.position.y][packet.position.x];
      const nextRouter = routers[nextPosition.y][nextPosition.x];
      const { hasPacketInIt, packetCulprit } = cellHasPacketInIt(
        nextPosition,
        packets
      );
      if (canEnterRouter(nextRouter, oppositeDirection(nextDirection))) {
        const packetLastMovement = getPacketLastMovement(packet);
        if (packetLastMovement !== null) {
          // decrement current router port counter
          currentRouter.buffersCapacity[
            oppositeDirection(packetLastMovement.direction)
          ] -= 1;
        }
        // increment next router port counter
        nextRouter.buffersCapacity[oppositeDirection(nextDirection)] += 1;
        // update packet position and history
        packet.position = nextPosition;
        packet.history.push({
          type: ActionType.Move,
          direction: nextDirection,
        });
        packet.subsequentRoundsWaiting = 0;
        if (equalCoordinates(packet.position, packet.destination)) {
          packet.status = PacketStatus.Completed;
        }
      } else {
        // wait
        packet.history.push({
          type: ActionType.Wait,
          targetRouter: { x: nextPosition.x, y: nextPosition.y },
          targetRouterPort: oppositeDirection(nextDirection),
        });
        packet.subsequentRoundsWaiting++;
        if (packet.subsequentRoundsWaiting === 10) {
          packet.history.push({
            type: ActionType.GiveUp,
          });
          packet.status = PacketStatus.GivenUp;
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
          `\t - Waiting to enter router (${action.targetRouter.x},${action.targetRouter.y}) at port ${action.targetRouterPort}`
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

function canEnterRouter(router: Router, port: Direction) {
  return router.buffersCapacity[port] < 4;
}

function oppositeDirection(direction: Direction) {
  if (direction === Direction.South) return Direction.North;
  if (direction === Direction.North) return Direction.South;
  if (direction === Direction.East) return Direction.West;
  return Direction.East;
}

function getPacketLastMovement(packet: Packet) {
  for (let i = packet.history.length - 1; i >= 0; i--) {
    if (packet.history[i].type === ActionType.Move) {
      return packet.history[i] as MoveAction;
    }
  }
  return null;
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
