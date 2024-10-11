#include "board.h"
#include <iostream>
#include <sstream>

int main() {
    Board board;
    std::string input;

    while (true) {
        std::cout << "\nEnter command (draw/list/shapes/add/undo/clear/save/load/exit): " << std::endl;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "draw") {
            board.draw();
        }
        else if (command == "list") {
            board.list();
        }
        else if (command == "shapes") {
            Board::shapes();
        }
        else if (command == "add") {
            std::string shapeName;
            int x, y, param1, param2 = 0;

            iss >> shapeName >> x >> y >> param1;
            if (shapeName == "rectangle" || shapeName == "line") {
                iss >> param2;
            }

            board.add(shapeName, x, y, param1, param2);
        }
        else if (command == "undo") {
            board.undo();
        }
        else if (command == "clear") {
            board.clear(board.getFilePath());
        }
        else if (command == "save") {
            board.save(board.getFilePath());
        }
        else if (command == "load") {
            board.load(board.getFilePath());
        }
        else if (command == "exit") {
            std::cout << "Exiting the program." << std::endl;
            break;
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}