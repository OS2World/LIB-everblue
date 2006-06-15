/**/
 '@ECHO OFF';
 env = 'OS2ENVIRONMENT';

 call RxFuncAdd    'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

 rc = 0;

 Debuggee    = 'test.exe';  /* modify filename here */


 SubDirList = 'src\samples\test', 
              'src\lib\common',
              'src\lib\emxhandl',
              'src\lib\ebhook',
              'src\xdaemon\lib',
              'src\xdaemon\app',
              'src\lib\x11';

 DebugSubDir = 'debug';
 FilesList   = 'ebhook.dll emxhandl.dll xdaemon.dll x11.dll',
               'xdaemon.exe' Debuggee;


 DO 1
    /* check for debugger */
    ExeName = SysSearchPath( 'PATH', 'ICSDEBUG.EXE');
    IF (ExeName = '') THEN
    DO
       SAY 'error: debugger of Visual Age Compiler not found.';
       rc = 2;
       LEAVE;
    END;

    /* determine basepath of project */
    /* this script resides in bin subdirectory */
    PARSE SOURCE . . ScriptName;
    CallDir = LEFT( ScriptName, LASTPOS( '\', ScriptName) - 1);
    BaseDir = LEFT( CallDir, LASTPOS( '\', CallDir) - 1);

    /* check for required files */
    MissingFiles = ''
    DO WHILE (FilesList \= '')
       PARSE VAR FilesList File FilesList;
       IF (\FileExist( BaseDir'\'DebugSubDir'\'File)) THEN
          MissingFiles = MissingFiles File
    END;
    IF (MissingFiles \= '') THEN
    DO
       SAY 'error: files missing in debug diectory:'
       SAY '     ' MissingFiles;
       rc = 2;
       LEAVE;
    END;

    /* assemble path for sources */
    PmdPath = '';
    DO WHILE (SubDirList \= '')
       PARSE VAR SubDirList SubDir SubDirList;
       PmdPath = PmdPath''BaseDir'\'SubDir';'
    END;

    /* set fallback point for environment */

    /* changes and modify environment     */
    rcx = SETLOCAL();
    rcx = VALUE( 'PMDPATH', PmdPath, env);

    /* change to debug directory */
    IF (DIRECTORY( BaseDir'\'DebugSubDir) = '') THEN
    DO
       SAY 'error: debug directory not found.';
       rc = 3;
       LEAVE;
    END;

    /* launc debugger */
    ExeName Debuggee;
 END;

 RETURN( rc);
  

/* ------------------------------------------------------------------------- */
FileExist: PROCEDURE
 PARSE ARG FileName

 RETURN(STREAM(Filename, 'C', 'QUERY EXISTS') > '');

