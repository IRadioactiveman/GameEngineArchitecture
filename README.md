# GameEngineArchitecture

Hallo, zum builden werden nur noch die lib Dateien für GLEW und SDL benötigt, die gibt es im Netz oder in Timos Projekt.

Project 1: Cube.exe
erstellt ein Fenster und malt einen bunten Würfel in die Mitte der Welt. Man kann sich mit der Maus wie in einem Ego Shooter umsehen und mit wads oder den Pfeiltasten bewegen.

Project 2: Scene.exe
erstellt ein Fenster und malt mehrere Würfel in dei Szene, man kann sich wieder frei bewegen. Es ist auch ein raycast implementiert, der leider nicht richtig funktioniert. Ich habe es auf 2 Arten probiert:
1. manuell: Mein eigens geschriebener Intersection Algorithmus funktioniert irgendwie nicht. (OBB Intersection)
2. mit einer Physik Engine (Bullet): Geht aus irgendeinem Grund auch nicht.
