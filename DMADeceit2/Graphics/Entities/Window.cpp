#include "pch.h"
#include "Window.h"
#include "Drawing.h"

Window::Window(float x, float y, float width, float height, float minWidth, float minHeight, const std::wstring& title, bool resizable)
    : Dragging(false)
    , Resizing(false)
    , MinWidth(minWidth)
    , MinHeight(minHeight)
    , TitleBarHeight(30.0f)
    , Title(title)
{
    SetPosition(x, y);
    SetSize(width, height);
    SetVisible(true);
}

void Window::Update()
{
    if (!IsVisible())
        return;

    // Update window position when dragging
    if (Dragging && IsMouseDown(VK_LBUTTON))
    {
        POINT mousePos;
        GetCursorPos(&mousePos);
        SetPosition(static_cast<float>(mousePos.x - GetSize().x / 2),
                   static_cast<float>(mousePos.y - TitleBarHeight / 2));
    }
    else
    {
        Dragging = false;
    }

    // Check for title bar click to start dragging
    if (IsMouseDown(VK_LBUTTON) &&
        GetMousePosition().x >= GetPosition().x &&
        GetMousePosition().x <= GetPosition().x + GetSize().x &&
        GetMousePosition().y >= GetPosition().y &&
        GetMousePosition().y <= GetPosition().y + TitleBarHeight)
    {
        Dragging = true;
    }

    // Update children
    for (auto& child : GetChildren())
    {
        if (child)
            child->Update();
    }
}

void Window::Draw()
{
    if (!IsVisible())
        return;

    // Draw window background
    DrawFilledRect(GetPosition().x, GetPosition().y,
                  GetSize().x, GetSize().y,
                  MenuColours[LIT("Window Background")]);

    // Draw title bar
    DrawFilledRect(GetPosition().x, GetPosition().y,
                  GetSize().x, TitleBarHeight,
                  MenuColours[LIT("Title Bar")]);

    // Draw window title
    DrawText(GetPosition().x + 10, GetPosition().y + TitleBarHeight / 2 - 8,
             Title, LIT("Verdana"), 16,
             MenuColours[LIT("Title Text")], None);

    // Draw children
    for (auto& child : GetChildren())
    {
        if (child)
            child->Draw();
    }
}

void Window::SetTitle(const std::wstring& title)
{
    Title = title;
}

const std::wstring& Window::GetTitle() const
{
    return Title;
}

bool Window::IsDragging() const
{
    return Dragging;
}

bool Window::IsResizing() const
{
    return Resizing;
}

float Window::GetTitleBarHeight() const
{
    return TitleBarHeight;
}

void Window::SetTitleBarHeight(float height)
{
    TitleBarHeight = height;
}

void Window::AddChild(std::shared_ptr<Entity> child)
{
    if (child)
    {
        child->SetParent(this);
        GetChildren().push_back(child);
    }
}

void Window::RemoveChild(std::shared_ptr<Entity> child)
{
    auto& children = GetChildren();
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Window::ClearChildren()
{
    GetChildren().clear();
}

const std::vector<std::shared_ptr<Entity>>& Window::GetChildren() const
{
    static std::vector<std::shared_ptr<Entity>> children;
    return children;
} 