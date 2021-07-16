#pragma once

// A wrapper around a file system for re-entrancy sanity.
// Although, from the point of view of the file system, it
// might be "ok" to allow files to be deleted or renamed
// WHILE they are opened for reading, from a human perspective
// there are a number of things that just don't make sense.
//
// This wrapper ensures the sanctity of the files and directories
// in a multi-tasking environment by disallowing certain nonsensical
// operations.  If you open a directory and are traversing it, no one
// else should be able to delete it or rename it while you are in the
// middle.  If a file is opened, no-one else should be allowed to move
// it to another location while you are reading or writing to it.
//
// Essentially this implements a file-locking system on top of the file
// system such that files and directories are opened with "read locks"
// that prevent anyone else from diddling with the objects while they
// are open.
//
// It also has a limited notion of an "idle" file system which is one
// on which no files or directories are currently opened.

#include <FS.h>

class protectedFS;
    // forward

class proFile
{
    public:

        proFile();

        void close();
        operator bool() const                           { return (bool) _file; }

        const char *name()                              { return _file.name(); }
        boolean isDirectory(void)                       { return _file.isDirectory(); }
        int32_t size()                                  { return _file.size(); }
        int32_t position()                              { return _file.position(); }

        void seek(size_t i, fs::SeekMode how)           { _file.seek(i,how); }
        int32_t write(uint8_t c)                        { return _file.write(c); }
        int32_t write(const uint8_t *buf, size_t size)  { return _file.write(buf,size); }
        void flush()                                    { _file.flush(); }

        int32_t available()                             { return _file.available(); }
        int peek()                                      { return _file.peek(); }
        int read()                                      { return _file.read(); }

        int32_t readBytes(char *buf, size_t len)        { return _file.readBytes(buf,len); }
        int32_t read(uint8_t* buf, size_t len)          { return _file.read(buf,len); }

        proFile openNextFile();

        const char *leafName();
        

    protected:

        friend class protectedFS;

        proFile(File file);

        File _file;
        protectedFS *p_fs;

        proFile *p_next = 0;
        proFile *p_prev = 0;
};



class protectedFS
{
public:

    protectedFS(fs::FS& fs);

    proFile open(const char* path, const char* mode = FILE_READ);
    proFile open(const String& path, const char* mode = FILE_READ);

    bool exists(const char* path);
    bool exists(const String& path);

    bool remove(const char* path);
    bool remove(const String& path);
        // i wonder if this removes dirs recursively?

    bool rename(const char* pathFrom, const char* pathTo);
    bool rename(const String& pathFrom, const String& pathTo);

    bool mkdir(const char *path);
    bool mkdir(const String &path);

    bool rmdir(const char *path);
    bool rmdir(const String &path);

    bool idle()     { return p_first == NULL; }

protected:

    friend class proFile;

    void linkFile(proFile *pf);
    void unlinkFile(proFile *pf);
    bool check_path(const char *path);

    fs::FS& _fs;

    proFile *p_first = 0;
    proFile *p_last  = 0;

};
