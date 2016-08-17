========================================================================
    DYNAMIC LINK LIBRARY: ZeliskoDll-Projektübersicht
========================================================================

Der Anwendungs-Assistent hat die ZeliskoDll-DLL erstellt. 

Die Datei enthält eine Zusammenfassung des Inhalts der Dateien für die
ZeliskoDll-Anwendung.


ZeliskoDll.vcproj
    Dies ist die Hauptprojektdatei für VC++-Projekte, die mit dem Anwendungs-Assistenten generiert werden. 
    Sie enthält Informationen zu der Version von Visual C++, mit der die Datei generiert wurde,
    sowie Informationen zu Plattformen, Konfigurationen und Projektfeatures, die mit dem
    Anwendungs-Assistenten ausgewählt wurden.

ZeliskoDll.cpp
    Dies ist die Quelldatei der Haupt-DLL.

	Diese DLL exportiert keine Symbole, deshalb wird beim Erstellen keine 
	LIB-Datei generiert. Wenn das Projekt
	eine Projektabhängigkeit eines anderen Projekts sein soll, müssen Sie 
	Code hinzufügen, um Symbole aus der DLL zu exportieren, damit eine Exportbibliothek 
	erstellt wird, oder Sie können die Eigenschaft "Eingabebibliothek ignorieren" 
	auf der Eigenschaftenseite "Allgemein" in den Projekteigenschaften auf "Ja" festlegen.
	Dialogfeld "Seiten".

/////////////////////////////////////////////////////////////////////////////
Weitere Standarddateien:

StdAfx.h, StdAfx.cpp
    Mit diesen Dateien werden vorkompilierte Headerdateien (PCH)
    mit der Bezeichnung ZeliskoDll.pch und eine vorkompilierte Typdatei mit der Bezeichnung StdAfx.obj erstellt.

/////////////////////////////////////////////////////////////////////////////
Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:"-Kommentare, um die Teile des Quellcodes anzugeben, die
hinzugefügt oder bearbeitet werden müssen.

/////////////////////////////////////////////////////////////////////////////s