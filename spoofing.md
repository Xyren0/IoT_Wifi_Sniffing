Für den Fall, dass ihr lediglich Teil des Netzwerkes seid, empfiehlt es sich, Linux zu verwenden, da es die Vorgänge stark vereinfacht. (Wir haben Kali verwendet, da es mit dem Live Modus schnell einsatzbereit ist und die benötigte Software bereits vorhanden ist, die Webseite zu Kali findet ihr in den Ressourcen)

Da ihr nur ein Teil seid, wird der Verkehr eures Opfers nicht mehr über euch laufen. Um dies zu ermöglichen, gibt es folgende Möglichkeit: 

Als erstes solltet ihr die IP-Adresse eures Opfers herausfinden. Dazu kann der Befehl nmap genutzt werden. 

nmap -sP \<Gateway-IP\>/24 

z.B. Im Falle der Gateway-IP 192.168.137.1: nmap -sP 192.168.137.1/24

Als nächstes stellt ihr eure Maschine in den ip-forwarding Modus. Dies wird auf diese Weise erreicht.

![ip-forwarding](/IoTScreenshots/sslstrip_befehle_ipforward.png)

Nun muss eurem Opfer gesagt werden, dass ihr der Router seid und die Pakete zunächst and euch gesendet werden sollen. 
Damit ihr die Antworten des Servers bekommt müsst ihr zudem dem Router sagen, dass ihr euer Opfer seid um auch dessen Pakete zu erhalten.
Das wird mit dem Befehl arpspoof erreicht: 

![arpspoof2](/IoTScreenshots/sslstrip_befehle_arpspoof2.png)

Dabei wird mit dem Parameter -i angegeben, um welches Interface es sich handelt (meist wlan0), mit dem Parameter -t euer Opfer und die letzte IP-Adresse ist euer Gateway. 

Dieser Befehl muss nun noch in die andere Richtung ausgeführt werden, sprich mit Gateway und Opfer-IP vertauscht:

![arpspoof1](/IoTScreenshots/sslstrip_befehle_arpspoof1.png)

Habt ihr diese Befehle ausgeführt, könnt ihr wieder ganz einfach mit Wireshark die Pakete auslesen.

![mqtt com](/IoTScreenshots/reading_mqtt_com.png)

![mqtt-packet](/IoTScreenshots/reading_mqtt_pack.png)

Wie zu sehen ist, kommen die Pakete von eurem Opfer und ihr könnt mitlesen. 

Wenn ihr Glück habt, findet ihr Pakete, welche auf unsicheren Protokollen basieren und könnt dort direkt die Daten auslesen. Trefft ihr auf verschlüsselten Verkehr, könnt ihr es mit SSLStrip versuchen, das HTTPS auf HTTP downgradet. 
Mehr dazu unter [SSLStrip](https://github.com/Xyren0/IoT_Wifi_Sniffing/blob/master/sslstrip.md)
