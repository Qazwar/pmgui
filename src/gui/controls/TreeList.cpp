//
// Created by robin on 14.07.18.
//

#include "TreeList.h"

pmgui::TreeList::TreeList() : Control("undefined", "undefined")
{

}

pmgui::TreeList::TreeList(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::TreeList;
}

bool pmgui::TreeList::process()
{
    bool isAnyItemActivated = false;
    if(Control::process())
    {
        if (m_eraseItems.size() > 0) {
            size_t erasedCount = 0;
            for (auto del : m_eraseItems) {
                erasedCount += m_items.erase(del);
            }
            m_eraseItems.clear();
            //SystemLog::get()->addDebug(fmt::format("Erased {0} from {1} ({2} items)", erasedCount, getId(), m_items.size()));
        }
        pushWidth();
        m_isOpen = (m_hasParentNode) ? ImGui::TreeNode(m_label.c_str()) : true;
        popWidth();
        if(m_isOpen)
        {
            processMouseEvents();
            int i = 0;
            //Child nodes
            for(auto & [pos, item] : m_items)
            {
                if(item.process())
                    isAnyItemActivated = true;
                ++i;
            }
            if(m_hasParentNode)
                ImGui::TreePop();
        }
        else
        {
            ImGui::PushID(m_label.c_str());
            {
                processMouseEvents();
            }
            ImGui::PopID();
        }
        if(ImGui::IsMouseClicked(0))
        {
            m_rightClickContextActivated = false;
        }
    }

    return isAnyItemActivated;
}

/*!
 * Adds a selectable item to the TreeList.
 * ID is automatically generated as a lower-case string of the label
 * @param label The text shown in the TreeList
 * @return The generated Selectable as a pointer
 */
pmgui::Selectable* pmgui::TreeList::add(const std::string &label)
{
    std::string id = label;
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);

    //m_items[id] = {id, label};
    //Must be done this way because pmgui::Selectable has no empty constructor
    m_items.insert(std::map< std::string, pmgui::Selectable>::value_type (id, pmgui::Selectable(id, label)));
    return &m_items.find(id)->second;//&m_items[id];
}

pmgui::Selectable *pmgui::TreeList::add(const std::string &id, const std::string &label)
{
    m_items.insert(std::map< std::string, pmgui::Selectable>::value_type (id, pmgui::Selectable(id, label)));
    return &m_items.find(id)->second;//&m_items[id];
}

/*!
 * Adds a selectable item to the TreeList with an image in the front generated by image data.
 * @param id The ID used to track the object inside a std::map
 * @param label The label of the object shown to the user
 * @param image The image object that is placed in the front of the text of the selectable
 * @return
 */
pmgui::Selectable *pmgui::TreeList::add(const std::string &id, const std::string &label, const pmgui::Image &image)
{
    m_items.insert(std::map< std::string, pmgui::Selectable>::value_type (id, pmgui::Selectable(id, label, image)));
    return &m_items.find(id)->second;
}

/*!
 * Adds a selectable item to the TreeList with an image in the front generated by image data.
 * ID is automatically generated as a lower-case string of the label
 * @param label The text shown in the TreeList
 * @return The generated Selectable as a pointer
 */
pmgui::Selectable* pmgui::TreeList::add(const std::string &label, const void *imageData, size_t imageDataSize)
{
    std::string id = label;
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);

    m_items.insert(std::map< std::string, pmgui::Selectable>::value_type (id, pmgui::Selectable(id, label)));
    auto item = &m_items.find(id)->second;
    item->setImage(imageData, imageDataSize, true);
    return item;
}

/*!
 * Quicker image loading!
 *
 * Adds a selectable item to the TreeList with an image in the front generated by image data.
 * ID is automatically generated as a lower-case string of the label
 * @param label The text shown in the TreeList
 * @return The generated Selectable as a pointer
 */
pmgui::Selectable *pmgui::TreeList::add(const std::string &label, pmgui::Image *imageRef)
{
    std::string id = label;
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);

    m_items.insert(std::map< std::string, pmgui::Selectable>::value_type (id, pmgui::Selectable(id, label)));
    auto item = &m_items.find(id)->second;
    item->setImageRef(imageRef);
    return item;
}

pmgui::Selectable *pmgui::TreeList::getItem(const std::string &id)
{
    return &m_items.find(id)->second;
}

