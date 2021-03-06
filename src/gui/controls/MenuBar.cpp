//
// Created by robin on 24.01.18.
//

#include "MenuBar.h"

pmgui::MenuBar::MenuBar(const std::string &id, bool isMainMenu) : Control(id, ""), m_isMainMenu {isMainMenu}
{
    m_type = ControlType::MenuBar;
    //m_isMainMenu = isMainMenu;
}

bool pmgui::MenuBar::process()
{
    m_isPressed = false;
    if(Control::process())
    {
        if(m_isMainMenu)
        {
            if (ImGui::BeginMainMenuBar())
            {
                m_menuSize = ImGui::GetWindowSize();
                m_isPressed = true;
                processMenuItems();
                ImGui::EndMainMenuBar();
            }
        }
        else
        {
            if (ImGui::BeginMenuBar())
            {
                m_menuSize = ImGui::GetWindowSize();
                m_isPressed = true;
                processMenuItems();
                ImGui::EndMenuBar();
            }
        }
    }

    return m_isPressed;
}

bool pmgui::MenuBar::processMenuItems()
{
    bool anyItemActive = false;
    for(const auto &item : m_menuItems)
    {
        if(item->process())
        {
            anyItemActive = true;
        }
    }

    for(const auto &item : m_menuRefItems)
    {
        if(item->process())
        {
            anyItemActive = true;
        }
    }

    /*for(auto const & [pos, obj] : m_menuItems)
    {
        if(obj->process())
        {
            anyItemActive = true;
        }
    }*/

    return anyItemActive;
}

void pmgui::MenuBar::add(std::unique_ptr<pmgui::Control> menuItem)
{
    //m_menuItems[menuItem->getId()] = std::move(menuItem);
    m_menuItems.push_back(std::move(menuItem));
}

void pmgui::MenuBar::addRef(pmgui::Control *menuItem)
{
    m_menuRefItems.push_back(menuItem);
}

/*!
 * process() must have been ran at least once to be able to determine the menu size.
 * @return
 */
const sf::Vector2<int> &pmgui::MenuBar::getMenuSize() const
{
    return m_menuSize;
}
