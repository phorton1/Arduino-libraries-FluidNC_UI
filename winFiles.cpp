//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "winFiles.h"
#include "gBuffer.h"
#include "dlgMsg.h"
#include "dlgConfirm.h"
#include <SD.h>
#include <string.h>
#include "protectedFS.h"

#define DEBUG_FILES  1


//----------------------
// implementation
//----------------------

winFiles files_win;



void winFiles::fileError(const char *text)
{
    setErrMsg(text);
    g_debug("FILE ERROR: %s",text);
    m_file_error = true;
}


void winFiles::getLine(int line_num, const char **left_side, const char **right_side)
{
    char *p = gs_buffer;
    while (line_num)
    {
        while (*p) p++;
        p++;
        while (*p) p++;
        p++;
        line_num--;
    }
    *left_side = p;
    while (*p) p++;
    p++;
    *right_side = p;
}


const char *winFiles::getFileToRun(int i) const
{
    int line_num = m_top_num + i;
    const char *p = gs_buffer;
    while (line_num)
    {
        while (*p) p++;
        p++;
        while (*p) p++;
        p++;
        line_num--;
    }
    return p;
}


bool winFiles::canGoHome()  { return strcmp(m_path,"/"); }
bool winFiles::canGoBack()  { return strcmp(m_path,"/"); }
void winFiles::onGoHome()   { setPath("/",true); }
void winFiles::onGoBack()
{
    char *p = m_path;
    char *last = p;
    while (*p)
    {
        if (*p == '/')
        {
            last = p;
        }
        p++;
    }
    *last = 0;
    setPath(m_path,true);
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
        !strcmp(new_path,"gcode") ||
        !strcmp(new_path,"ngc");
        !strcmp(new_path,"G") ||
        !strcmp(new_path,"GC") ||
        !strcmp(new_path,"NGC");
}


void winFiles::onLineClicked(uiDualElement *dual, int top_num, int rel_line_num)
{
    const char *fn = dual->label;

    #if DEBUG_FILES
        g_debug("onLineClicked(%d,%s)",rel_line_num,fn);
    #endif

    if (fn[0] == '/')
    {
        fn++;
        setPath(fn,false);
    }
    else if (isGcodeFile(fn))
    {
        m_last_path[0] = 0;
        dlg_confirm.setConfirm(CONFIRM_COMMAND_RUN_FILE + rel_line_num);
        the_app.openWindow(&dlg_confirm);
    }
    else
    {
        warningMsg("File is not gcode!");
    }
}




void winFiles::setPath(const char *ipath, bool absolute)
{
    if (absolute)       // ipath includes the slash
    {
        strcpy(m_path,ipath);
    }
    else                // ipath gets added to current path
    {
        if (strlen(m_path) + strlen(ipath) + 1 >= MAX_FILE_PATH)
        {
            errorMsg("path too long!!");
            return;
        }
        if (strcmp(m_path,"/"))
            strcat(m_path,"/");
        strcat(m_path,ipath);
    }
    if (!m_path[0])
        strcpy(m_path,"/");

    m_top_num = 0;
    m_last_top_num = -1;
    m_last_path[0] = 0;
    m_buffer_version = 0;

    #if DEBUG_FILES
        g_debug("setPath(%s)",m_path);
    #endif
}


void winFiles::begin()
{
    uiScrollable::begin();
    if (m_file_error)
    {
        m_file_error = false;
        setErrMsg(NULL);
        setPath("/",true);
    }
    else
    {
        m_last_path[0] = 0;
    }
    m_draw_needed = true;
}



