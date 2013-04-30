/*
 * Copyright (c) 2012 by Xavier HINAULT - support@mon-club-elec.fr
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

#ifndef UtilsServo_h
#define UtilsServo_h

#include "Arduino.h"
#include "Servo.h"
#include "Utils.h"


class UtilsServo {

  private : // variables internes

  int _vitesseServos; // variable utilisée pour délai entre pour vitesse servomoteur en ms  
	int _pasServos; // variable utilisée pour cran angle entre 2 positions intermédiaires (degrés)

	int _ANGLE_MIN; // angle position MIN en degrés
	int _POS_MIN; // largeur impulsion pour position ANGLE_MIN degrés du servomoteur

	int _ANGLE_MAX; // angle position MAX en degrés
	int _POS_MAX; // largeur impulsion pour position ANGLE_MAX degrés du servomoteur

	boolean _debug; // flag de message de debug  

	int _nombreServos; // variable pour mémoriser le nombre de servomoteurs

	//-- todo : pb de portée avec ces variables
	int _indexServoPan; // index utilisé pour servo Pan
	int _indexServoTilt; // index utilisé pour servo Tilt

	int _brochesServos[12]; // tableau des broches des servomoteurs 

	float _anglesServos[12][3]; // tableau interne des angles des servomoteurs 

	//------ pour une raison mytérieuse... mettre les déclas de int etc... avant 

	Utils _utils; // objet utils donnant accès aux fonctions de la lib

  public :

	Servo _servos[12]; // tableau interne des objets servomoteurs 

  
  public :
  	UtilsServo(); // constructeur
  	//UtilsServo(Servo *servos, float (*anglesServosIn)[3], int nombreServosIn); // constructeur
	UtilsServo(int nombreServosIn); // constructeur

	void anglesServo(int index, float angle0, float angle1, float angle2); // initialise angles du servomoteur 

	void init( int brochesServosIn[]); // initialise les servomoteurs

	float  angle(float valeur_angle); 
	void vitesseServos(int delaiVitesse); // fixe delai entre 2 pas, en ms
	void debug(boolean debugIn); // activation / désactivation debug

	void home ();
	void toHome(); 

	void servo(int indexServo, float toAngle);

	void servoTo( Servo toServo, float fromAngle, float toAngle, int toVitesse, int toPas);
	void servoTo( int indexServo, float toAngle);

	void servoToR( Servo toServo, float fromAngle, float toAngle, int toVitesse, int toPas);
	void servoToR( int indexServo, float toAngle);

	void servosSync( float S[], int tailleIn); 
	void servosSync( float S0, float S1);
	void servosSync( float S1, float S2, float S3, float S4, float S5) ;
	void servosSync( float S1, float S2, float S3, float S4, float S5, float S6, float S7, float S8, float S9, float S10, float S11, float S12) ;

	void servosSyncR(float S[], int tailleIn); 
	void servosSyncR( float S0, float S1);
	void servosSyncR( float S0, float S1, float S2, float S3, float S4);
	void servosSyncR( float S1, float S2, float S3, float S4, float S5, float S6, float S7, float S8, float S9, float S10, float S11, float S12);

	void analyseChaine(String chaineReceptionIn); 


  private : // fonctions internes

};


#endif
