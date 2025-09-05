#include <iostream>
#include <string_view>
#include <string>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawMenu();
    DrawContent();
    DrawInfo();
    DrawSavePopup();
    DrawLoadPopup();

    ImGui::End();
}

void WindowClass::DrawMenu()
{
    //get which keys are pressed (IsKeyPressed doesn't work for Ctrl key)
    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);
    const auto s_pressed = ImGui::IsKeyPressed(ImGuiKey_S);
    const auto l_pressed = ImGui::IsKeyPressed(ImGuiKey_L);

    if (ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrl_pressed && l_pressed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        std::memset(textBuffer, 0, bufferSize);
    }


}

void WindowClass::DrawSavePopup()
{
    static char saveFileNameBuffer[256];  //256 length char array
    const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(popUpPos);

    if (ImGui::BeginPopupModal("Save File", nullptr, popUpFlags))  //needs to match string in referring open popup function
    {
        ImGui::InputText("Filename", saveFileNameBuffer, sizeof(saveFileNameBuffer));

        if (ImGui::Button("Save", popUpButtonSize))
        {
            SaveToFile(saveFileNameBuffer);
            currentFilename = saveFileNameBuffer;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void WindowClass::DrawLoadPopup()
{
    static char loadFileNameBuffer[256];  //256 length char array
    const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(popUpPos);

    if (ImGui::BeginPopupModal("Load File", nullptr, popUpFlags))  //needs to match string in referring open popup function
    {
        ImGui::InputText("Filename", loadFileNameBuffer, sizeof(loadFileNameBuffer));

        if (ImGui::Button("Load", popUpButtonSize))
        {
            LoadFromFile(loadFileNameBuffer);
            currentFilename = loadFileNameBuffer;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void WindowClass::DrawContent()
{
    static constexpr auto inputTextSize = ImVec2(1200.0F, 625.0F);
    static constexpr auto inputTextFlags =
        ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll;
    static constexpr auto lineNumberSize = ImVec2(30.0F, inputTextSize.y);

        //creates window inside already existing window
    ImGui::BeginChild("LineNumbers", lineNumberSize);
    const auto line_count = std::count(textBuffer, textBuffer + bufferSize, '\n') + 1;

    //adds the line number
    for (auto i = 1; i <= line_count; ++i)
        ImGui::Text("%d", i);

    ImGui::EndChild();  //if child window is not used, then the input window will line up with the last number, and migrate down the window

    ImGui::SameLine();  //allows input window to be in same line and child window

    ImGui::InputTextMultiline("###inputField", textBuffer, bufferSize, inputTextSize, inputTextFlags);
}

void WindowClass::DrawInfo()
{
    if (currentFilename.size() == 0)
    {
        ImGui::Text("No File Opened!");
        return;
    }

    const auto file_extension = GetFileExtension(currentFilename);
    ImGui::Text("Opened file %s | File extension %s", currentFilename.data(), file_extension.data());

}

void WindowClass::SaveToFile(std::string_view filename)
{
    auto out = std::ofstream{filename.data()};

    if (out.is_open())
    {
        out << textBuffer;
        out.close();
    }
}

void WindowClass::LoadFromFile(std::string_view filename)
{
    auto in = std::ifstream{filename.data()};

    if (in.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << in.rdbuf();
        std::memcpy(textBuffer, buffer.str().data(), bufferSize);
        in.close();
    }
}

std::string WindowClass::GetFileExtension(std::string_view filename)
{
    //initialize with input filename
    const auto file_path = fs::path{filename};

    return file_path.extension().string();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Some Name");
}
