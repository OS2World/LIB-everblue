
Doku:
=====
doc\changelist.txt - Liste der �nderungen
doc\namespace.doc  - Erste lose Gedanken zur Policy bzgl.
                       - Verwendung vn .(i)h Headerfiles und was sie
                         exportieren sollten
                       - Namespace f�r Funktionen
doc\modules.txt    - wirre Todoliste und meine Aufzeichungnen zu Modul-Details
doc\testcase.txt   - Aufzeichnungen aus meinen Tests
(alles andere in doc\diverse eher unwichtig)


Details zum makefile-konzept:
- - - - - - - - - - - - - - - 
- Makefiles defnieren relativ das Basisverzeichnis und das projektspezifische
  include namens project.in, und inkludieren dann rules.in
- Makros, die in den (Unter)makefiles verwendet werden, sind 
  dokumentiert im Konpf von config\rules.in
- project.in ruft bin\gccchk.cmd auf, um zu pr�fen, welcher Compiler
  (emx oder gxx) vorliegt (erste gcc.exe im PATH gewinnt). Das
  makefile von emhhandl bricht ab, wenn nicht emx vorliegt. Eine
  Restriktion f�r alle anderen Module f�r EMX w�rde ggfs. Sinn machen,
  ist aber (noch) nicht drin. Effektiv braucht man nur kurz eine Shell
  mit EMX zu �ffnen und "make all" einzugeben, um emxhandl zu bauen.


makefile im Hauptverzeichnis:
=============================

make        - erzeugt debug version
make rel    - erzeugt release Version
make clean  - l�scht "compile" und "debug", debug wird aber (noch) nicht
              richtig aufger�umt, die Unterverzeichnisse bleiben stehen

f�r emx Umgebung: nur emxhandl wird gebaut
f�r gcc Umgebung: alle anderen Module werden gebaut (emxhandle wird vorausgesetzt)

Die Reihenfoge der Module ist im Hauptmakefile mit den Variablen
UTL_MODULES LIB_MODULES XDM_MODULES X11_MODULES vorgegeben, es wird von
oben nach unten und von links nach rechts gebaut. Die Untermakefiles beachten
noch (!) keine Abh�ngigkeiten untereinander, rufen sich also noch nicht
gegenseitig auf. F�rs erste, bis das Compilat l�uft, reicht wohl erstmal das 
Hauptmakefile aus

Ausgabeverzeichnisse
- - - - - - - - - - -
compile - enth�lt versionsunbabh�ngige Dateien
debug   - enth�lt versionsabh�ngige Dateien *.dll *.lib *.res
debug\<Unterverzeichnisse> - enth�lt *.obj je modul (siehe Makro OBJDIR in Untermakefiles)


makefile in Modulverzeichnis
============================
make          - baut debug version
make NDEBUG=1 - baut release version
make clean    - l�scht die erzeugten Dateien, aber nicht das OBJDIR

