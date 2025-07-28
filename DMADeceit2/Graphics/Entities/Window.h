#pragma once
#include "Entity.h"
#include <string>
#include <memory>
#include <vector>

class Window : public Entity {
protected:
    bool Dragging;
    bool Resizing;
    float MinWidth;
    float MinHeight;
    float TitleBarHeight;
    std::wstring Title;

public:
    Window(float x, float y, float width, float height, float minWidth, float minHeight, const std::wstring& title, bool resizable = true);
    virtual ~Window() = default;

    void Update();
    void Draw();
    
    void SetTitle(const std::wstring& title);
    const std::wstring& GetTitle() const;
    
    bool IsDragging() const;
    bool IsResizing() const;
    
    float GetTitleBarHeight() const;
    void SetTitleBarHeight(float height);
    
    void AddChild(std::shared_ptr<Entity> child);
    void RemoveChild(std::shared_ptr<Entity> child);
    void ClearChildren();
    
    const std::vector<std::shared_ptr<Entity>>& GetChildren() const;
}; 