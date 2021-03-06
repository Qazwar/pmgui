//
// Created by robin on 01.10.18.
//

#ifndef PMGUI_INPUTFLOAT_H
#define PMGUI_INPUTFLOAT_H

#include "Control.h"

namespace pmgui
{
    class InputFloat : public Control
    {
        public:
            InputFloat(const std::string &id, const std::string &label, float minimum, float maximum, float step, float stepFast);
            InputFloat(const std::string &id, const std::string &label);

            void initialize(float minimum, float maximum, float step, float stepFast);
            bool process() override;

            void setValue(float value);

            float getValue() const;

        protected:
            bool performValueValidation();

            float m_value;
            float m_minimum;
            float m_maximum;
            float m_step;
            float m_stepFast;
    };
}

#endif //PMGUI_INPUTFLOAT_H
