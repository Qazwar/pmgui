//
// Created by robin on 16.01.18.
//

#include "Listbox.h"

pmgui::Listbox::Listbox(const std::string &id, const std::string &label, bool multichoice, int itemSpace,
                      int heightInItems) : Control(id, label), m_multichoice {multichoice}, m_itemSpace {itemSpace}
{
    //m_multichoice = multichoice;
    //m_itemSpace = itemSpace;
    m_heightInItems = (heightInItems > -2) ? heightInItems : m_itemSpace;
    m_type = ControlType::Listbox;
}

/*!
 *
 * @return true if anything is selected
 */
bool pmgui::Listbox::process()
{
    if(Control::process())
    {
        //auto localItems = getLocalItems();
        if(!m_hasLabel)
            ImGui::PushItemWidth(-1);
        else
            pushWidth();

        ImGui::ListBoxHeader(m_imguiId.c_str(), m_itemSpace, m_heightInItems);
        bool anythingPressed = false;
        for (const auto & [pos, item] : m_items)
        {
            if(item->process())
            {
                ImGui::SetCursorPos({100.f, 100.f});
                if(!m_multichoice)
                    manageItem(item.get());

                anythingPressed = true;
            }
        }
        ImGui::ListBoxFooter();

        if(!m_hasLabel)
            ImGui::PopItemWidth();
        else
            popWidth();

        return anythingPressed;
    }

    return false;
}

void pmgui::Listbox::manageItem(Selectable *item)
{
    for (const auto & [pos, value] : m_items)
    {
        bool isSelected = (value.get() == item) ? true : false;
        value->setSelected(isSelected);
    }
}

int pmgui::Listbox::getCurrentItem() const
{
    return m_currentItem;
}

/*const char ** pmgui::Listbox::getLocalItems()
{
    const char * items[m_items.size()];

    for(int i = 0; i < m_items.size(); i++)
    {
        items[i] = m_items[i].c_str();
    }

    return items;
}*/

void pmgui::Listbox::addValue(const int &id, const std::string &value)
{
    std::string valueId = fmt::format("{0}_{1}", m_label, id);
    m_items[id] = std::make_unique<pmgui::Selectable>(valueId, value);
}

void pmgui::Listbox::addValues(const std::initializer_list<std::pair<size_t, std::string>> &values)
{
    for(const auto & item : values)
    {
        std::string id = fmt::format("{0}_{1}", m_label, item.first);
        m_items[item.first] = std::make_unique<pmgui::Selectable>(id, item.second);
    }
}

size_t pmgui::Listbox::getItemSpace() const
{
    return m_itemSpace;
}

void pmgui::Listbox::setItemSpace(size_t itemSpace)
{
    m_itemSpace = itemSpace;
}

bool pmgui::Listbox::hasLabel() const
{
    return m_hasLabel;
}

void pmgui::Listbox::setHasLabel(bool hasLabel)
{
    m_hasLabel = hasLabel;
}

