//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "winFiles.h"

#ifdef WITH_APPLICATION

    #include "dlgMsg.h"
    #include "dlgConfirm.h"
    #include <SD.h>
    #include "protectedFS.h"

    #define V_SDCARD_CS   GPIO_NUM_21

    #define NUM_LINES 5

    winFiles files_win;

    static char path[80] = "/";
    static int  top_num = 0;
    static bool draw_needed;
    static int  last_top_num = 0;
    static char last_path[80];
    static char filenames[NUM_LINES][80];
    static char selected_file[128] = "";
    static bool file_error = false;
    static bool running_file = false;


    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    #define NUM_BUTTONS  4

    #define IDX_BACK_BUTTON     0
    #define IDX_UP_BUTTON       1
    #define IDX_DOWN_BUTTON     2
    #define IDX_ROOT_BUTTON     3

    #define ID_BACK_BUTTON   (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
    #define ID_UP_BUTTON     (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
    #define ID_DOWN_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
    #define ID_ROOT_BUTTON   (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )

    #define ID_FILE1     (0x0011 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
    #define ID_FILE2     (0x0012 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
    #define ID_FILE3     (0x0013 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
    #define ID_FILE4     (0x0014 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
    #define ID_FILE5     (0x0015 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)

    uiMutable buttons[NUM_BUTTONS] =
    {
        { "..", COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
        { "^",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
        { "v",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
        { "/",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    };
    uiMutable files[NUM_LINES] =
    {
        { filenames[0],  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_SMALL },
        { filenames[1],  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_SMALL },
        { filenames[2],  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_SMALL },
        { filenames[3],  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_SMALL },
        { filenames[4],  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_SMALL },
    };


    static const uiElement file_elements[] =
    {
        { ID_BACK_BUTTON, 265,  37, 50, 42, V(&buttons[0]) },
        { ID_UP_BUTTON  , 265,  79, 50, 42, V(&buttons[1]) },
        { ID_DOWN_BUTTON, 265, 121, 50, 42, V(&buttons[2]) },
        { ID_ROOT_BUTTON, 265, 163, 50, 42, V(&buttons[3]) },

        { ID_FILE1,     0,   35, 250, 34, V(&files[0]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
        { ID_FILE2,     0,   69, 250, 34, V(&files[1]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
        { ID_FILE3,     0,  103, 250, 34, V(&files[2]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
        { ID_FILE4,     0,  137, 250, 34, V(&files[3]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
        { ID_FILE5,     0,  171, 250, 34, V(&files[4]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    };





    //----------------------
    // implementation
    //----------------------

    winFiles::winFiles() :
        uiWindow(file_elements,sizeof(file_elements)/sizeof(uiElement))
    {}


    const char *winFiles::getFileQuestion(int i)
    {
        strcpy(selected_file,"run ");
        strcat(selected_file,filenames[i]);
        char *p = &selected_file[4];
        while (*p && *p != ' ') p++;
        *p = 0;
        strcat(selected_file,"?");
        return selected_file;
    }


    const char *winFiles::getFileToRun(int i)
    {
        strcpy(selected_file,path);
        if (strcmp(path,"/"))
            strcat(selected_file,"/");
        strcat(selected_file,filenames[i]);
        char *p = selected_file;
        while (*p && *p != ' ') p++;
        *p = 0;
        return selected_file;
    }


    static void setPath(const char *ipath, bool absolute)
    {
        if (absolute)
        {
            if (ipath != path)
                strcpy(path,ipath);
        }
        else
        {
            if (!strcmp(path,"/"))
                path[0] = 0;
            strcat(path,ipath);
        }
        if (!path[0])
            strcpy(path,"/");
        top_num = 0;
        last_top_num = 0;
        last_path[0] = 0;
        debug_serial("init_path(%s)",path);
    }


    void winFiles::begin()
    {
        tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
        if (file_error)
        {
            file_error = false;
            setPath("/",true);
        }
        draw_needed = true;
    }


    static void fileError(const char *text)
    {
        drawText(
            text,
            JUST_CENTER,
            FONT_BIG,
            0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,
            COLOR_RED,
            COLOR_BLACK);
        debug_serial("FILE ERROR: %s",text);
        file_error = true;
    }


    bool isGcodeFile(const char *new_path)
    {
        const char *p = new_path;
        while (*p)
        {
            if (*p++ == '.')
                new_path = p;
        }
        return
            !strcmp(new_path,"g") ||
            !strcmp(new_path,"gc") ||
            !strcmp(new_path,"ngc");
            !strcmp(new_path,"G") ||
            !strcmp(new_path,"GC") ||
            !strcmp(new_path,"NGC");
    }


    void winFiles::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        if (!pressed)
        {
            switch (ele->id_type)
            {
                case ID_BACK_BUTTON :
                    {
                        char *p = path;
                        char *last = &path[0];
                        while (*p)
                        {
                            if (*p == '/')
                            {
                                last = p;
                            }
                            p++;
                        }
                        *last = 0;
                        setPath(path,true);
                    }
                    break;
                case ID_UP_BUTTON :
                    top_num--;
                    break;
                case ID_DOWN_BUTTON :
                    top_num++;
                    break;
                case ID_ROOT_BUTTON :
                    setPath("/",true);
                    break;
                case ID_FILE1 :
                case ID_FILE2 :
                case ID_FILE3 :
                case ID_FILE4 :
                case ID_FILE5 :
                {
                    uint16_t file_num = ele->id_type - ID_FILE1;
                    const char *new_path = filenames[file_num];
                    debug_serial("ID_FILE(%d) %s",file_num,new_path);

                    if (new_path[0] == '/')
                    {
                        setPath(new_path,false);
                    }
                    else if (isGcodeFile(getFileToRun(file_num)))
                    {
                        // only allow running of .g, .gc, or .ngc files
                        last_path[0] = 0;
                        confirm_dlg.setConfirm(CONFIRM_COMMAND_RUN_FILE + file_num);
                        the_app.openWindow(&confirm_dlg);
                    }
                    else
                    {
                        warningMsg("File is not gcode!");
                    }
                }
            }
        }
    }



    void winFiles::update()
    {
        if (top_num != last_top_num ||
            strcmp(last_path,path))
        {
            draw_needed = true;
            last_top_num = top_num;
            strcpy(last_path,path);

            for (int i=0; i<NUM_LINES; i++)
            {
                filenames[i][0] = 0;
                files[i].bg = COLOR_BUTTON_HIDDEN;
            }
            for (int i=0; i<NUM_BUTTONS; i++)
                buttons[i].bg = COLOR_BUTTON_HIDDEN;

            the_app.setTitle(path);

            #ifdef WITH_GRBL
                if (g_status.getSDState(true) == grbl_SDState_t::Idle)
            #else
                static bool sd_started = false;
                if (!sd_started)
                    sd_started = SD.begin(V_SDCARD_CS);

                if (sd_started)
            #endif
            {
                protectedFS fs(SD);
                int num_files = 0;
                int num_lines = 0;

                proFile root = fs.open(path);
                if (root && root.isDirectory())
                {
                    proFile file = root.openNextFile();
                    while (file)
                    {
                        debug_serial("file(%s)",file.leafName());

                        if (num_files >= top_num &&
                            num_files < top_num + NUM_LINES)
                        {
                            files[num_lines].bg = COLOR_DARKBLUE;
                            if (file.isDirectory())
                            {
                                sprintf(filenames[num_lines++],"/%s",file.leafName());
                            }
                            else
                            {
                                sprintf(filenames[num_lines++],"%-20s %d",file.leafName(),file.size());
                            }
                        }
                        num_files++;
                        file = root.openNextFile();
                    }
                }
                else
                {
                    fileError("Could not opendir");
                }

                if (root)
                    root.close();

                #ifdef WITH_GRBL
                    SD.end();
                #endif

                debug_serial("dir(%s) top(%d) lines(%d) files(%d)", path, top_num, num_lines, num_files);

                if (strcmp(path,"/"))
                    buttons[IDX_BACK_BUTTON].bg = COLOR_BLUE;

                if (top_num)
                    buttons[IDX_UP_BUTTON].bg = COLOR_BLUE;

                if (top_num + NUM_LINES < num_files)
                    buttons[IDX_DOWN_BUTTON].bg = COLOR_BLUE;

                if (strcmp(path,"/"))
                    buttons[IDX_ROOT_BUTTON].bg = COLOR_BLUE;
            }
            else
                fileError("Could not get SDCARD");
        }

        if (draw_needed && !file_error)
            drawTypedElements();
        draw_needed = false;
    }


#endif  // WITH_APPLICATION
