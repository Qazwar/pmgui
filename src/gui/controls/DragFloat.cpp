//
// Created by robin on 28.01.18.
//

#include "DragFloat.h"

pmgui::DragFloat::DragFloat(const std::string &id, const std::string &label, float minimum, float maximum,
                          int decimals, float speed, pmgui::DragFloat::DragFloatSize dragSize)
        : Control(id, label), m_minimum {minimum}, m_maximum {maximum}, m_speed {speed}, m_dragSize {dragSize},
          m_decimals {decimals}
{
    //m_minimum = minimum;
    //m_maximum = maximum;
    //m_speed = speed;
    m_type = ControlType::DragFloat;
    //m_dragSize = dragSize;
    //m_decimals = decimals;
    m_format = fmt::format("%.{0}f", decimals);
    initialize();
}

bool pmgui::DragFloat::process()
{
    bool isChanged = false;
    if(Control::process())
    {
        //pushWidth();
        if(processDrag())
        {
            performValueValidation();
            isChanged = true;
        }
        //popWidth();
    }

    return isChanged;
}

void pmgui::DragFloat::setValues(const std::initializer_list<float> &values)
{
    size_t size = (m_arraySize > values.size()) ? values.size() : m_arraySize;

    size_t i = 0;
    for(const auto & v : values)
    {
        if(i < size)
            m_values[i] = v;

        i++;
    }
}

const float *pmgui::DragFloat::getValues() const
{
    return m_values.get();
}

bool pmgui::DragFloat::processDrag()
{
    std::string format = (m_hideNumber) ? "" : m_format;
    //Not working if called through popWidth() - which does the same...
    if(m_width >= 1.f)
        ImGui::PushItemWidth(m_width);
    bool result = false;
    switch (m_dragSize)
    {
        case DragFloatSize::One:
            result = ImGui::DragFloat(m_imguiId.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
            break;

        case DragFloatSize::Two:
            result = ImGui::DragFloat2(m_imguiId.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
            break;

        case DragFloatSize::Three:
            result = ImGui::DragFloat3(m_imguiId.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
            break;

        case DragFloatSize::Four:
            result = ImGui::DragFloat4(m_imguiId.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
            break;

    }

    if(m_width >= 1.f)
        ImGui::PopItemWidth();

    return result;
}

bool pmgui::DragFloat::performValueValidation()
{
    bool valuesHasBeenCorrected = false;

    for(int i = 0; i < m_arraySize; i++)
    {
        if(m_values[i] < m_minimum)
        {
            m_values[i] = m_minimum;
            valuesHasBeenCorrected = true;
        }
        else if(m_values[i] > m_maximum)
        {
            m_values[i] = m_maximum;
            valuesHasBeenCorrected = true;
        }
    }

    return valuesHasBeenCorrected;
}

void pmgui::DragFloat::initialize()
{
    switch (m_dragSize)
    {
        case DragFloatSize::One:
            m_values = std::make_unique<float[]>(1);
            m_values[0] = m_minimum;
            m_arraySize = 1;
            break;

        case DragFloatSize::Two:
            m_values = std::make_unique<float[]>(2);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_arraySize = 2;
            break;

        case DragFloatSize::Three:
            m_values = std::make_unique<float[]>(3);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_values[2] = m_minimum;
            m_arraySize = 3;
            break;

        case DragFloatSize::Four:
            m_values = std::make_unique<float[]>(4);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_values[2] = m_minimum;
            m_values[3] = m_minimum;
            m_arraySize = 4;
            break;
    }
}

bool pmgui::DragFloat::getHideNumber() const
{
    return m_hideNumber;
}

void pmgui::DragFloat::setHideNumber(bool hideNumber)
{
    m_hideNumber = hideNumber;
}

float pmgui::DragFloat::getSpeed() const
{
    return m_speed;
}

void pmgui::DragFloat::setSpeed(float speed)
{
    m_speed = speed;
}

//float pmgui::DragFloat::getWidth() const
//{
//    return m_width;
//}
//
//void pmgui::DragFloat::setWidth(float width)
//{
//    m_width = width;
//}
