Bei SSLStrip geht es darum, Verkehr, der über HTTPS läuft, also gesichert ist, zu zwingen über HTTP zu laufen und somit leicht auslesbar.
Dazu gibt es ein von Moxie Marlinspike erstelltes [Tool](https://moxie.org/software/sslstrip/)

Ist der Verkehr mit SSL verschlüsselt, können keine nähere Informationen ausgelesen werden:

![secure communication](/IoTScreenshots/secured_com_ssl.png)

Um dies zu umgehen benötigt es wieder die spoofing Befehle aus dem [Spoofing](https://github.com/Xyren0/IoT_Wifi_Sniffing/blob/master/spoofing.md)

Diese werden nun aber mit weiteren Befehlen erweitert:

![sslstrip commands](/IoTScreenshots/sslstrip_befehle_forward.png)

Diese Befehle stellen zunächst den Rechner in den ip-forwarding Modus. Anschließend wird der Traffic umgeleitet zu Port 1000, wo später das Tool laufen wird.
Als letztes wird sslstrip ausgeführt mit dem Parameter -l, der den Port, in diesem Fall 1000, angibt. 

Möchte man nun auf eine Seite zugreifen, bewirkt dieses Tool, dass die Seite über http aufgerufen wird und somit leicht abhörbar wird.
Moderne Browser verhindern dies jedoch größtenteils und erzwingen oftmals eine Verbindung über https, es ist also gut möglich, dass trotz 
des Tools mancher Verkehr nicht ausgelesen werden kann. 

Opfer vor SSLStrip: 

![before sslstrip](/IoTScreenshots/nonvic_google_https.PNG)


Opfer nach SSLStrip:

![sslstrip performed](/IoTScreenshots/vic_google_http.PNG)

Weiteres Beispiel mit Youtube, normalerweise läuft es über HTTPS. 
Jetzt kann es direkt ausgelesen werden:

![Youtube com](/IoTScreenshots/youtube_sslstrip.png)

![Youtube com](/IoTScreenshots/youtube_paket_sslstrip.png)

Es zeigt sich, dass das Abhören von Wifi-Verkehr nicht schwer ist, selbst wenn der Verkehr nicht direkt über einen selbst läuft bzw. laufen sollte.
Genauso gut wie hier anhand von Websiten gezeigt, funktionieren diese Methoden auch gut bei IoT-Geräten, dort teilweise sogar noch besser, da viele kleine Geräte nicht die benötigte Rechenleistung haben um ihre Kommunikation zu verschlüsseln. 
