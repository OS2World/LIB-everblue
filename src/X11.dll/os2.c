#include "X11.h"

char *__XOS2RedirRoot(char *fname)
{
    /* This adds a further redirection by allowing the ProjectRoot
     * to be prepended by the content of the envvar X11ROOT.
     * This is for the purpose to move the whole X11 stuff to a different
     * disk drive.
     * The feature was added despite various environment variables
     * because not all file opens respect them.
     */
    static char redirname[300]; /* enough for long filenames */
    char *root;

    /* if name does not start with /, assume it is not root-based */
    if (fname==0 || !(fname[0]=='/' || fname[0]=='\\'))
	return fname;

    root = (char*)getenv("X11ROOT");
    if (root==0 || 
	((fname[1]==':') && (isalpha((int)fname[0]))) ||
        (strlen(fname)+strlen(root)+2) > 300)
	return fname;
    sprintf(redirname,"%s%s",root,fname);
    return redirname;
}
