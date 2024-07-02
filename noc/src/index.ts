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
  const blockedCells = new Set(
    config.blockedCells.map((coordinate) => `${coordinate.x},${coordinate.y}`)
  );
  const routers: Array<Array<Router>> = Array.from({ length: 8 }, () =>
    Array.from({ length: 8 }, () => new Router())
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
      // Implement BFS for packet movement
      const path = bfs(
        packet.position,
        packet.destination,
        blockedCells,
        routers
      );
      if (path.length > 1) {
        const nextStep = path[1]; // Get the next step in the path
        const nextDirection = getDirection(packet.position, nextStep);
        const nextRouter = routers[nextStep.y][nextStep.x];
        // Desired router port is not full.
        if (canEnterRouter(nextRouter, oppositeDirection(nextDirection))) {
          const packetLastMovement = getPacketLastMovement(packet);
          // Update the value of the port of the current router to reflect the packet leaving.
          if (packetLastMovement !== null) {
            const currentRouter = routers[packet.position.y][packet.position.x];
            currentRouter.buffersCapacity[
              oppositeDirection(packetLastMovement.direction)
            ] -= 1;
          }
          // Updating the value of the port of the next router to reflect the packet arriving.
          nextRouter.buffersCapacity[oppositeDirection(nextDirection)] += 1;
          packet.history.push({
            type: ActionType.Move,
            direction: nextDirection,
          });
          packet.position = nextStep;
          packet.subsequentRoundsWaiting = 0;
        } else {
          packet.history.push({
            type: ActionType.Wait,
            targetRouter: nextStep,
            targetRouterPort: oppositeDirection(nextDirection),
          });
          packet.subsequentRoundsWaiting++;
          if (packet.subsequentRoundsWaiting === 10) {
            packet.history.push({ type: ActionType.GiveUp });
            packet.status = PacketStatus.GivenUp;
          }
          continue;
        }
      } else {
        packet.history.push({ type: ActionType.GiveUp });
        packet.status = PacketStatus.GivenUp;
      }

      if (
        packet.position.x === packet.destination.x &&
        packet.position.y === packet.destination.y
      ) {
        const packetLastMovement = getPacketLastMovement(packet);
        if (packetLastMovement !== null) {
          const currentRouter = routers[packet.position.y][packet.position.x];
          currentRouter.buffersCapacity[
            oppositeDirection(packetLastMovement.direction)
          ] -= 1;
        }
        packet.status = PacketStatus.Completed;
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
        let nextPos = getNeighbor(pos, action.direction);
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

function getPacketLastMovement(packet: Packet) {
  for (let i = packet.history.length - 1; i >= 0; i--) {
    const action = packet.history[i];
    if (action.type === ActionType.Move) return action as MoveAction;
  }
  return null;
}

function canEnterRouter(router: Router, from: Direction) {
  return router.buffersCapacity[from] < 4;
}

function oppositeDirection(direction: Direction) {
  switch (direction) {
    case Direction.East:
      return Direction.West;
    case Direction.North:
      return Direction.South;
    case Direction.West:
      return Direction.East;
    default:
      return Direction.North;
  }
}

function getNeighbor(pos: Coordinate, direction: Direction): Coordinate {
  switch (direction) {
    case Direction.North:
      return { x: pos.x, y: pos.y - 1 };
    case Direction.South:
      return { x: pos.x, y: pos.y + 1 };
    case Direction.East:
      return { x: pos.x + 1, y: pos.y };
    case Direction.West:
      return { x: pos.x - 1, y: pos.y };
    default:
      return pos;
  }
}

function getDirection(from: Coordinate, to: Coordinate): Direction {
  if (to.x > from.x) return Direction.East;
  if (to.x < from.x) return Direction.West;
  if (to.y > from.y) return Direction.South;
  if (to.y < from.y) return Direction.North;
  throw new Error("Invalid direction");
}

function bfs(
  start: Coordinate,
  goal: Coordinate,
  blockedCells: Set<string>,
  routers: Array<Array<Router>>
): Coordinate[] {
  const queue: Array<{ coord: Coordinate; path: Coordinate[] }> = [
    { coord: start, path: [start] },
  ];
  const visited = new Set<string>();
  visited.add(`${start.x},${start.y}`);

  while (queue.length > 0) {
    const { coord, path } = queue.shift()!;

    if (coord.x === goal.x && coord.y === goal.y) {
      return path;
    }

    for (const direction of [
      Direction.North,
      Direction.South,
      Direction.East,
      Direction.West,
    ]) {
      const neighbor = getNeighbor(coord, direction);

      if (
        neighbor.x >= 0 &&
        neighbor.x < 8 &&
        neighbor.y >= 0 &&
        neighbor.y < 8 &&
        !blockedCells.has(`${neighbor.x},${neighbor.y}`) &&
        !visited.has(`${neighbor.x},${neighbor.y}`)
      ) {
        queue.push({ coord: neighbor, path: path.concat([neighbor]) });
        visited.add(`${neighbor.x},${neighbor.y}`);
      }
    }
  }

  return []; // Return an empty array if no path is found
}

const config = JSON.parse(
  fs.readFileSync(path.join(__dirname, "config.json")).toString()
) as Config;

main(config);
