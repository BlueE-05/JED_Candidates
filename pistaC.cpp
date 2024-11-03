// carlos gtz
// algoritmo pista C (y simulacion) en c++, no esta hecho para interactuar con los componentes debido a unas complicaciones que tuvimos con nuestro robot, la intencion es mostrar el funcionamiento

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <string>

// dimensiones
const int MAZE_ROWS = 3;
const int MAZE_COLS = 5;

// colores para detectar 
enum class MazeColor {
    UNKNOWN = 0,
    RED = 1,  // final
    YELLOW = 2, 
    PURPLE = 3,
    PINK = 4,
    BLUE = 5,   // inicio
    BLACK = 6   
};

enum class Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

struct Position {
    int row;
    int col;
    Direction facing;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator<(const Position& other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
};


struct MazeCell {
    bool northWall = false;
    bool southWall = false;
    bool eastWall = false;
    bool westWall = false;
    MazeColor color = MazeColor::UNKNOWN;
    bool visited = false;
    bool isBlackZone = false;
};

// laberinto simulado
class MazeSimulation {
private:
    MazeCell realMaze[MAZE_ROWS][MAZE_COLS];

public:
    MazeSimulation() {
        setupTestMaze();
    }

    // nuevo lab
    void setupTestMaze() {
    
        for (int i = 0; i < MAZE_ROWS; i++) {
            for (int j = 0; j < MAZE_COLS; j++) {
                realMaze[i][j] = MazeCell();
            }
        }

        // asignar colores
        realMaze[0][0].color = MazeColor::RED;   
        realMaze[0][1].color = MazeColor::YELLOW;
        realMaze[0][2].color = MazeColor::BLACK; 
        realMaze[0][3].color = MazeColor::PINK;
        realMaze[0][4].color = MazeColor::YELLOW;

        // fila en medio
        realMaze[1][0].color = MazeColor::PURPLE;
        realMaze[1][1].color = MazeColor::YELLOW;
        realMaze[1][2].color = MazeColor::PURPLE;
        realMaze[1][3].color = MazeColor::YELLOW;
        realMaze[1][4].color = MazeColor::PURPLE;

        // fila abajo
        realMaze[2][0].color = MazeColor::PINK;
        realMaze[2][1].color = MazeColor::PURPLE;
        realMaze[2][2].color = MazeColor::PINK;
        realMaze[2][3].color = MazeColor::PURPLE;
        realMaze[2][4].color = MazeColor::BLUE; 

        // paredes
        buildVerticalWall(0, 2);
        buildVerticalWall(1, 2);
        buildVerticalWall(1, 3);

        buildHorizontalWall(0, 2);
        buildHorizontalWall(1, 1);
        buildHorizontalWall(1, 3);
    }

    void buildVerticalWall(int row, int col) {
        if (col > 0) {
            realMaze[row][col].westWall = true;
            realMaze[row][col-1].eastWall = true;
        }
    }

    void buildHorizontalWall(int row, int col) {
        if (row > 0) {
            realMaze[row][col].northWall = true;
            realMaze[row-1][col].southWall = true;
        }
    }

    // deteccion pared
    bool hasWallInFront(const Position& pos) {
        switch (pos.facing) {
            case Direction::NORTH: return pos.row == 0 || realMaze[pos.row][pos.col].northWall;
            case Direction::SOUTH: return pos.row == MAZE_ROWS-1 || realMaze[pos.row][pos.col].southWall;
            case Direction::EAST: return pos.col == MAZE_COLS-1 || realMaze[pos.row][pos.col].eastWall;
            case Direction::WEST: return pos.col == 0 || realMaze[pos.row][pos.col].westWall;
        }
        return false;
    }

    bool hasWallOnLeft(const Position& pos) {
        Direction leftDir = static_cast<Direction>((static_cast<int>(pos.facing) + 3) % 4);
        Position leftPos = pos;
        leftPos.facing = leftDir;
        return hasWallInFront(leftPos);
    }

    bool hasWallOnRight(const Position& pos) {
        Direction rightDir = static_cast<Direction>((static_cast<int>(pos.facing) + 1) % 4);
        Position rightPos = pos;
        rightPos.facing = rightDir;
        return hasWallInFront(rightPos);
    }

    MazeColor getColor(const Position& pos) {
        return realMaze[pos.row][pos.col].color;
    }
};

class Navigator {
private:
    MazeCell knownMaze[MAZE_ROWS][MAZE_COLS];  
    Position robotPos;                          
    std::vector<Position> visitHistory;         
    MazeSimulation& sim;                       
    bool reachedGoal = false;                 
    bool dontTouch = false;              
    std::map<MazeColor, int> colorFrequency;   // conteo colores

public:
    Navigator(MazeSimulation& simulation) 
        : sim(simulation) {
        robotPos = {MAZE_ROWS-1, MAZE_COLS-1, Direction::NORTH};  
        resetExploration();
    }

    void resetExploration() {
        for (int i = 0; i < MAZE_ROWS; i++) {
            for (int j = 0; j < MAZE_COLS; j++) {
                knownMaze[i][j] = MazeCell();
            }
        }
        visitHistory.clear();
        reachedGoal = false;
        dontTouch = false;
        colorFrequency.clear();
    }

