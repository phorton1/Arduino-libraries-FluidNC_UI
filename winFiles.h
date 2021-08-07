//--------------------------------------
// The directory listing window
//--------------------------------------

#pragma once

#include "uiScrollable.h"

#define MAX_FILE_PATH 80


class winFiles : public uiScrollable
{
    public:

        winFiles() :  uiScrollable()
        {
            strcpy(m_path,"/");
        }

        const char *getPath() const
        {
            return m_path;
        }

        const char *getFileToRun(int i) const;

    private:

        virtual void begin() override;
        // virtual void update() override;
        // virtual void onButton(const uiElement *ele, bool pressed) override;
        virtual const char *getMenuLabel() const override  { return "FILES"; }
        virtual const char *getTitle() const override { return m_path; }

        virtual uint32_t updateBuffer() override;
        virtual void getLine(int line_num, const char **left_side, const char **right_side) override;

        virtual bool canGoBack() override;
        virtual bool canGoHome() override;
        virtual void onGoBack()  override;
        virtual void onGoHome()  override;
        virtual void onLineClicked(uiDualElement *dual, int top_num, int rel_line_num) override;

        bool m_file_error;
        char m_path[MAX_FILE_PATH+1];
        char m_last_path[MAX_FILE_PATH+1];

        void fileError(const char *text);
        void setPath(const char *ipath, bool absolute);
        bool addFilename(const char *filename, int size);
        void sortFilenames();

};

extern winFiles files_win;
