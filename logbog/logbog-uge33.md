## Logbog, uge 33

I denne uge startede vi på et nyt projekt, som omhandler kommunikation med droner igennem UDP(User Datagram Protocol). 

Grunden til at UDP blev brugt som netværksprotokol i stedet for TCP, er fordi at UDP er hurtigere og simplere, da der ikke skal tjekkes om hver pakke blev modtaget, i stedet kan man bare holde øje med om dronen svare fx. ok.  Kommunikationen til og fra dronen foregår med simple ascii encoded strenge, som dronen lytter efter på port 8889, og ens program er derefter lytte på port 8890 efter en godkendelse eller hvis man har spurgt efter telemitri kan den også svare tilbage der. 

Dronen har også et kamera, som der også kan fås adgang til efter man har sendt den rigtige kommando. Kommunikation til og fra kameraet foregår igennem UDP hvor dataen er i H264 encoded video billeder. 

Disse billeder kan bl.a. afkodes igennem OpenCV, hvor man kan gemme de rå pixelværdier i datastrukturen `cv::Mat`.  Selve `cv::Mat` klassen er bare et 3d array, som gemmer pixelværdierne, dog så er der defineret nogle operatorer for klassen, som gør det nemt at få dataen ind fra cv::VideoCapture. Desuden så er klassen også integreret ind i cv::imshow, som gør det nemt at vise billederne.



Jeg startede ud med at få programmeringsmiljøet op og køre, samt at få den mest elementale kommunikation til at virke. Der var nogle problemer i forhold til at få sat de rigtige porte i koden, og muligvis har enhedernes firewalls, hvilket betød at det tog noget tid for at få det op og køre. 

Da jeg var overbevist om at jeg havde fået kommunikationen på plads startede jeg med at skrive en plan op for den overordnede samlede struktur for det færdige program. Dette program er vises, som et grafisk vindue, ved brug af ImGui og SFML + ImTerm. Heri kan jeg både simulere en terminal(med autocomplete), samt vise video, telemitri og forskellige indstillinger i et vindue. Da terminalen var op og køre, så ville jeg gerne integrere den med UDP. Derfor gik jeg igang med en UDP server, som bliver kørt på en seperat tråd, så jeg kan fortsætte med at vise gui'en. Denne UDP server har en queue så man kan håndtere requests der kommer lige efter hinanden.  Der er lagt op til at en request kan have en timeout. Indtil videre er der implemeteret en funktion i terminalen så jeg kan sende enhver string over udp, samt en quit, echo og clear kommando.