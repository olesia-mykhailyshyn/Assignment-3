#include "board.h"
#include <iostream>
#include <sstream>

int main() {
    Board board;
    std::string input;

    while (true) {
        std::cout << "\nEnter command (draw/list/shapes/add/undo/clear/save/load/exit): ";
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
            std::string fillModeStr, color, shapeName;
            int x, y, param1, param2 = 0;

            iss >> fillModeStr >> color >> shapeName >> x >> y >> param1;

            if (shapeName == "rectangle" || shapeName == "line") {
                if (!(iss >> param2)) {
                    std::cout << "Invalid parameters for " << shapeName << ". Needs an additional parameter." << std::endl;
                    continue;
                }
            }

            FillMode fillMode = (fillModeStr == "fill") ? FillMode::Fill : FillMode::Frame;

            board.add(shapeName, color, x, y, param1, param2, fillMode);
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
        else if (command == "select") {
            if (iss.peek() >= '0' && iss.peek() <= '9') {
                int ID;
                iss >> ID;
                board.select(ID);
            }
            else {
                int x, y;
                iss >> x >> y;
                board.select({x, y});
            }
        }
        else if (command == "remove") {
            board.remove();
        }
        else if (command == "edit") {
            int ID, x, y, param1, param2 = 0;
            std::string shapeName;
            iss >> ID >> shapeName >> x >> y >> param1;
            if (shapeName == "rectangle" || shapeName == "line") {
                iss >> param2;
            }
            board.edit(ID, shapeName, x, y, param1, param2);
        }
        else if (command == "paint") {
            std::string color;
            iss >> color;
            board.paint(color);
        }
        else if (command == "move") {
            int x, y;
            iss >> x >> y;
            board.move(x, y);
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}