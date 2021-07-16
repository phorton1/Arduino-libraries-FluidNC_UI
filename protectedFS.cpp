

#include "protectedFS.h"
#include "Grbl_MinUI.h"


#define DEBUG_PROTECTED_FS  0


portMUX_TYPE protected_fs_mutex = portMUX_INITIALIZER_UNLOCKED;



proFile::proFile()
{
}

proFile::proFile(File file) :
    _file(file)
{
}

void proFile::close()
{
    #if DEBUG_PROTECTED_FS
        debug_serial("close(%s)",name());
    #endif
    if (p_fs)
        p_fs->unlinkFile(this);
    _file.close();
}

proFile proFile::openNextFile()
{
    File file = _file.openNextFile();
    proFile pf(file);
    return pf;
}



const char *proFile::leafName()
{
    const char *p = name();
    const char *rslt = p;
    while (*p)
    {
        if (*p == '/')
            rslt = p+1;
        p++;
    }
    return rslt;
}



protectedFS::protectedFS(fs::FS& fs) :
    _fs(fs)
{
}


void protectedFS::linkFile(proFile *pf)
{
    vTaskEnterCritical(&protected_fs_mutex);
    pf->p_prev = p_last;
    if (p_last)
    {
        p_last->p_next = pf;
        p_last = pf;
    }
    if (!p_first)
    {
        p_first = pf;
        p_last = pf;
    }
    pf->p_fs = this;
    vTaskExitCritical(&protected_fs_mutex);
}


void protectedFS::unlinkFile(proFile *pf)
{
    if (pf->p_fs)
    {
        vTaskEnterCritical(&protected_fs_mutex);
        pf->p_fs = 0;
        if (pf->p_next) pf->p_next->p_prev = pf->p_prev;
        if (pf->p_prev) pf->p_prev->p_next = pf->p_next;
        if (p_last == pf)
            p_last = pf->p_prev;
        if (p_first == pf)
            p_first = pf->p_next;
        pf->p_next = 0;
        pf->p_prev = 0;
        vTaskExitCritical(&protected_fs_mutex);
    }
}


proFile protectedFS::open(const char* path, const char* mode/* = FILE_READ*/)
{
    #if DEBUG_PROTECTED_FS
        debug_serial("open(%s)",path);
    #endif
    File file = _fs.open(path,mode);
    proFile pf(file);
    if (pf)
        linkFile(&pf);
    return pf;
}
proFile protectedFS::open(const String& path, const char* mode/* = FILE_READ*/)
{
    return open(path.c_str(),mode);
}


bool protectedFS::exists(const char* path)
{
    return _fs.exists(path);
}
bool protectedFS::exists(const String& path)
{
    return _fs.exists(path);
}


bool protectedFS::remove(const char* path)
{
    if (!check_path(path))
        return false;
    return _fs.remove(path);
}
bool protectedFS::remove(const String& path)
{
    return remove(path.c_str());
}


bool protectedFS::rename(const char* pathFrom, const char* pathTo)
{
    if (!check_path(pathFrom))
        return false;
    if (!check_path(pathTo))
        return false;
    return _fs.rename(pathFrom,pathTo);
}
bool protectedFS::rename(const String& pathFrom, const String& pathTo)
{
    return rename(pathFrom.c_str(),pathTo.c_str());
}


bool protectedFS::mkdir(const char *path)
{
    if (!check_path(path))
        return false;
    return _fs.mkdir(path);
}
bool protectedFS::mkdir(const String &path)
{
    return mkdir(path.c_str());
}


bool protectedFS::rmdir(const char *path)
{
    if (!check_path(path))
        return false;
    return _fs.rmdir(path);
}
bool protectedFS::rmdir(const String &path)
{
    return rmdir(path.c_str());
}



bool protectedFS::check_path(const char *path)
{
    proFile *ptr = p_first;
    int len = strlen(path);
    // debug_serial("check_path len(%d) %s",len,path);

    // the name() is fully qualified

    while (ptr)
    {
        const char *tpath = ptr->name();    // fully qualified path including terminal node
        int tlen = strlen(tpath);
        // debug_serial("    checking tlen(%d) %s",tlen,tpath);

        if (!strncmp(path,tpath,len))      // if they match upto the length of the path being removed
        {
            if (len == tlen)                // they are the same
            {
                #if DEBUG_PROTECTED_FS
                    debug_serial("---> check_path(%s) fail len==tlen",path);
                #endif
                return false;               // can't remove the directory
            }

            // tlen must be greater than len here
            // so it's ok to delete the path if the next
            // character in tpath is not a forward slash
            // i.e. it's a different path ..

            if (tpath[len] == '/')
            {
                #if DEBUG_PROTECTED_FS
                    debug_serial("---> check_path(%s) fail tpath[%d]='/' path2(%s)",path,len,tpath);
                #endif
                return false;
            }
        }

        ptr = ptr->p_next;
    }
    return true;
}