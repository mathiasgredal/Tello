## Logbog, uge 34

I denne uge lærte vi om hvordan man kan bruge funktioner, som parametre til andre funktioner. I mit program blev der integreret "callback" funktioner i UDP server programmet, for at kunne finjustere den kommando der blev kørt på hver request, samtidig med at man ikke skulle stoppe program eksekveringen, når man venter på et svar. 

Da det var problematisk at få boost.asio til at virke, så der er blevet skiftet over til uvw/libuv til asyncron udo kommunikation.

De forskellige dele af programmet er også mere løst koblet fra hinanden nu, så man f.eks. først starter videoforbindelsen igennem en knap i programmet.

