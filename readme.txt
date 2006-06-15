
Doku:
=====
doc\changelist.txt - Liste der énderungen
doc\namespace.doc  - Erste lose Gedanken zur Policy bzgl.
                       - Verwendung vn .(i)h Headerfiles und was sie
                         exportieren sollten
                       - Namespace fÅr Funktionen
doc\modules.txt    - wirre Todoliste und meine Aufzeichungnen zu Modul-Details
doc\testcase.txt   - Aufzeichnungen aus meinen Tests
(alles andere in doc\diverse eher unwichtig)


Details zum makefile-konzept:
- - - - - - - - - - - - - - - 
- Makefiles defnieren relativ das Basisverzeichnis und das projektspezifische
  include namens project.in, und inkludieren dann rules.in
- Makros, die in den (Unter)makefiles verwendet werden, sind 
  dokumentiert im Konpf von config\rules.in
- project.in ruft bin\gccchk.cmd auf, um zu prÅfen, welcher Compiler
  (emx oder gxx) vorliegt (erste gcc.exe im PATH gewinnt). Das
  makefile von emhhandl bricht ab, wenn nicht emx vorliegt. Eine
  Restriktion fÅr alle anderen Module fÅr EMX wÅrde ggfs. Sinn machen,
  ist aber (noch) nicht drin. Effektiv braucht man nur kurz eine Shell
  mit EMX zu îffnen und "make all" einzugeben, um emxhandl zu bauen.


makefile im Hauptverzeichnis:
=============================

make        - erzeugt debug version
make rel    - erzeugt release Version
make clean  - lîscht "compile" und "debug", debug wird aber (noch) nicht
              richtig aufgerÑumt, die Unterverzeichnisse bleiben stehen

fÅr emx Umgebung: nur emxhandl wird gebaut
fÅr gcc Umgebung: alle anderen Module werden gebaut (emxhandle wird vorausgesetzt)

Die Reihenfoge der Module ist im Hauptmakefile mit den Variablen
UTL_MODULES LIB_MODULES XDM_MODULES X11_MODULES vorgegeben, es wird von
oben nach unten und von links nach rechts gebaut. Die Untermakefiles beachten
noch (!) keine AbhÑngigkeiten untereinander, rufen sich also noch nicht
gegenseitig auf. FÅrs erste, bis das Compilat lÑuft, reicht wohl erstmal das 
Hauptmakefile aus

Ausgabeverzeichnisse
- - - - - - - - - - -
compile - enthÑlt versionsunbabhÑngige Dateien
debug   - enthÑlt versionsabhÑngige Dateien *.dll *.lib *.res
debug\<Unterverzeichnisse> - enthÑlt *.obj je modul (siehe Makro OBJDIR in Untermakefiles)


makefile in Modulverzeichnis
============================
make          - baut debug version
make NDEBUG=1 - baut release version
make clean    - lîscht die erzeugten Dateien, aber nicht das OBJDIR