    void explore() {
        std::cout << "\nempezando en (" 
                  << robotPos.row << "," << robotPos.col << ")\n";

        while (!reachedGoal) {
            scanCurrentCell();

            // final del maze
            if (sim.getColor(robotPos) == MazeColor::RED) {
                std::cout << "cuadro rojo encontrado\n";
                returnToMostSeenColor();
                break;
            }

            // siguiente celda
            Position nextStep = decidePath();
            if (nextStep == robotPos) {
                std::cout << "lack of progress.\n";
                break;
            }

            // mover 
            moveToPosition(nextStep);
        }
    }

private:
    void scanCurrentCell() {
        MazeCell& cell = knownMaze[robotPos.row][robotPos.col];
        MazeColor cellColor = sim.getColor(robotPos);

        if (!cell.visited) {
            std::cout << "nueva celda (" 
                      << robotPos.row << "," << robotPos.col 
                      << "),  color: " << static_cast<int>(cellColor) << "\n";

            
            if (cellColor != MazeColor::RED && 
                cellColor != MazeColor::BLUE && 
                cellColor != MazeColor::BLACK) {
                colorFrequency[cellColor]++;
            }
        }

        if (cellColor == MazeColor::BLACK) {
            dontTouch= true;
            return;
        }

        // actualizar
        cell.color = cellColor;
        cell.visited = true;

        // checar paredes
        cell.northWall = sim.hasWallInFront({robotPos.row, robotPos.col, Direction::NORTH});
        cell.southWall = sim.hasWallInFront({robotPos.row, robotPos.col, Direction::SOUTH});
        cell.eastWall = sim.hasWallInFront({robotPos.row, robotPos.col, Direction::EAST});
        cell.westWall = sim.hasWallInFront({robotPos.row, robotPos.col, Direction::WEST});

        visitHistory.push_back(robotPos);
    }

    Position decidePath() {
        std::vector<Position> options = getAvailableMoves(robotPos);

        // priorizar investigar nuevas celdas
        for (const Position& pos : options) {
            if (!knownMaze[pos.row][pos.col].visited) {
                return pos;
            }
        }

        // backtracking en caso de ser necesario
        if (!visitHistory.empty()) {
            Position lastPos = visitHistory.back();
            visitHistory.pop_back();
            return lastPos;
        }

        return robotPos;
    }

    std::vector<Position> getAvailableMoves(const Position& pos) {
        std::vector<Position> moves;
        const MazeCell& cell = knownMaze[pos.row][pos.col];

        // checar nuevas celdas disponibles
        if (!cell.northWall && pos.row > 0)
            moves.push_back({pos.row - 1, pos.col, Direction::NORTH});
        if (!cell.southWall && pos.row < MAZE_ROWS - 1)
            moves.push_back({pos.row + 1, pos.col, Direction::SOUTH});
        if (!cell.eastWall && pos.col < MAZE_COLS - 1)
            moves.push_back({pos.row, pos.col + 1, Direction::EAST});
        if (!cell.westWall && pos.col > 0)
            moves.push_back({pos.row, pos.col - 1, Direction::WEST});

        return moves;
    }

    void moveToPosition(const Position& newPos) {
        // que direccion necesio ver
        Direction targetDir;
        if (newPos.row < robotPos.row) targetDir = Direction::NORTH;
        else if (newPos.row > robotPos.row) targetDir = Direction::SOUTH;
        else if (newPos.col > robotPos.col) targetDir = Direction::EAST;
        else targetDir = Direction::WEST;

        // girar a celda (si es necesario)
        if (robotPos.facing != targetDir) {
            std::cout << "girando de " << static_cast<int>(robotPos.facing) 
                      << " a " << static_cast<int>(targetDir) << "\n";
        }

        // mover
        std::cout << "moviendo de (" << robotPos.row << "," << robotPos.col 
                  << ") a (" << newPos.row << "," << newPos.col << ")\n";

        robotPos = newPos;
        robotPos.facing = targetDir;
    }

    void returnToMostSeenColor() {
        // color mas repetido
        MazeColor targetColor = MazeColor::UNKNOWN;
        int maxCount = 0;
        for (const auto& pair : colorFrequency) {
            if (pair.second > maxCount) {
                maxCount = pair.second;
                targetColor = pair.first;
            }
        }

        std::cout << "color mas repetido es " << static_cast<int>(targetColor) << ", " <<maxCount << " veces)\n";

        // regresar a  color msa repetido
        Position targetPos = findNearestColorCell(targetColor);
        moveToPosition(targetPos);

        std::cout << "celda del color encontrada en (" << targetPos.row 
                  << "," << targetPos.col << ")\n";
        reachedGoal = true;
    }

    Position findNearestColorCell(MazeColor targetColor) {
        Position nearest = robotPos;
        int minDist = MAZE_ROWS * MAZE_COLS;

        for (int i = 0; i < MAZE_ROWS; i++) {
            for (int j = 0; j < MAZE_COLS; j++) {
                if (knownMaze[i][j].color == targetColor) {
                    Position pos = {i, j, robotPos.facing};
                    int dist = std::abs(robotPos.row - i) + std::abs(robotPos.col - j);
                    if (dist < minDist) {
                        minDist = dist;
                        nearest = pos;
                    }
                }
            }
        }
        return nearest;
    }
};

int main() {
    MazeSimulation sim;
    Navigator navigator(sim);

    std::cout << "Inicio simulacion\n";
    navigator.explore();

    return 0;
}