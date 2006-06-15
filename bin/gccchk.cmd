/*
 *   GCCCHK.CMD - C.Langanke - Everblue Project 2006
 */

 call RxFuncAdd    'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

 '@ECHO OFF';

 DO 1
    /* get name of output file */
    PARSE ARG OutFile;
    OutFile = STRIP( OutFile);
    IF (OutFile = '') THEN
       LEAVE;

    /* reset and rewrite file */
    'DEL' OutFile '>NUL 2>&1';
    PARSE VALUE GetGccType() WITH GccType GccVersion
    rc = LINEOUT( Outfile, 'GCCTYPE='GccType);
    rc = LINEOUT( Outfile, 'GCCVERSION='GccVersion);
    rc = LINEOUT( Outfile);
 END;

 RETURN( 0);

/* =============================================================== */
GetGccType: PROCEDURE

 GccType = '';

 DO 1

    /* search GCC */
    ExecName = SysSearchPath( 'PATH', 'gcc.exe');
    IF (ExecName = '') THEN
       LEAVE

    /* determine gcc version */
    QueueName = RXQUEUE('CREATE');
    rc        = RXQUEUE('SET', QueueName);

    ExecName '--version | rxqueue' QueueName;
    PARSE PULL ThisLine;
    DO WHILE (ThisLine \= '')
       PARSE VAR ThisLine ThisWord ThisLine;
       PARSE VAR ThisWord GccVersionMajor'.'.;
       IF (DATATYPE( GccVersionMajor) = 'NUM') THEN
       DO
          SELECT
             WHEN (GccVersionMajor < 3) THEN GccType = 'gcc2' ThisWord;
             WHEN (GccVersionMajor > 2) THEN GccType = 'gcc3' ThisWord;
             OTHERWISE NOP;
          END;
          IF (GccType \= '') THEN
             LEAVE;
       END;
    END;

    rc = RXQUEUE('DELETE', QueueName);
    rc = RXQUEUE('SET', 'SESSION');

 END;

 RETURN( GccType);

