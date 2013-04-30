/*
 * Copyright (c) 2012 by Xavier HINAULT - support@mon-club-elec.fr
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

#include "Arduino.h"
#include "Servo.h"
#include "Utils.h"
#include "UtilsServo.h"

  UtilsServo::UtilsServo(){ // constructeur par défaut 
  
  }

  //UtilsServo::UtilsServo(Servo *servos, float (*anglesServosIn)[3], int nombreServosIn){ // constructeur 
  UtilsServo::UtilsServo(const int nombreServosIn){ // constructeur 

	this->_vitesseServos=10; // variable utilisée pour délai entre pour vitesse servomoteur en ms
	this->_pasServos=1; // variable utilisée pour cran angle entre 2 positions intermédiaires (degrés)

	this->_debug=true; // flag de message de debug  

	this->_ANGLE_MIN=0; // angle position MIN en degrés
	this->_POS_MIN=550; // largeur impulsion pour position ANGLE_MIN degrés du servomoteur
        // POS_MIN=550 pour ANGLE_MIN=0 avec un futaba S3003

	this->_ANGLE_MAX=172; // angle position MAX en degrés
	this->_POS_MAX=2400; // largeur impulsion pour position ANGLE_MAX degrés du servomoteur
        // POS_MAX=2400 pour ANGLE_MAX=172 pour futaba S3003

	this->_nombreServos=nombreServosIn; // mémorise le nombre de servomoteurs

	this->_indexServoPan=0; // index par défaut du servomoteur Pan
	this->_indexServoTilt=1; // index par défaut du servomoteur Tilt

	//---- mémorise le tableau des broches 
	//this->_brochesServos[nombreServosIn]; // initialise le tableau interne // ne pas déclarer ici mais dans *.h

	//----- mettre les initialisations des int etc.. avant.. 

	//this->_servos[nombreServosIn]; //initialise le tableau de servomoteurs // ne pas déclarer ici mais dans *.h
	//this->_anglesServos[nombreServosIn][3]; // initialise  le tableau des angles des servomoteurs // ne pas déclarer ici mais dans *.h


} // fin constructeur

/////////// fonctions de paramétrages //////////////
 
void UtilsServo::anglesServo(int index,float angle0, float angle1, float angle2){ // initialise angles du servomoteur 

	Serial.print("Index="), Serial.println(index); 

	this->_anglesServos[index][0]= angle0; // [0] : angle courant = angle actuel du servo
	this->_anglesServos[index][1]= angle1; // [1] : angle de travail - utilisé pour positions intermédiaires
	this->_anglesServos[index][2]= angle2; // [2] : angle Initial (Home)

} // fin anglesServo

void UtilsServo::init(int brochesServosIn[]) {

// -------- 	

	for (int i=0; i<_nombreServos; i++) {
		Serial.print(brochesServosIn[i]), Serial.print(":"); 
		this->_brochesServos[i]=brochesServosIn[i]; 
		Serial.print(_brochesServos[i]); 
	} // fin for


//------- affichage des paramètres utilisés ---------- 
    Serial.println(F("--- Valeur des variables internes --- ")); 
    
    Serial.print(F("vitesse Servos : ")), Serial.println(_vitesseServos); 
    Serial.print(F("pas Servos : ")), Serial.println(_pasServos); 

    Serial.print(F("Angle min : ")), Serial.println(_ANGLE_MIN); 
    Serial.print(F("Pos min : ")), Serial.println(_POS_MIN); 
    Serial.print(F("Angle max : ")), Serial.println(_ANGLE_MAX); 
    Serial.print(F("Pos max : ")), Serial.println(_POS_MAX); 

    Serial.print(F("Nombre Servos: ")), Serial.println(_nombreServos); 

    Serial.print(F("index Servo Pan : ")), Serial.println(_indexServoPan); 
    Serial.print(F("index Servo Tilt : ")), Serial.println(_indexServoTilt); 

	Serial.print(F("Broches servos :")); 

	for (int i=0; i<_nombreServos; i++) {
		Serial.print(_brochesServos[i]), Serial.print(F(", ")); 
	} // fin for

	Serial.println(); 

	//---- activation des servomoteurs --- 
	for (int i=0; i<_nombreServos; i++) {
	  this->_servos[i].attach(this->_brochesServos[i]);  // attache l'objet servo à la broche de commande du servomoteur Pan  
	  // se positione à 90° par défaut   
	} // fin for

	//----- broches des servomoteurs en sortie
	for (int i=0; i<_nombreServos; i++) {
	  pinMode (this->_brochesServos[i],OUTPUT); // Broches des servomoteurs  configurées en sortie
	} // fin for

	home(); // positionnement initial des servomoteurs

} // fin init

void UtilsServo::vitesseServos(int delaiVitesse){

	this->_vitesseServos=delaiVitesse; // en ms

} // fin vitesseServo

//---- désactivation debug
void UtilsServo::debug(boolean debugIn){

	this->_debug=debugIn; 

} // fin debug

///////////// fonctions générales communes  //////////////

//------- fonction de conversion angle en impulsion - mieux avec float -----
float  UtilsServo::angle(float valeur_angle) { 

	float impuls=0;
	impuls=map(valeur_angle,this->_ANGLE_MIN,this->_ANGLE_MAX,this->_POS_MIN, this->_POS_MAX);
	return impuls;   

} // fin fonction impulsion servomoteur


////////// fonctions de positionnement des servomoteurs ///////////////

//--------- positionnement initial ---- 
void UtilsServo::home () {

	if(_debug) Serial.println("Angles initiaux :"); 

	//---- positionnement initial des servomoteurs --- 
	for (int i=0; i<_nombreServos; i++) {
	  this->_servos[i].writeMicroseconds(angle(this->_anglesServos[i][2])); // crée impulsion à partir valeur angle - plus précis que write()

	  if(_debug) Serial.println(this->_anglesServos[i][2]); 

	//--- mise à jour valeur des autres angles ---
	this->_anglesServos[i][0]= this->_anglesServos[i][2]; 
	this->_anglesServos[i][1]= this->_anglesServos[i][2]; 

	} // fin for

	delay(500); // Laisse le temps de se positionner... 

} // fin home 

//--------- positionnement initial ---- 
void UtilsServo::toHome () {

	float toS[this->_nombreServos]; // tableau local 

	if(_debug) Serial.println("Angles initiaux :"); 

	for (int i=0; i<_nombreServos; i++) {

	if(_debug) Serial.println(this->_anglesServos[i][2]); 
	
	toS[i]=this->_anglesServos[i][2]; // récupère la colonne dans le tableau 1 dimension
	if(_debug) Serial.println(toS[i]); 

	} // fin for

	
	//---- positionnement progressif synchronisés des servomoteurs à leur position initiale --- 

	servosSync(toS,this->_nombreServos); // passe le tableau des angles initiaux à la fonction servosSync

	for (int i=0; i<_nombreServos; i++) {

	  //servoTo(i, this->_anglesServos[i][2]); 

	  if(_debug) Serial.println(this->_anglesServos[i][2]); 

	//--- mise à jour valeur des autres angles ---
	this->_anglesServos[i][0]= this->_anglesServos[i][2]; 
	this->_anglesServos[i][1]= this->_anglesServos[i][2]; 

	} // fin for



} // fin home 

//------ fonction de positionnement native (=immédiat) de la lib Servo -----

void UtilsServo::servo(int indexServo,float toAngle) {

    this->_servos[indexServo].writeMicroseconds(angle(toAngle)); // crée impulsion à partir valeur angle - plus précis que write()

    this->_anglesServos[indexServo][0]=toAngle; // mémorise angle actuel 
   if (_debug) Serial.print(F("angle actuel Servo [")),Serial.print(indexServo),Serial.print(F("] = ")), Serial.println(this->_anglesServos[indexServo][0]);

} // fin servo 


//------ fonction de positionnement progressif du servomoteur par pas fixe - angle absolu ----- 

 void UtilsServo::servoTo( Servo toServo, float fromAngle, float toAngle, int toVitesse, int toPas) {

       //--- positionnement progressif par pas fixe de 1 degré --- 
    
    int delta=toAngle-fromAngle; // variation d'angle 
    
   // Serial.print("delta = "), Serial.println(delta); 
    
    if (delta>=0) { // si variation positive
      
      for (int i=0; i<delta; i++) { // defile n positions pour atteindre angle final dans sens positif
    
        fromAngle=fromAngle+1; // ajoute cran
        toServo.writeMicroseconds(angle(fromAngle)); // crée impulsion à partir valeur angle - plus précis que write()
        if (_debug) Serial.print(F("angle courant servo = ")), Serial.println(fromAngle); 
        delay(this->_vitesseServos); // pause entre chaque positionnement

      } // fin for 

    } // fin if 
    
    else { // si variation négative

      for (int i=-delta; i>0; i--) { // defile n positions pour atteindre angle final dans sens négatif
    
        fromAngle=fromAngle-1; // ajoute cran
        toServo.writeMicroseconds(angle(fromAngle)); // crée impulsion à partir valeur angle - plus précis que write()
        if (_debug) Serial.print(F("angle courant servo = ")), Serial.println(fromAngle); 
        delay(this->_vitesseServos); // pause entre chaque positionnement

      } // fin for 
    
    } // fin else 
    

 } // fin servoTo

//--- servoTo - forme simplifiée 
void UtilsServo::servoTo( int indexServo, float toAngle){

	servoTo( this->_servos[indexServo], this->_anglesServos[indexServo][0], toAngle, this->_vitesseServos, this->_pasServos); // utilise vitesse et pas courant 

    this->_anglesServos[indexServo][0]=toAngle; // mémorise angle actuel 
   if (_debug) Serial.print(F("angle actuel Servo [")),Serial.print(indexServo),Serial.print(F("] = ")), Serial.println(this->_anglesServos[indexServo][0]);

}


//--- fonction de positionnement progressif du servomoteur par pas fixe - angle relatif à la position courante  ----- 

 void UtilsServo::servoToR( Servo toServo, float fromAngle, float toAngle, int toVitesse, int toPas) {

       //--- positionnement progressif par pas fixe de 1 degré --- 
    
    int delta=toAngle; // variation d'angle correspond à l'angle transmis
    
    //Serial.print("delta = "), Serial.println(delta); 
    
    if (delta>=0) { // si variation positive
      
      for (int i=0; i<delta; i++) { // defile n positions pour atteindre angle final dans sens positif
    
        fromAngle=fromAngle+1; // ajoute cran
        toServo.writeMicroseconds(angle(fromAngle)); // crée impulsion à partir valeur angle - plus précis que write()
        if (_debug)Serial.print(F("angle courant servo = ")), Serial.println(fromAngle); 
        delay(this->_vitesseServos); // pause entre chaque positionnement

      } // fin for 

    } // fin if 
    
    else { // si variation négative

      for (int i=-delta; i>0; i--) { // defile n positions pour atteindre angle final dans sens négatif
    
        fromAngle=fromAngle-1; // ajoute cran
        toServo.writeMicroseconds(angle(fromAngle)); // crée impulsion à partir valeur angle - plus précis que write()
        if (_debug) Serial.print(F("angle courant servo = ")), Serial.println(fromAngle); 
        delay(this->_vitesseServos); // pause entre chaque positionnement

      } // fin for 
    
    } // fin else 

    
 } // fin servoToR

//--- servoToR - forme simplifiée 
void UtilsServo::servoToR( int indexServo, float toAngle){

	servoToR( this->_servos[indexServo], this->_anglesServos[indexServo][0], toAngle, this->_vitesseServos, this->_pasServos); // utilise vitesse et pas courant 

    this->_anglesServos[indexServo][0]=this->_anglesServos[indexServo][0]+toAngle; // mémorise angle actuel 
   if (_debug) Serial.print(F("angle0 Servo [")),Serial.print(indexServo),Serial.print(F("] = ")), Serial.println(this->_anglesServos[indexServo][0]);

} // fin servoToR



//---------- servosSync() = fonction de positionnement synchronisé des servomoteurs du robot en position absolue - 12 valeurs ------ 
void UtilsServo::servosSync( float S0, float S1, float S2, float S3, float S4 , float S5, float S6, float S7, float S8, float S9, float S10, float S11) { // reçoit les angles absolus des servos

	float S[12]; 
	
	S[0]=S0, S[1]=S1,  S[2]=S2, S[3]=S3, S[4]=S4, S[5]=S5, S[6]=S6, S[7]=S7, S[8]=S8, S[9]=S9, S[10]=S10, S[11]=S11; 
	
	servosSync( S , 12) ; 
	
} // fin servosSync (12 valeurs)

void UtilsServo::servosSync( float S0, float S1, float S2, float S3, float S4) { // reçoit les angles absolus des servos

	float S[5]; 
	
	S[0]=S0, S[1]=S1,  S[2]=S2, S[3]=S3, S[4]=S4; 
	
	servosSync( S , 5) ; 
	
} // fin servosSync (5 valeurs)

//---------- servosSync() = fonction de positionnement synchronisé des servomoteurs du robot en position absolue - 5 valeurs ------ 
void UtilsServo::servosSync( float S0, float S1) { // reçoit les angles absolus des servos

	float S[2]; 
	
	S[0]=S0, S[1]=S1; 
	
	servosSync( S , 2) ; 
	
} // fin servosSync (2 valeurs)

//---------- servosSync() = fonction de positionnement synchronisé des servomoteurs du robot en position absolue - forme principale avec tableau ------ 
void UtilsServo::servosSync( float S[], int tailleIn) { // reçoit les angles absolus des servos

  //------------- tableaux de valeurs utilisés par la fonction --- 
  //float S[5]={S1,S2,S3,S4,S5}; // tableau de valeurs utilisant les paramètres reçus par la fonction
  
  float deltaS[tailleIn]; // tableau de valeurs pour la différence entre l'angle courant et l'angle cible (deltas)
  float absDeltaS[tailleIn]; // tableau de valeurs pour valeur absolues des deltas

  float cranS[tailleIn];   // tableau de valeurs pour calcul des crans d'incrémentation pour chaque servomoteur
  
  //---- variables utilisées par la fonction 
  float deltaMax=0;     //------ la plus grande valeur absolue des deltas

  // affichage angles reçus :

	if(_debug) Serial.println("Angles recus :"); 

	for (int i=0; i<tailleIn; i++) {

	if(_debug) Serial.println(S[i]); 

	} // fin for

  //--------- calcul de la variation d'angle pour chaque servomoteur
  
  for (int i=0; i<tailleIn; i++){ // défile les servomoteurs
    
    deltaS[i]=S[i]-this->_anglesServos[i][0]; // -- calcule la différence entre l'angle courant et l'angle cible du servo i 
    
    absDeltaS[i]=abs(deltaS[i]); // calcul de la valeur absolue du delta du servo i  
   //--- calculé ici pour éviter d'utiliser fonctions dans fonction max() - cf Référence
   
   //------ calcul du deltaMax = la plus grande valeur absolue des delta  
  deltaMax=max(deltaMax, absDeltaS[i]); // après tous les passages, la valeur la plus grande est conservée
  
  }

  if(_debug) Serial.print(F("deltaMax = ")), Serial.println(deltaMax); // affiche deltaMax

  // ---------- calcul des crans d'incrémentation pour chaque servomoteur ---- 
  //--- utilise delta avec signe +/- --- 
  
    for (int i=0; i<tailleIn; i++){ // défile servo
    
    cranS[i]=deltaS[i]/deltaMax; // divise delta / nombre de pas à effectuer par le servomoteur i
    
    if(_debug)Serial.print(F("cranS[")),  Serial.print(i), Serial.print(F("] = ")), Serial.println(cranS[i]); 
    
    //-------- réinitialise variable angle courant des servomoteurs ----
    //- évite de modifier angle0Servo lors des calculs
    this->_anglesServos[i][1]=this->_anglesServos[i][0]; 
    
  } // fin for i nbServos


  
  //----------- défile les deltaMax positions et positionne les servomoteurs --------------
  for (int j=0; j<deltaMax; j++) { // parcourt les deltaMax crans 
    
    for (int i=0; i<tailleIn; i++){ // défile les n servomoteurs 

        //---------- servomoteur i
       _anglesServos[i][1]=_anglesServos[i][1]+cranS[i]; // ajoute cran
        _servos[i].writeMicroseconds(angle(_anglesServos[i][1])); // crée impulsion à partir valeur angle - plus précis que write()
        if(_debug)Serial.print(F(" / S")),Serial.print(i), Serial.print(F(" = ")), Serial.print(_anglesServos[i][1]); 
  

    } // fin for i nbServos

        //-------------- pause vitesse entre 2 positionnement des servomoteurs pour mouvement progressif
        if(_debug)Serial.println(); 
        delay(this->_vitesseServos); // pause après positionnement de tous les servomoteurs
    
  } // fin for j deltaMax
  

    //------------- mise à jour variable des angles courants ------------------  
     //--- en se basant sur valeur angle de départ et delta ---
    //--- le résultat doit correspondre à celui obtenu par calculs précédents 

    for (int i=0; i<tailleIn; i++){ // défile les n servomoteurs 
     
         _anglesServos[i][0]=S[i]; // nouvel angle du servomoteur i
         if(_debug)Serial.print(F(" / S")),Serial.print(i), Serial.print(F("0 = ")), Serial.print(_anglesServos[i][0]); 


    } // fin for i nbServos 
    
    if(_debug) Serial.println(); 


 
} // fin servosRobotSyncIndice - fonction de positionnement synchronisé - angles en valeur absolue 

//--------- fin ServosSync


//---------- servosSync() = fonction de positionnement synchronisé des servomoteurs du robot en position absolue - 12 valeurs ------ 
void UtilsServo::servosSyncR( float S0, float S1, float S2, float S3, float S4 , float S5, float S6, float S7, float S8, float S9, float S10, float S11) { // reçoit les angles absolus des servos

	float S[12]; 
	
	S[0]=S0, S[1]=S1,  S[2]=S2, S[3]=S3, S[4]=S4, S[5]=S5, S[6]=S6, S[7]=S7, S[8]=S8, S[9]=S9, S[10]=S10, S[11]=S11; 
	
	servosSyncR( S , 12) ; 
	
} // fin servosSync (12 valeurs)

//---------- servosSyncR() = fonction de positionnement synchronisé des servomoteurs du robot en position relative - 5 valeurs ------ 
void UtilsServo::servosSyncR( float S0, float S1, float S2, float S3, float S4) { // reçoit les angles absolus des servos

	float S[5]; 
	
	S[0]=S0, S[1]=S1,  S[2]=S2, S[3]=S3, S[4]=S4; 
	
	servosSyncR( S , 5) ; 
	
} // fin servosSync (5 valeurs)

//---------- servosSyncR() = fonction de positionnement synchronisé des servomoteurs du robot en position relative - 5 valeurs ------ 
void UtilsServo::servosSyncR( float S0, float S1) { // reçoit les angles absolus des servos

	float S[2]; 
	
	S[0]=S0, S[1]=S1; 
	
	servosSyncR( S , 2) ; 
	
} // fin servosSyncR (2 valeurs)

//------------ fonction de positionnement synchronisé des servomoteurs du robot en position relative -  forme principale avec tableau  ----- 
void UtilsServo::servosSyncR(float S[], int tailleIn) { // reçoit les angles relatifs des servos
//------------ le nombre d'angle reçus par la fonction doit correspondre aux nombres de servomoteurs - constante nbServos 

  //------------- tableaux de valeurs utilisés par la fonction --- 
  //float S[nbServos]={S1,S2,S3,S4,S5}; // tableau de valeurs utilisant les paramètres reçus par la fonction
  
  float deltaS[tailleIn]; // tableau de valeurs pour la différence entre l'angle courant et l'angle cible (deltas)
  float absDeltaS[tailleIn]; // tableau de valeurs pour valeur absolues des deltas

  float cranS[tailleIn];   // tableau de valeurs pour calcul des crans d'incrémentation pour chaque servomoteur
  
  //---- variables utilisées par la fonction 
  float deltaMax=0;     //------ la plus grande valeur absolue des deltas

  //--------- calcul de la variation d'angle pour chaque servomoteur
  
  for (int i=0; i<tailleIn; i++){
    
    deltaS[i]=S[i]; // -- le delta est l'angle relatif envoyé à la fonction du servo i 

    absDeltaS[i]=abs(deltaS[i]); // calcul de la valeur absolue du delta du servo i  
   //--- calculé ici pour éviter d'utiliser fonctions dans fonction max() - cf Référence
   
   //------ calcul du deltaMax = la plus grande valeur absolue des delta  
  deltaMax=max(deltaMax, absDeltaS[i]); // après tous les passages, la valeur la plus grande est conservée
  
  }


  if(_debug) Serial.print(F("deltaMax = ")), Serial.println(deltaMax); // affiche deltaMax

  // ---------- calcul des crans d'incrémentation pour chaque servomoteur ---- 
  //--- utilise delta avec signe +/- --- 
  
    for (int i=0; i<tailleIn; i++){
    
    cranS[i]=deltaS[i]/deltaMax; // divise delta / nombre de pas à effectuer par le servomoteur i
    
    if(_debug) Serial.print(F("cranS[")),  Serial.print(i), Serial.print(F("] = ")), Serial.println(cranS[i]); 
    
    //-------- réinitialise variable angle courant des servomoteurs ----
    //- évite de modifier angle0Servo lors des calculs
    _anglesServos[i][1]=_anglesServos[i][0]; 
    
  } // fin for i nbServos

  
  //----------- défile les deltaMax positions et positionne les servomoteurs --------------
  for (int j=0; j<deltaMax; j++) { // parcourt les deltaMax crans 
    
    for (int i=0; i<tailleIn; i++){ // défile les n servomoteurs 

        //---------- servomoteur i
        _anglesServos[i][1]=_anglesServos[i][1]+cranS[i]; // ajoute cran
        _servos[i].writeMicroseconds(angle(_anglesServos[i][1])); // crée impulsion à partir valeur angle - plus précis que write()
        if(_debug) Serial.print(F(" / S")),Serial.print(i), Serial.print(F(" = ")), Serial.print(_anglesServos[i][1]); 
       
    

    } // fin for i nbServos

        //-------------- pause vitesse entre 2 positionnement des servomoteurs pour mouvement progressif
        if(_debug) Serial.println(); 
        delay(_vitesseServos); // pause après positionnement de tous les servomoteurs
    
  } // fin for j deltaMax
  

    //------------- mise à jour variable des angles courants ------------------  
     //--- en se basant sur valeur angle de départ et delta ---
    //--- le résultat doit correspondre à celui obtenu par calculs précédents 

    for (int i=0; i<tailleIn; i++){ // défile les n servomoteurs 
     
         _anglesServos[i][0]=_anglesServos[i][0]+S[i]; // S[i]
         if(_debug) Serial.print(F(" / S")),Serial.print(i), Serial.print(F("0 = ")), Serial.print(_anglesServos[i][0]); 


    } // fin for i nbServos 
    
    if(_debug) Serial.println(); 

 
} // fin servosSyncR - fonction de positionnement synchronisé relatif


//------------ analyseChaine ---------------
void UtilsServo::analyseChaine(String chaineReceptionIn) { // fonction d'analyse de la chaine recue

 String paramString=""; // chaine locale - initialiser à "" !
 long params[12]; // tableau de paramètres pour instructions à paramètres multiples - taille du tableau fixée par le nombre max de paramètres utilisés 

  // ---- analyse de la chaine recue sur le port Série ---- 
  chaineReceptionIn.trim(); // enlève les espaces - version 1.0

 //xxxxxxxxxxxxxxxxxxxx instructions sans paramètres  xxxxxxxxxxxxxxx

//================ instructions home =========
  
  //-------------- test instruction home() - retour brut à la position home ---------- 
  if (chaineReceptionIn=="home()") { // si instruction sans paramètre reçue 
    home(); // exécute instruction si valide
  } 
  //-------------- test instruction toHome() - retour synchronisé à la position home----------- 
  else if (chaineReceptionIn=="toHome()") { // si instruction sans paramètre reçue 
    toHome(); // exécute instruction si valide
  } 

 //xxxxxxxxxxxxxxxxxxxxx instructions avec parametres chaines xxxxxxxxxxxxxxxxxxx
  
    //paramString=_utils.testInstructionString("debug(", chaineReceptionIn); // récupère la chaine  paramètre ou null 
    paramString=_utils.testInstructionString( chaineReceptionIn, "debug("); // récupère la chaine  paramètre ou null 
    if (paramString) { // si instruction avec chaine reçue !=0 équivaut !=null   (ne pas utiliser ==true)

      if (_debug) Serial.print(F("Chaine recue : ")); 
      if (_debug) Serial.println(paramString); // affiche la chaine extraite
 
      if (paramString=="true"){
        
        _debug=true; // active debug

         
      } // fin if true 

      if (paramString=="false"){
        _debug=false; // désactive debug
        
        
      } // fin if false 
      
  } // if paramString==true 

 
  //xxxxxxxxxxxxxxxxxxxx instructions avec paramètres numeriques entiers xxxxxxxxxxxxxxx

     // info : la valeur numérique extraite par testInstruction() est stockée dans le tableau de variables globales param
     
 //xxxxxxxxxx Utiliser else if pour éviter doubles prises en compte xxxxxxxxxxxxxxx

  //================= instructions paramètres généraux =============
 
  //-------------- test instruction vitesseServos(xxx) ----------- 
   else if (_utils.testInstruction2(chaineReceptionIn,"vitesseServos(",1,params)==true) { //si instruction avec 1 paramètre reçue valide 
 
     Serial.println("La condition est true"); // debug 
   
    this->_vitesseServos=params[0]; // change valeur vitesse (= durée delay en ms)
    if(_debug) Serial.print(F("vitesse Servos = ")), Serial.println(_vitesseServos); 
    
  } // fin test vitesseServos(xxx)

  //================ instructions servo Pan  =========
  
  //-------------- test instruction servoPan(xxx) ----------- 
  //if (testInstruction2("servoPan(",1)==true) { // si instruction avec 1 paramètre reçue valide
  else if (_utils.testInstruction2(chaineReceptionIn,"servoPan(",1,params)==true) { //si instruction avec 1 paramètre reçue valide

    servo(_indexServoPan, params[0]); // positionne le servo à l'angle voulu - mouvement brut
	//servo(0, params[0]); // positionne le servo à l'angle voulu - mouvement brut      

  } // fin test servoPan()

  //-------------- test instruction servoPanTo(xxx) ----------- 

  else if (_utils.testInstruction2(chaineReceptionIn,"servoPanTo(",1,params)==true) { //si instruction avec 1 paramètre reçue valide

    servoTo(_indexServoPan, params[0]); // positionne progressivement le servo à l'angle absolu souhaité
	//servoTo(0, params[0]); // positionne progressivement le servo à l'angle absolu souhaité

  } // fin test servoPanTo()

  //-------------- test instruction servoPanToR(xxx) ----------- 

  else if (_utils.testInstruction2(chaineReceptionIn,"servoPanToR(",1,params)==true) { //si instruction avec 1 paramètre reçue valide

    servoToR(_indexServoPan, params[0]); // positionne progressivement le servo à l'angle absolu souhaité
	//servoToR(0, params[0]); // positionne progressivement le servo à l'angle absolu souhaité

  } // fin test servoPanToR()

  //================ instructions servo Tilt  =========
  
  //-------------- test instruction servoTilt(xxx) ----------- 
  //if (testInstruction2("servoPan(",1)==true) { // si instruction avec 1 paramètre reçue valide
  else if (_utils.testInstruction2(chaineReceptionIn,"servoTilt(",1,params)==true) { //si instruction avec 1 paramètre reçue valide


    servo(_indexServoTilt, params[0]); // positionne le servo à l'angle voulu - mouvement brut   
    //servo(1, params[0]); // positionne le servo à l'angle voulu - mouvement brut   

  } // fin test servoTilt()

  //-------------- test instruction servoTiltTo(xxx) ----------- 

  else if (_utils.testInstruction2(chaineReceptionIn,"servoTiltTo(",1,params)==true) { //si instruction avec 1 paramètre reçue valide

    servoTo(_indexServoTilt, params[0]); // positionne progressivement le servo à l'angle absolu souhaité
	//servoTo(1, params[0]); // positionne progressivement le servo à l'angle absolu souhaité

  } // fin test servoTiltTo()

  //-------------- test instruction servoTiltToR(xxx) ----------- 

  else if (_utils.testInstruction2(chaineReceptionIn,"servoTiltToR(",1,params)==true) { //si instruction avec 1 paramètre reçue valide

    servoToR(_indexServoTilt, params[0]); // positionne progressivement le servo à l'angle absolu souhaité
	//servoToR(1, params[0]); // positionne progressivement le servo à l'angle absolu souhaité
  } // fin test servoTiltToR()

 //================ instructions servo Index  =========
  
  //-------------- test instruction servo(iii,xxx) ----------- 
  else if (_utils.testInstruction2(chaineReceptionIn,"servo(",2,params)==true) { //si instruction avec 2 paramètres reçus valides

    servo(params[0],params[1]); // positionne le servo à l'angle voulu - mouvement brut   

  } // fin test servo()

  //-------------- test instruction servoTo(iii,xxx) ----------- 

  else if (_utils.testInstruction2(chaineReceptionIn,"servoTo(",2,params)==true) { //si instruction avec 1 paramètre reçue valide

    servoTo(params[0],params[1]); // positionne progressivement le servo à l'angle absolu souhaité

  } // fin test servoTo()

  //-------------- test instruction servoToR(iii,xxx) ----------- 

  else if (_utils.testInstruction2(chaineReceptionIn,"servoToR(",2,params)==true) { //si instruction avec 1 paramètre reçue valide

    servoToR(params[0],params[1]); // positionne progressivement le servo à l'angle absolu souhaité

  } // fin test servoToR()

//================ instructions servosSync  =========
  
  //-------------- test instruction servosSync(xxx,xxx, xxx, xxx, xxx) ----------- 
  else if (_utils.testInstruction2(chaineReceptionIn,"servosSync(",5,params)==true) { //si instruction avec 2 paramètres reçus valides

   servosSync(params[0],params[1],params[2],params[3],params[4]); // mouvement synchronisé
    //servosSync((float*)params,5); // mouvement synchronisé à partir tableau  - pb avec le cast du tableau long en float

  } // fin test servoSync()

  //-------------- test instruction servosSync(xxx,xxx) ----------- 
  else if (_utils.testInstruction2(chaineReceptionIn,"servosSync(",2,params)==true) { //si instruction avec 2 paramètres reçus valides

    servosSync(params[0],params[1]); // mouvement synchronisé
    // servosSync((float*)params,2); // mouvement synchronisé à partir tableau  - pb avec le cast du tableau long en float

  } // fin test servoSync()



//================ instructions servosSyncR  =========

  //-------------- test instruction servosSyncR(xxx,xxx, xxx, xxx, xxx) ----------- 
  else if (_utils.testInstruction2(chaineReceptionIn,"servosSyncR(",5,params)==true) { //si instruction avec 2 paramètres reçus valides

   servosSyncR(params[0],params[1],params[2],params[3],params[4]); // mouvement synchronisé

  } // fin test servoSync()
  
  //-------------- test instruction servosSyncR(xxx,xxx) ----------- 
  else if (_utils.testInstruction2(chaineReceptionIn,"servosSyncR(",2,params)==true) { //si instruction avec 2 paramètres reçus valides

    servosSyncR(params[0],params[1]); // mouvement synchronisé

  } // fin test servoSync()




} // fin analyse chaine 
