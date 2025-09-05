#pragma once

#include <cstdint>
#include <string_view>
#include <cstring>
#include <string>
#include <filesystem>
#include <imgui.h>

namespace fs = std::filesystem;

class WindowClass
{
public:
    //size for character array
    static constexpr auto bufferSize = std::size_t{1024};
    static constexpr auto popUpFlags = ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar;

    static constexpr auto popUpSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popUpButtonSize = ImVec2(120.0F, 0.0F);  //with 0, width will be automaticaclly detected
    static constexpr auto popUpPos =
            ImVec2(1280.0F / 2.0F - popUpSize.x / 2.0F,
                    720.0F / 2.0F - popUpSize.y / 2.0F);

    //place in the middle of the window
            //ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popUpSize.x / 2.0F,
            //       ImGui::GetIO().DisplaySize.y / 2.0F - popUpSize.y / 2.0F);


public:
//inside constructor, initialize an empty string
    WindowClass() : currentFilename({})
    {
        std::memset(textBuffer, 0, bufferSize);  //set text buffer to 0 bytes, with bufferSize value 1024
    }

    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawContent();
    void DrawInfo();
    void DrawSavePopup();
    void DrawLoadPopup();

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
        //use character array
    char textBuffer[bufferSize];
    std::string currentFilename;
};

void render(WindowClass &window_obj);
