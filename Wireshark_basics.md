Ein bekanntes und mächtiges Tool um Netzwerkverkehr auszulesen ist "Wireshark"
Es gibt noch weitere Tools, doch Wireshark ist das am weitesten verbreitete und sehr zu empfehlen und wurde im Zuge des Projekts von uns verwendet.

Verwendung von Wireshark: 
Zuerst muss das passende Interface ausgewählt werden. Logischerweise muss hier das Interface gewählt werden, in dem sich das Gerät befindet, dessen Kommunikation abgehört werden soll.

![Wireshark Interfaces](/IoTScreenshots/Wireshark_mit_Hotspot_Interface.png)

Wollt ihr Wifi-Verkehr abhören, so wählt das entsprechende Interface aus. 

Stellt ihr mit einem Hotspot selbst das Netzwerk bereit, kann direkt mit dem Sniffing begonnen werden. In unserem Fall ist dies das Interface "Lan-Verbindung 15".
Je nach Betriebssystem ist die Bezeichnung anders, kann aber leicht in den Einstellungen gefunden werden. 

Jeglicher Verkehr von Geräten, die mit eurem bereitgestellten Netzwerk verbunden sind, läuft über euch und kann analysiert werden. 
(Falls ihr nur Teil des Netzwerkes seid, siehe spoofing)

Um die benötigten Pakete nun zu finden, kann man die Pakete filtern, beispielsweise nach dem verwendeten Protokoll oder der IP-Adresse. 

![http packets](/IoTScreenshots/reading_http_com.png)

Hier wird nach http-Paketen der Maschine mit der IP-Adresse 192.168.137.38 gesucht. 
Auf der Wireshark Webseite findet ihr eine Dokumentation mit allen Befehlen. 

Klickt ihr nun auf diese Pakete, können sie genauer untersucht werden. Handelt es sich um ungesicherte Protokolle kann man die Daten direkt in der Payload des Protokolles auslesen. 

![http packets inspect](/IoTScreenshots/reading_http_pack.png)

Untersucht man eines der Pakete genauer, kann man sehen, dass es sich bei der besuchten Seite um die Seite des Schalke04 handelt.
Besucht die Person eine ungesicherte Seite, in der ein Passwort angegeben werden muss, so kann dies auch direkt ausgelesen werden. 
