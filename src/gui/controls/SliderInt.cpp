//
// Created by robin on 20.01.18.
//

#include "SliderInt.h"

/*!
 *
 * @param id
 * @param label
 * @param minimum
 * @param maximum
 * @param sliderType
 * @param size Only used for vertical sliders
 */
pmgui::SliderInt::SliderInt(const std::string &id, const std::string &label, int minimum, int maximum,
                          SliderIntType sliderType, const sf::Vector2<int> &size)
        : Control(id, label), m_minimum {minimum}, m_maximum {maximum}, m_size {size}, m_sliderType {sliderType}
{
    //m_minimum = minimum;
    //m_maximum = maximum;
    //m_size = size;
    m_type = ControlType::DragInt;
    //m_sliderType = sliderType;
    initialize();
}

bool pmgui::SliderInt::process()
{
    bool isChanged = false;
    if(Control::process())
    {
        pushWidth();
        if(processSlider())
        {
            isChanged = true;
        }
        popWidth();
    }

    return isChanged;
}

void pmgui::SliderInt::initialize()
{
    switch (m_sliderType)
    {
        case SliderIntType::H_One:
        case SliderIntType::Vertical:
            m_values = std::make_unique<int[]>(1);
            m_values[0] = m_minimum;
            m_arraySize = 1;
            break;

        case SliderIntType::H_Two:
            m_values = std::make_unique<int[]>(2);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_arraySize = 2;
            break;

        case SliderIntType::H_Three:
            m_values = std::make_unique<int[]>(3);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_values[2] = m_minimum;
            m_arraySize = 3;
            break;

        case SliderIntType::H_Four:
            m_values = std::make_unique<int[]>(4);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_values[2] = m_minimum;
            m_values[3] = m_minimum;
            m_arraySize = 4;
            break;
    }
}

bool pmgui::SliderInt::processSlider()
{
    std::string format = (m_hideNumber) ? "" : "%.0f";
    bool result = false;
    switch (m_sliderType)
    {
        case SliderIntType::H_One:
            result = ImGui::SliderInt(m_imguiId.c_str(), m_values.get(), m_minimum, m_maximum, format.c_str());
            break;

        case SliderIntType::H_Two:
            result = ImGui::SliderInt2(m_imguiId.c_str(), m_values.get(), m_minimum, m_maximum, format.c_str());
            break;

        case SliderIntType::H_Three:
            result = ImGui::SliderInt3(m_imguiId.c_str(), m_values.get(), m_minimum, m_maximum, format.c_str());
            break;

        case SliderIntType::H_Four:
            result = ImGui::SliderInt4(m_imguiId.c_str(), m_values.get(), m_minimum, m_maximum, format.c_str());
            break;

        case SliderIntType::Vertical:
            result = ImGui::VSliderInt(m_imguiId.c_str(), m_size, m_values.get(), m_minimum, m_maximum, format.c_str());
            break;
    }

    return result;
}

void pmgui::SliderInt::setValues(const std::initializer_list<int> &values)
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

const int * pmgui::SliderInt::getValues() const
{
    return m_values.get();
}

bool pmgui::SliderInt::getHideNumber() const
{
    return m_hideNumber;
}

void pmgui::SliderInt::setHideNumber(bool hideNumber)
{
    m_hideNumber = hideNumber;
}