bool winFiles::addFilename(const char *filename, int size)
{
    int len_fn = strlen(filename);

    #if DEBUG_FILES > 1
        g_debug("adding %s %d at %d",filename,size,gs_buf_ptr);
    #endif

    if (size == -1)     // directory
    {
        if (gs_buf_ptr + len_fn + 3 >= MAX_G_BUFFER)
        {
            g_debug("too many files at directory '%s'",filename);
            return false;
        }
        gs_buffer[gs_buf_ptr++] = '/';
        strcpy(&gs_buffer[gs_buf_ptr],filename);
        gs_buf_ptr += len_fn + 1;
        gs_buffer[gs_buf_ptr++] = 0;
    }
    else
    {
        char tmp_buf[12];
        sprintf(tmp_buf,"%d",size);
        int len_tmp = strlen(tmp_buf);
        if (gs_buf_ptr + len_fn + len_tmp + 2 >= MAX_G_BUFFER)
        {
            g_debug("too many files at '%s'",filename);
            return false;
        }
        strcpy(&gs_buffer[gs_buf_ptr],filename);
        gs_buf_ptr += len_fn + 1;
        strcpy(&gs_buffer[gs_buf_ptr],tmp_buf);
        gs_buf_ptr += len_tmp + 1;
    }
    m_num_lines++;
    return true;
}



void winFiles::sortFilenames()
    // laborious sort in place with no cached pointers
{
    #if DEBUG_FILES
        g_debug("sort %d filenames",m_num_lines);
    #endif

    int i = 0;
    char *p1 = gs_buffer;
    while (i < m_num_lines-1)
    {
        char *p2 = p1;
        while (*p2) p2++;
        p2++;
        char *psize1 = p2;
        while (*p2) p2++;
        p2++;
        char *psize2 = p2;
        while (*psize2) psize2++;
        psize2++;

        #if DEBUG_FILES > 1
            g_debug("compare(%d) %s:%s to %s:%s",i,p1,psize1,p2,psize2);
        #endif

        char *pcmp1 = p1;
        char *pcmp2 = p2;
        if (*pcmp1 == '/') pcmp1++;
        if (*pcmp2 == '/') pcmp2++;
        int cmp = strcasecmp(pcmp1,pcmp2);

        if (cmp <= 0)
        {
            #if DEBUG_FILES > 2
                g_debug("skipping(%d) %s:%s",i,p1,psize1);
            #endif

            i++;
            p1 = p2;
        }
        else
        {
            #define MAX_FILENAME  128
            char filename[MAX_FILENAME];
            int copy1_len = strlen(p1) + strlen(psize1) + 2;
            int copy2_len = strlen(p2) + strlen(psize2) + 2;
            if (copy1_len > MAX_FILENAME)
            {
                fileError("Cannot sort directory!");
                return;
            }

            char *to2 = &p1[copy2_len];
            memcpy(filename,p1,copy1_len);
            memcpy(p1,p2,copy2_len);
            memcpy(to2,filename,copy1_len);

            #if DEBUG_FILES > 2
                g_debug("after swapping %s and %s",p1,to2);
            #endif

            if (i>0)
            {
                i--;
                if (i == 0)
                {
                    p1 = gs_buffer;
                }
                else
                {
                    p1 -= 2;            // skip previous size delimiter
                    while (*p1) p1--;   // skip size if any
                    p1--;               // skip name delimiter
                    while (*p1) p1--;   // until before previous name
                    p1++;               // and then forward one
                }
            }
        }
    }
    #if DEBUG_FILES > 1
        g_debug("sort finished");
    #endif

}


uint32_t winFiles::updateBuffer()
{
    g_debug("winFiles::updateBuffer(%d,%s)",m_top_num,m_path);

    // invalidate the global buffer, take ownership of the version
    // and set base number of lines and buffer pointer to zero

    gs_buffer_version++;
    gs_buf_ptr = 0;
    m_num_lines = 0;

    // start the SDCard as necessary
    // winFiles uses g_status directly

    if (g_status.getSDState(true) != SDState::Idle)
    {
        fileError("Could not get SDCard");
        return gs_buffer_version;
    }

    // iterate over the directory

    protectedFS fs(SD);     // for leafName();
    proFile root = fs.open(m_path);
    if (root && root.isDirectory())
    {
        proFile file = root.openNextFile();
        while (file)
        {
            if (!addFilename(file.leafName(),
                file.isDirectory()? -1 : file.size()))
            {
                break;
            }

            file = root.openNextFile();
        }
    }
    else
    {
        fileError("Could not opendir");
    }
    if (root)
        root.close();


    // SD.end();

    sortFilenames();

    return gs_buffer_version;
}
