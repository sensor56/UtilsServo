UtilsServo
==========

Librairie fournissant de nombreuses fonctions utiles pour le positionnement de servomoteurs : 
* positionnement brut
* positionnement progressif à l'angle absolue désiré
* positionnement progressif à l'angle relatif désiré vis à vis de la position courante
* positionnement synchrons de plusieurs servomoteurs

D'autre part, cette librairie intègre une fonction d'analyse de chaine reçue sur le port série pour un contrôle facilité des servomoteurs. Les chaînes reconnues sont : 
* home() 
* toHome() 

* vitesseServos(xxx) 

* servoPan(xxx) 
* servoPanTo(xxx) 
* servoPanToR(xxx) 
* servoTilt(xxx) 
* servoTiltTo(xxx) 
* servoTiltToR(xxx) 

* servo(index,xxx) 
* servoTo(index,xxx) 
* servoToR(index,xxx) 

* servosSync(xxx,xxx) 
* servosSyncR(xxx,xxx) 

* servosSync(xxx,xxx, xxx, xxx, xxx) 
* servosSyncR(xxx,xxx, xxx, xxx, xxx) 

La documentation de cette librairie est ici : http://www.mon-club-elec.fr/pmwiki_reference_lib_arduino_perso/pmwiki.php?n=Main.UtilsServo
