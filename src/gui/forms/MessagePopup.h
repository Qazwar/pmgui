//
// Created by robin on 27.04.18.
//

#ifndef PMGUI_MESSAGEPOPUP_H
#define PMGUI_MESSAGEPOPUP_H

#include "Popup.h"
#include "../controls/Button.h"
#include "../controls/Group.h"
#include "../controls/Child.h"
#include "../controls/Label.h"

namespace pmgui
{
    enum class MessagePopupType : unsigned
    {
        Ok = 0,
        OkCancel = 1,
        YesNo = 2,
        YesNoCancel = 3,
        Custom = 4
    };

    enum class MessagePopupResponse : unsigned
    {
        None = 0,
        Ok = 1,
        Yes = 2,
        No = 3,
        Cancel = 4,
        Custom = 5
    };

    class MessagePopup : public Popup
    {
        typedef std::function<void(pmgui::Button *, pmgui::MessagePopupResponse)> func_msg_response; //OnPressed(), for instance
        public:
            MessagePopup(std::string id, std::string title, const MessagePopupType &popupType);//, std::string imguiId);

            MessagePopup(const sf::Vector2<int> &position, const sf::Vector2<int> &size, std::string id,
                         std::string title, const MessagePopupType &popupType);//, std::string imguiId);

            bool draw() override;

            void refresh();

            void changePopupType(const MessagePopupType &popupType);

            void registerOnMsgResponseCallback(const func_msg_response &cb);
            void clearCallbacks();

            void setMessage(const std::string &msg);

            void setCustomButtonVisibility(bool btn1Visible, bool btn2Visible, bool btn3Visible);
            void setCustomButtonTexts(const std::string &btn1Text, const std::string &btn2Text, const std::string &btn3Text);
            void setCustomButtonCommands(const std::string &btn1Cmd, const std::string &btn2Cmd, const std::string &btn3Cmd);

            const std::string &getMessage() const;
            MessagePopupResponse getLastResponse() const;

        protected:
            void initialize();
            void initializeButtons();
            void addReferences();

            void onButtonPress(Button *btn, const std::string &command);
            size_t getNumberOfButtons();

            Group m_messageGroup {"msg_group", "Message"};
            Child m_messageChild {"msg_child", "Message child"};
            Group m_btnGroup {"btn_group", "Button"};
            Child m_btnChildFiller {"btn_child_filler", "Filler"};
            Child m_btnChildMid {"btn_child_main", "Main"};

            Label m_msgText {"msg_box"};
            Button m_btn1 {"btn1"};
            Button m_btn2 {"btn2"};
            Button m_btn3 {"btn3"};

            std::vector<func_msg_response> m_callbackOnMsgResponse;

            std::string m_message;
            MessagePopupType m_messagePopupType = MessagePopupType::Ok;
            MessagePopupResponse  m_lastResponse = MessagePopupResponse::None;

    };
}

#endif //PMGUI_MESSAGEPOPUP_H
