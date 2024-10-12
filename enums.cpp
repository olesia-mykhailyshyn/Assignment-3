#include "enums.h"

const std::unordered_map<std::string, ShapeType> shapeTypeMap = {
        {"triangle", ShapeType::Triangle},
        {"rectangle", ShapeType::Rectangle},
        {"circle", ShapeType::Circle},
        {"line", ShapeType::Line}
};

const std::unordered_map<std::string, CommandType> commandMap = {
        {"add", CommandType::Add},
        {"draw", CommandType::Draw},
        {"list", CommandType::List},
        {"shapes", CommandType::Shapes},
        {"undo", CommandType::Undo},
        {"clear", CommandType::Clear},
        {"save", CommandType::Save},
        {"load", CommandType::Load},
        {"exit", CommandType::Exit},
        {"select", CommandType::Select},
        {"remove", CommandType::Remove},
        {"edit", CommandType::Edit},
        {"paint", CommandType::Paint},
        {"move", CommandType::Move}
};