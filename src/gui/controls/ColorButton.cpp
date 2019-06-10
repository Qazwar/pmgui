//
// Created by robin on 28.01.18.
//

#include "ColorButton.h"

pmgui::ColorButton::ColorButton(const std::string &id, const std::string &label, const sf::Color &color,
                              const sf::Vector2<int> &size) : Control(id, label), m_color {color}, m_size {size}
{
    //m_color = color;
    m_type = ControlType::ColorButton;
    //m_size = size;
}

bool pmgui::ColorButton::process()
{
    m_isClicked = false;
    if(Control::process())
    {
        pushWidth();
        if(ImGui::ColorButton(m_label.c_str(), m_color, 0, m_size))
        {
            m_isClicked = true;
        }
        popWidth();
    }
    return m_isClicked;
}

const sf::Color &pmgui::ColorButton::getColor() const
{
    return m_color;
}

void pmgui::ColorButton::setColor(const sf::Color &color)
{
    m_color = color;
}