pmgui::Selectable *pmgui::TreeList::getItem(int index)
{
    if(index > -1 && index < (m_items.size() - 1))
        return getItems()[index];

    return nullptr;
}

/*!
 * Removes an item by its ID
 * @param id The items of the id to remove
 */
bool pmgui::TreeList::remove(const std::string &id)
{
    for(std::map<std::string, pmgui::Selectable>::iterator it = m_items.begin(); it != m_items.end();)//;it++)
    {
        if(it->first == id)
        {
            m_eraseItems.push_back(it->first);
            //SystemLog::get()->addInfo(fmt::format("{0} scheduled remove from {1}!", id, getId()));
            return true;
        }
        else
            ++it;
    }

    return false;
}

void pmgui::TreeList::clear()
{
    m_items.clear();
}

void pmgui::TreeList::registerOnChosenCallback(const pmgui::TreeList::func_treelist_parent &cb)
{
    m_callbackOnParentClicked.emplace_back(cb);
}

void pmgui::TreeList::registerOnRightClickCallback(const pmgui::TreeList::func_treelist_parent_rightclick &cb)
{
    m_callbackOnParentRightClicked.emplace_back(cb);
}

void pmgui::TreeList::registerOnDoubleClickCallback(const pmgui::TreeList::func_treelist_parent &cb)
{
    m_callbackOnParentDoubleClicked.emplace_back(cb);
}

void pmgui::TreeList::registerOnChosenContextItemCallback(const pmgui::TreeList::func_treelist_parent_chosencontextitem &cb)
{
    m_callbackOnChosenParentContextItem.emplace_back(cb);
}

/*!
 * Creates a context of menuitems used for when the right click context has been activated.
 * This is a fresh context, and will overwrite any potential existing data.
 *
 * @param items Each item is defined as {"id", "label"}
 */
void pmgui::TreeList::createRightClickContextItemsAdvanced(
        const std::initializer_list<std::pair<std::string, std::string>> &items)
{
    m_rightClickContextItems.clear();
    for(auto &item : items)
    {
        m_rightClickContextItems.emplace_back(item.first, item.second);
    }
}

/*!
 * An easier way to create right click context, where ID's are automatically set to be the same as label, but in lower-case.
 * @param items
 */
void pmgui::TreeList::createRightClickContextItems(const std::initializer_list<std::string> &items)
{
    m_rightClickContextItems.clear();
    for(auto &item : items)
    {
        std::string id = item;
        std::transform(id.begin(), id.end(), id.begin(), ::tolower);
        m_rightClickContextItems.emplace_back(id, item);
    }
}

void pmgui::TreeList::processMouseEvents()
{
    if(ImGui::IsItemClicked(1))
    {
        for (auto &callback : m_callbackOnParentRightClicked)
        {
            if (callback(this))
            {
                m_rightClickContextActivated = true;
            }
        }
    }

    if(ImGui::IsItemClicked())
    {
        for (auto &callback : m_callbackOnParentClicked)
            callback(this);
    }

    if(m_rightClickContextActivated)
    {
        if(ImGui::BeginPopupContextWindow())
        {
            for(auto &item : m_rightClickContextItems)
            {
                if(item.process())
                {
                    for (auto &callback : m_callbackOnChosenParentContextItem)
                    {
                        callback(this, &item);
                    }
                }
            }
            ImGui::EndPopup();
        }
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
    {
        for (auto &callback : m_callbackOnParentDoubleClicked)
        {
            callback(this);
        }
    }
}

std::vector<pmgui::Selectable *> pmgui::TreeList::getItems()
{
    std::vector<Selectable *> items;
    for(auto & [pos, item] : m_items)
    {
        items.push_back(&item);
    }
    return items;
}

bool pmgui::TreeList::hasParentNode() const
{
    return m_hasParentNode;
}

/*!
 * Removes the parent node is false. Perfect for a list
 * @param hasParentNode
 */
void pmgui::TreeList::setHasParentNode(bool hasParentNode)
{
    m_hasParentNode = hasParentNode;
}

bool pmgui::TreeList::isOpen() const
{
    return m_isOpen;
}

void pmgui::TreeList::swap(const std::string &id1, const std::string &id2)
{
    std::swap(m_items.find(id1)->second, m_items.find(id2)->second);
}

std::string_view pmgui::TreeList::getItemId(int index)
{
    int i = 0;
    for(std::map<std::string, pmgui::Selectable>::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        if(i == index)
            return it->first;
        ++i;
    }

    return nullptr;
}