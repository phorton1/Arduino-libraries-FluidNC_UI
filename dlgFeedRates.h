//-----------------------------------------
// A dialog to modify realtime feed rates
//-----------------------------------------

#pragma once

#include "gApp.h"

#define PARAM_IDX_FEED_RATE     0
#define PARAM_IDX_RAPID_RATE    1
#define PARAM_IDX_SPINDLE_RATE  2
#define PARAM_IDX_LIVE_Z        3


class dlgFeedRates : public uiWindow
{
    public:

        dlgFeedRates();

        void setSelectedItem(int param_num);

    private:

        bool draw_needed;

        virtual void begin() override;
        virtual void update() override;
        virtual bool isModal() const override { return true; }
        virtual void onButton(const uiElement *ele, bool pressed) override;

        void initValues();
        void selectItem(int param_num);
        void modifyItem(int param_num, int button_num);

};


extern dlgFeedRates dlg_feed_rates;
