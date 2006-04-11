#ifdef DEBUG

volatile struct Xlib_DebugInfo_st {
        char    *filename;
        int     linenumb;
        char    *procname,*procinfo;
} Xlib_DebugInfo[1024];

#define DBUG_ENTER(a)                                           \
        int DebugLevel = Xlib_DebugEnter(__FILE__,__LINE__,(a),"");
#define DBUG_POINT(a) {                                         \
                Xlib_DebugInfo[DebugLevel].linenumb = __LINE__; \
                Xlib_DebugInfo[DebugLevel].procinfo = a;        \
        }
#define DBUG_LEAVE {                                            \
                Xlib_DebugInfo[DebugLevel].linenumb = __LINE__; \
                Xlib_DebugOffset = --DebugLevel;                \
        }
#define DBUG_RETURN(a) {                                        \
				Xlib_DebugLeave((unsigned int)a);               \
                DBUG_LEAVE;                                     \
                return (a);                                     \
        }
#define DBUG_VOID_RETURN {                                      \
				Xlib_DebugLeaveVoid();                          \
                DBUG_LEAVE;                                     \
                return;                                         \
        }

volatile int Xlib_DebugOffset;
int Xlib_DebugEnter(char*,int,char*,char*);
void Xlib_DebugLeave(unsigned int a);
void Xlib_DebugLeaveVoid();

#else

#define DBUG_ENTER(a)
#define DBUG_POINT(a)
#define DBUG_RETURN(a) return (a)
#define DBUG_VOID_RETURN return

#endif
