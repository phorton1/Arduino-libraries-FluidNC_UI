//--------------------------------------
// The directory listing window
//--------------------------------------

#pragma once

#include "gApp.h"

#define MAX_SCROLLABLE_LINES           5


class uiScrollable : public uiWindow
    // Handles generic scrolling of a dual element list window
{
    public:

        uiScrollable();

    protected:

        bool m_draw_needed;
        int m_top_num;
            // set to zero when changing lists
        int m_last_top_num;
            // set to -1 to when changing lists
        int m_num_lines;
            // set to the number of available lines as you build it
        uint32_t m_buffer_version;
            // set to zero to force a call to updateBuffer()

        virtual void begin() override;
        virtual void update() override;

        virtual uint32_t updateBuffer() = 0;
            // client provided updateBuffer() method will be called first
            // client fills the buffer with whatever it needs and zets
            // m_num_lines to inform base class how many lines there are
        void setErrMsg(const char *err_msg) { m_err_msg = err_msg; }
            // or, if the number is zero, yo probably want to set
            // a big overriding error message on the screen

        virtual void getLine(int line_num, const char **left_side, const char **right_side) = 0;
            // The base class will then call setLine() for top_num .. top_num+4 lines
            // with the absolute line number in your range to get pointers to the left
            // and right side text to display
        virtual const char *getTitle() const = 0;
            // the base class will call this as needed to call the_app.setTitle()

            // there are no elements

        virtual bool canGoBack() = 0;
        virtual bool canGoHome() = 0;
        virtual void onGoBack() = 0;
        virtual void onGoHome() = 0;
        virtual void onLineClicked(uiDualElement *dual, int top_num, int rel_line_num) = 0;
            // you are given top_num and rel_line_num, so your
            // elsement is top_num + rel_line_num


    private:

        virtual void onButton(const uiElement *ele, bool pressed) override;



        const char *m_err_msg;
        const char *m_last_err_msg;
};
