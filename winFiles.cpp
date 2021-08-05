//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "winFiles.h"
#include "dlgMsg.h"
#include "dlgConfirm.h"
#include <SD.h>
#include <string.h>
#include "protectedFS.h"

#define DEBUG_FILES  0

#define NUM_LINES         5
#define MAX_FILENAMES     30
#define MAX_FILEBUFFER    2048



winFiles files_win;

static char path[MAX_FILENAME+1] = "/";
static int  top_num = 0;
static bool draw_needed;
static int  last_top_num = 0;
static char last_path[80];
static bool file_error = false;

static int   g_num_files = 0;
static int   g_filebuffer_ptr = 0;
static char  g_filebuffer[MAX_FILEBUFFER+1];
static int   g_filesize[MAX_FILENAMES];
static char *g_filenames[MAX_FILENAMES];


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

#define ID_FILE1     (0x0011 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_FILE2     (0x0012 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_FILE3     (0x0013 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_FILE4     (0x0014 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_FILE5     (0x0015 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)

uiMutable buttons[NUM_BUTTONS] =
{
    { "..", COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    { "^",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    { "v",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    { "/",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
};

uiDualElement files[NUM_LINES] =
{
    { ELEMENT_TYPE_INT, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_INT, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_INT, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_INT, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_INT, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
};


static const uiElement file_elements[] =
{
    { ID_BACK_BUTTON, 270,  37, 48, 42, V(&buttons[0]) },
    { ID_UP_BUTTON  , 270,  79, 48, 42, V(&buttons[1]) },
    { ID_DOWN_BUTTON, 270, 121, 48, 42, V(&buttons[2]) },
    { ID_ROOT_BUTTON, 270, 163, 48, 42, V(&buttons[3]) },

    { ID_FILE1, 0,   35, 265, 34, V(&files[0]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_FILE2, 0,   69, 265, 34, V(&files[1]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_FILE3, 0,  103, 265, 34, V(&files[2]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_FILE4, 0,  137, 265, 34, V(&files[3]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_FILE5, 0,  171, 265, 34, V(&files[4]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
};





//----------------------
// implementation
//----------------------

winFiles::winFiles() :
    uiWindow(file_elements,sizeof(file_elements)/sizeof(uiElement))
{}

const char *winFiles::getPath()
{
    return path;
}

const char *winFiles::getFileToRun(int i)
{
    return files[i].label;
}


static void setPath(const char *ipath, bool absolute)
{
    if (absolute)       // ipath includes the slash
    {
        strcpy(path,ipath);
    }
    else                // ipath gets added to current path
    {
        if (strlen(path) + strlen(ipath) + 1 >= MAX_FILENAME)
        {
            errorMsg("path too long!!");
            return;
        }
        if (strcmp(path,"/"))
            strcat(path,"/");
        strcat(path,ipath);
    }
    if (!path[0])
        strcpy(path,"/");
    top_num = 0;
    last_top_num = 0;
    last_path[0] = 0;

    #if DEBUG_FILES
        g_debug("init_path(%s)",path);
    #endif
}


void winFiles::begin()
{
    tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
    if (file_error)
    {
        file_error = false;
        setPath("/",true);
    }
    else
    {
        last_path[0] = 0;
            // force refresh on FILES-FILES button
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
    g_debug("FILE ERROR: %s",text);
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
                uiDualElement *dual = &files[file_num];
                const char *fn = dual->label;
                bool is_dir = dual->type & ELEMENT_TYPE_FLAG_DIR;


                #if DEBUG_FILES
                    g_debug("ID_FILE(%d) is_dir(%d) %s",file_num,is_dir,fn);
                #endif

                if (is_dir)
                {
                    setPath(fn,false);
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



static void addFilename(const char *filename, int size)
{
    if (g_num_files >= MAX_FILENAME)
    {
        g_debug("too many files at '%s'",filename);
        return;
    }
    int len = strlen(filename);
    if (len >= MAX_FILENAME)
    {
        g_debug("filename too long: len=%d '%s'",len,filename);
        return;
    }
    int avail = MAX_FILEBUFFER - g_filebuffer_ptr;
    if (avail < len)
    {
        g_debug("no room for filename: avail=%d len=%d '%s'",avail,len,filename);
        return;
    }

    char *ptr = &g_filebuffer[g_filebuffer_ptr];
    strcpy(ptr,filename);
    g_filesize[g_num_files] = size;
    g_filenames[g_num_files++] = ptr;
    g_filebuffer_ptr += len + 1;
}


static void sortFilenames()
    // sort directories first, case insensitive
{
    #if DEBUG_FILES
        g_debug("sort %d filenames",g_num_files);
    #endif

    int i = 0;
    while (i < g_num_files-1)
    {
        int cmp = 0;

        #if 0   // dirs first
            bool isdir1 = g_filesize[i] == -1;
            bool isdir2 = g_filesize[i+1] == -1;
            if (isdir1 && !isdir2)
                cmp = -1;
            else if (isdir2 && !isdir1)
                cmp = 1;
            else
        #endif
            cmp = strcasecmp(g_filenames[i],g_filenames[i+1]);

        if (cmp <= 0)
        {
            i++;
        }
        else
        {
            int jj = g_filesize[i];
            g_filesize[i] = g_filesize[i+1];
            g_filesize[i+1] = jj;

            char *ptr = g_filenames[i];
            g_filenames[i] = g_filenames[i+1];
            g_filenames[i+1] = ptr;
            if (i > 0)
            {
                i--;
            }
        }
    }
    #if DEBUG_FILES
        g_debug("sort finished");
    #endif

}


static void refreshDir()
{
    g_num_files = 0;
    g_filebuffer_ptr = 0;

    g_debug("refhresDir(%d,%s)",top_num,path);

    protectedFS fs(SD);     // for leafName();
    proFile root = fs.open(path);
    if (root && root.isDirectory())
    {
        proFile file = root.openNextFile();
        while (file)
        {
            #if DEBUG_FILES
                g_debug("file(%s)",file.leafName());
            #endif

            addFilename(file.leafName(),file.isDirectory() ?
                -1 : file.size());
            file = root.openNextFile();
        }
    }
    else
    {
        fileError("Could not opendir");
    }
    if (root)
        root.close();
    sortFilenames();

    // on refresh, constrain top_num in case files were deleted

    if (top_num > g_num_files - NUM_LINES)
    {
        top_num = g_num_files - NUM_LINES;
        if (top_num < 0) top_num = 0;
    }
}


void winFiles::update()
{
    bool path_changed = strcmp(last_path,path);
    if (path_changed || top_num != last_top_num)
    {
        draw_needed = true;

        last_top_num = top_num;
        strcpy(last_path,path);

        for (int i=0; i<NUM_LINES; i++)
        {
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

            if (path_changed)
            {
                refreshDir();
            }

            int num_lines = 0;
            int num_files = top_num;
            while (num_files < g_num_files &&
                   num_lines < NUM_LINES)
            {
                uiDualElement *dual = &files[num_lines];
                // note the leading space which is removed upon usage
                dual->bg = COLOR_DARKBLUE;
                dual->label = g_filenames[num_files];
                dual->value = &g_filesize[num_files];
                if (g_filesize[num_files] == -1)
                    dual->type |= ELEMENT_TYPE_FLAG_DIR;
                else
                    dual->type &= ~ELEMENT_TYPE_FLAG_DIR;
                num_lines++;
                num_files++;
            }

            num_files = g_num_files;

            #ifdef WITH_GRBL
                SD.end();
            #endif

            #if DEBUG_FILES
                g_debug("dir(%s) top(%d) lines(%d) files(%d)", path, top_num, num_lines, num_files);
            #endif

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
