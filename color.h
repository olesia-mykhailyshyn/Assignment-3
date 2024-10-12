#pragma once
#include <string>
#include <unordered_map>

enum class ColorName {
    Red, Green, Blue, Yellow, Cyan, Magenta, White, Reset, Invalid
};

class Color {
public:
    explicit Color(ColorName name = ColorName::Reset);
    [[nodiscard]] std::string getName() const;
    static ColorName fromString(const std::string& colorStr);

    ColorName name;
};

class ColorFormatter {
public:
    static std::string getAnsiCode(const Color& color);
};