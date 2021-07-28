//-------------------------------
// SPIFFS based preferences
//-------------------------------


#include "gPrefs.h"

#ifdef WITH_APPLICATION

    #include "gApp.h"
    #include <FS.h>
    #include <SPIFFS.h>

    #define DEBUG_PREFS     0

    #define PREF_TYPE_INT   0
    #define PREF_TYPE_S1    1
    #define PREF_TYPE_S128  2

    #define PREF_FILENAME                "/ui_prefs.txt"
    #define PREF_MAXLINE                 (128+15)
    #define PREF_DEFAULT_JOG_FEED_RATE   1500

    // prh - if these are static, I think the compiler optimizes out
    // the setting of jog_feed_rate

    static int jog_feed_rate = PREF_DEFAULT_JOG_FEED_RATE;
    static char macro_char[4][2];
    static char macro_path[4][MAX_MACRO_PATH+1];

    typedef struct pref_t {
        const uint8_t type;
        const char *name;
        const void *ptr;
    };

    static const pref_t prefs[] = {
        {PREF_TYPE_INT,    "JOG_FEED_RATE",  &jog_feed_rate, },
        {PREF_TYPE_S1,     "MACRO1_CHAR",    macro_char[0],  },
        {PREF_TYPE_S1,     "MACRO2_CHAR",    macro_char[1],  },
        {PREF_TYPE_S1,     "MACRO3_CHAR",    macro_char[2],  },
        {PREF_TYPE_S1,     "MACRO4_CHAR",    macro_char[3],  },
        {PREF_TYPE_S128,   "MACRO1_PATH",    macro_path[0],  },
        {PREF_TYPE_S128,   "MACRO2_PATH",    macro_path[1],  },
        {PREF_TYPE_S128,   "MACRO3_PATH",    macro_path[2],  },
        {PREF_TYPE_S128,   "MACRO4_PATH",    macro_path[3],  },
    };

    #define NUM_PREFS       (sizeof(prefs) / sizeof(pref_t))

    void parsePref(int line,char *buf)
    {
        char *val = buf;
        while (*val && *val != '=') val++;
        if (*val == '=')
        {
            *val++ = 0;

            #if DEBUG_PREFS
                g_debug("PREF[%s}=%s]",buf,val);
            #endif

            for (int i=0; i<NUM_PREFS; i++)
            {
                const pref_t *pref = &prefs[i];
                if (!strcmp(buf,pref->name))
                {
                    if (pref->type == PREF_TYPE_INT)
                    {
                        *((int *) pref->ptr) =  atoi(val);
                    }
                    else
                    {
                        strcpy((char *)pref->ptr,val);
                    }
                    return;
                }
            }
            g_debug("Error: Could not find pref[%d} named %s",line,buf);
        }
        g_debug("Error: Could not find '=' on prefs line[%d} '%s'",line,buf);
    }


    void readPrefs()
    {
        #if DEBUG_PREFS
            g_debug("readPrefs()");
        #endif

        if (SPIFFS.exists(PREF_FILENAME))
        {
            File pfile = SPIFFS.open(PREF_FILENAME,FILE_READ);
            if (pfile)
            {
                int len = 0;
                int line = 0;
                char buf[PREF_MAXLINE+1];
                int c = pfile.read();
                while (c != -1)
                {
                    if (c == 13)
                    {
                        buf[len] = 0;
                        parsePref(line,buf);
                        len = 0;
                        line++;
                    }
                    else if (len < PREF_MAXLINE)
                    {
                        buf[len++] = c;
                    }
                    else
                    {
                        g_debug("Warning/Error: pref_line[%d] too long",line);
                    }
                    c = pfile.read();
                }

                pfile.close();
            }
            else
            {
                g_debug("Error: could not open %s for reading",PREF_FILENAME);
            }
        }
        #if DEBUG_PREFS
            else
            {
                g_debug("Warning: %s does not exist",PREF_FILENAME);
            }
        #endif
    }


    static void writePrefs()
    {
        #if DEBUG_PREFS
            g_debug("writePrefs()");
        #endif
        File pfile = SPIFFS.open(PREF_FILENAME,FILE_WRITE);
        if (pfile)
        {
            for (int i=0; i<NUM_PREFS; i++)
            {
                char buf[PREF_MAXLINE+1];
                const pref_t *pref = &prefs[i];
                if (pref->type == PREF_TYPE_INT)
                {
                    sprintf(buf,"%s=%d\r",pref->name,*((int *)pref->ptr));
                }
                else
                {
                    sprintf(buf,"%s=%s\r",pref->name,((char *)pref->ptr));
                }

                #if DEBUG_PREFS
                    g_debug("--->PREF %s",buf);
                #endif

                int len = strlen(buf);
                if (pfile.write((const uint8_t *)buf,len) != len)
                {
                    g_debug("Error: could not write '%s' to '%s'",buf,PREF_FILENAME);
                    pfile.close();
                    return;
                }
            }

            pfile.close();
        }
        else
        {
            g_debug("Error: Could not open %s for writing",PREF_FILENAME);
        }
    }



    int  getIntPref(int id)
    {
        if (prefs[id].type != PREF_TYPE_INT)
        {
            g_debug("Error: illegal call to getIntPref(%d)",id);
            return 0;
        }
        return *((volatile int *) prefs[id].ptr);
    }


    const char *getStrPref(int id)
    {
        if (prefs[id].type != PREF_TYPE_S1 &&
            prefs[id].type != PREF_TYPE_S128)
        {
            g_debug("Error: illegal call to getStrPref(%d)",id);
            return "";
        }
        return (const char *) prefs[id].ptr;
    }


    void setIntPref(int id, int value)
    {
        if (prefs[id].type != PREF_TYPE_INT)
        {
            g_debug("Error: illegal call to setIntPref(%d,%d)",id,value);
            return;
        }
        *((int *) prefs[id].ptr) = value;
        writePrefs();
    }


    void setStrPref(int id, const char *value)
    {
        if (prefs[id].type != PREF_TYPE_S1 &&
            prefs[id].type != PREF_TYPE_S128)
        {
            g_debug("Error: illegal call to setStrPref(%d,%s)",id,value);
            return;
        }
        int maxlen = prefs[id].type == PREF_TYPE_S1 ? 1 : 128;
        int len = strlen(value);
        if (len > maxlen)
        {
            g_debug("WARNING: setStrPref(%d,%s) len(%d) truncating to %d",id,value,len,maxlen);
            len = maxlen;
        }
        char *p = (char *) prefs[id].ptr;
        memcpy(p,value,len);
        p[len] = 0;
        writePrefs();
    }

#endif  // WITH_APPLICATION
