use strict;
use bytes;

=comment

Translate.pl attempts to load all of the translations into one file. A
translation is simply a hash from English to the other language.

=cut





$::English = { 'LanguageCode' => 0,

# In-game text

	'MAX COMBO!!!'		=> undef,
	'%d-hit combo!'		=> undef,		# e.g. 3-hit combo!
	'%d%% damage'		=> undef,		# e.g. 30% damage
	'Round %d'			=> undef,
	'REW'				=> undef,
	'REPLAY'			=> undef,
	'DEMO'				=> undef,
	'Press F1 to skip...'=> undef,
	'HURRY UP!'			=> undef,
	'TIME IS UP!'		=> undef,

	'Final Judgement'	=> undef,
	'Continue?'			=> undef,
	'SPLAT!'			=> undef,
	'Choose A Fighter Dammit' => undef,

# Menu items

	"Main Menu"			=> undef,
	"~SINGLE PLAYER GAME"=> undef,
	"~MULTI PLAYER GAME"=> undef,
	"~SURRENDER GAME"	=> undef,
	"~OPTIONS"			=> undef,
	"~INFO"				=> undef,
	"QUIT"				=> undef,
	"~OK"				=> undef,
	"~LANGUAGE: "		=> undef,

	"Options"			=> undef,
	"~FULLSCREEN ON"	=> undef,
	"~FULLSCREEN OFF"	=> undef,
	"GAME SPEED: "		=> undef,
	"GAME TIME: "		=> undef,
	"STAMINA: "			=> undef,
	"~SOUND"			=> undef,
	"~RIGHT PLAYER KEYS"=> undef,
	"~LEFT PLAYER KEYS"	=> undef,
	
	"Sound"				=> undef,
	"CHANNELS: "		=> undef,	# Mono / Stereo
	"SOUND QUALITY: "	=> undef,	# KHz of playback rate
	"SOUND FIDELITY: "	=> undef,	# 8 bit or 16 bit
	"MUSIC VOLUME: "	=> undef,	# OFF or numeric
	"EFFECTS VOLUME: "	=> undef,	# OFF or numeric
	

# Menu options

	"BABY"				=> undef,
	"VERY LOW"			=> undef,
	"LOW"				=> undef,
	"NORMAL"			=> undef,
	"HIGH"				=> undef,
	"VERY HIGH"			=> undef,
	"NEAR IMMORTAL"		=> undef,

	"SNAIL RACE"		=> undef,
	"SLOW"				=> undef,
	"NORMAL"			=> undef,
	"TURBO"				=> undef,
	"KUNG-FU MOVIE"		=> undef,

	# Sound / Channels
	"MONO"				=> undef,
	"STEREO"			=> undef,

	# Sound / Mixing rate settings
	"LOW"				=> undef,
	"MEDIUM"			=> undef,
	"HIGH"				=> undef,
	
	# Sound volume
	"OFF"				=> undef,
	
# Key configuration

	'%s player-\'%s\'?'	=> undef,		# The first %s becomes Left or Right. The second %s is up/down/high punch/...
	'Left'				=> undef,
	'Right'				=> undef,
	"up",				=> undef,
	"down",				=> undef,
	"left",				=> undef,
	"right",			=> undef,
	"block",			=> undef,
	"low punch",		=> undef,
	"high punch",		=> undef,
	"low kick",			=> undef,
	"high kick"			=> undef,
	'Thanks!'			=> undef,
	
# Demo screens,

	'Fighter Stats'		=> undef,
	'Unfortunately this fighter is not yet playable.' => undef,
	'Name: '			=> undef,
	'Team: '			=> undef,
	'Style: '			=> undef,
	'Age: '				=> undef,
	'Weight: '			=> undef,
	'Height: '			=> undef,
	'Shoe size: '		=> undef,
	
	"Credits"			=> undef,
	"CreditsText1"		=>
"OPENMORTAL CREDITS


-- THE OPENMORTAL TEAM ARE --


CODING - UPi
MUSIC - Purple Motion
MUSIC - XTD / Mystic
GRAPHICS - UPi
German translation - ??
French translation - Vampyre
Spanish translation - EdsipeR
Portuguese translation - Vinicius Fortuna

-- CAST --

Boxer - Zoli
Cumi - As himself
Descant - As himself
Fureszes Orult - Ambrus
Grizli - As himself
Kinga - As herself
Macy - As herself
Misi - As himself
Rising-san - Surba
Sirpi - As himself
Taka Ito - Bence
Tokeletlen Katona - Dani
Watasiwa Baka Janajo - Ulmar
Black Dark Evil Mage - UPi

-- HOSTING --

sourceforge.net
apocalypse.rulez.org
freshmeat.net

OpenMortal is Copyright 2003 of the OpenMortal Team
Distributed under the GNU General Public Licence Version 2\n\n",

	'CreditsText2'		=>
'Thanks to Midway for not harrassing us with legal stuff so far, even though '.
'we must surely violate at least 50 of their patents, international copyrights and registered trademarks.


OpenMortal needs your help! If you can contribute music, graphics, improved code, '.
'additional characters, cash, beer, pizza or any other consumable, please mail us '.
'at upi@apocalypse.rulez.org! The same address is currently accepting comments and '.
"fanmail too (hint, hint!).\n\n",

	'CreditsText3'		=>
"Be sure to check out other stuff from
Apocalypse Production
and
Degec Entertainment\n\n",

	'Story1Text'		=>
"We, the Gods of the Evil Killer Black Antipathic Dim (witted) Fire Mages no longer tolerate the lack of evildoing.

We send them out on a mission so diabolical, so evil that the world will never be the same again!

We order our unworthy followers to
DESTROY THE SATURDAY
and plunge humanity into a dark age of 5 working days and 1 holiday per week... FOREVER!\n\n\n\n\n\n\n\n\n",

	'Story2Text',		=>
"Whenever EVIL looms on the horizon, the good guys are there to save the day. Son Goku, the protector of Earth and Humanity went to the rescue...

Only to become ROADKILL on his way to the Mortal Szombat tournament! It was Cumi's first time behind the wheel, after all...\n\n\n\n\n\n\n\n\n",
};









$::Hungarian = {  'LanguageCode' => 3,

# In-game text

	'MAX COMBO!!!'		=> "MAX KOMB�!!!",
	'%d-hit combo!'		=> '%dX komb�',
	'%d%% damage'		=> '%d%% sebz�s',
	'Round %d'			=> '%d. menet',
	'REW'				=> 'VISSZA',
	'REPLAY'			=> undef,
	'DEMO'				=> 'DEM�',
	'Press F1 to skip...'=> 'F1 gomb: tov�bb',
	'HURRY UP!'			=> 'GYER�NK M�R!',
	'TIME IS UP!'		=> 'NA ENNYI!',

	'Final Judgement'	=> 'V�gs� �t�let',
	'Continue?'			=> 'Tov�bb?',
	'SPLAT!'			=> 'FR�CCS!',
	'Choose A Fighter Dammit' => 'V�lassz J�t�kost, Baszki',

# Menu items

	"Main Menu"			=> 'F�MEN�',
	"~SINGLE PLAYER GAME"=> '~EGYSZEM�LYES J�T�K',
	"~MULTI PLAYER GAME"=> '~K�TSZEM�LYES J�T�K',
	"~SURRENDER GAME"	=> '~J�T�K FELAD�SA',
	"~OPTIONS"			=> '~BE�LL�T�SOK',
	"~INFO"				=> '~INFORM�CI�K',
	"QUIT"				=> 'QUIT',
	"~OK"				=> '~OK�',
	"~LANGUAGE: "		=> '~NYELV: ',

	"Options"			=> 'Be�ll�t�sok',
	"~FULLSCREEN ON"	=> 'Teljes k�perny�',
	"~FULLSCREEN OFF"	=> 'Ablakos megjelen�t�s',
	"GAME SPEED: "		=> 'J�t�k sebess�g: ',
	"GAME TIME: "		=> 'J�t�kid�: ',
	"STAMINA: "			=> '�ll�k�pess�g: ',
	"~SOUND"			=> '~HANG',
	"~RIGHT PLAYER KEYS"=> '~Jobb j�t�kos gombjai',
	"~LEFT PLAYER KEYS"	=> '~Bal j�t�kos gombjai',

	"Sound"				=> 'Hangok',
	"CHANNELS: "		=> 'Csatorn�k: ',
	"SOUND QUALITY: "	=> 'Hangmin�s�g: ',
	"SOUND FIDELITY: "	=> 'Hangpontoss�g: ',
	"MUSIC VOLUME: "	=> 'Zene hangereje: ',
	"EFFECTS VOLUME: "	=> 'Zajok hangereje: ',


# Menu options

	"BABY"				=> 'CSECSEM�',
	"VERY LOW"			=> 'NAGYON GY�R',
	"LOW"				=> 'GY�R',
	"NORMAL"			=> 'NORM�LIS',
	"HIGH"				=> 'KEM�NY',
	"VERY HIGH"			=> 'NAGYON KEM�NY',
	"NEAR IMMORTAL"		=> 'TERMIN�TOR',

	"SNAIL RACE"		=> 'CSIGAVERSENY',
	"SLOW"				=> 'LASS�',
	"NORMAL"			=> 'NORM�L',
	"TURBO"				=> 'TURB�',
	"KUNG-FU MOVIE"		=> 'KUNG-FU FILM',

	"MONO"				=> 'MON�',
	"STEREO"			=> 'SZTERE�',

	"LOW"				=> 'ALACSONY',
	"MEDIUM"			=> 'K�ZEPES',
	"HIGH"				=> 'MAGAS',

	"OFF"				=> 'NINCS',

# Key configuration

	'%s player-\'%s\'?'	=> '%s j�t�kos - \'%s\'?',
	'Left'				=> 'Bal',
	'Right'				=> 'Jobb',
	"up",				=> 'fel',
	"down",				=> 'le',
	"left",				=> 'balra',
	"right",			=> 'jobbra',
	"block",			=> 'v�d�s',
	"low punch",		=> 'als� �t�s',
	"high punch",		=> 'fels� �t�s',
	"low kick",			=> 'als� r�g�s',
	"high kick"			=> 'fels� r�g�s',
	'Thanks!'			=> 'K�szi!',

# Demo screens,

	'Fighter Stats'		=> 'Harcos Adatai',
	'Unfortunately this fighter is not yet playable.' => 'Sajnos � m�g nem j�tszhat�.',
	'KEYS'				=> 'GOMBOK',
	'Name: '			=> 'N�v: ',
	'Team: '			=> 'Csapat: ',
	'Style: '			=> 'St�lus: ',
	'Age: '				=> 'Kor: ',
	'Weight: '			=> 'S�ly: ',
	'Height: '			=> 'Magass�g: ',
	'Shoe size: '		=> 'Cip�m�ret: ',

	"Credits"			=> 'K�sz�tett�k',
	"CreditsText1"		=>
"A Mort�l Szombat Elk�vet�i


-- A MORT�L SZOMBAT CSAPAT --


PROGRAM - UPi
ZENE - Purple Motion
ZENE - XTD / Mystic
GRAFIKA - UPi
N�met ford�t�s - ??
Francia ford�t�s - Vampyre
Spanyol ford�t�s - EdispeR

-- SZEREPL�K --

Boxer - Zoli
Cumi - Mint �nmaga
Descant - Mint �nmaga
F�r�szes �r�lt - Ambrus
Grizli - Mint �nmaga
Kinga - Mint �nmaga
Macy - Mint �nmaga
Misi - Mint �nmaga
Rising-san - Surba
Sirpi - Mint �nmaga
Taka Ito - Bence
T�k�letlen Katona - Dani
Watasiwa Baka Janajo - Ulmar
Black Dark Evil Mage - UPi

-- SZERVEREINK --

sourceforge.net
apocalypse.rulez.org
freshmeat.net

A Mort�l Szombat Copyright 2003, A Mort�l Szombat Csapat
Distributed under the GNU General Public Licence Version 2\n\n",

	'CreditsText2'		=>
'Thanks to Midway for not harrassing us with legal stuff so far, even though '.
'we must surely violate at least 50 of their patents, international copyrights and registered trademarks.


OpenMortal needs your help! If you can contribute music, graphics, improved code, '.
'additional characters, cash, beer, pizza or any other consumable, please mail us '.
'at upi@apocalypse.rulez.org! The same address is currently accepting comments and '.
"fanmail too (hint, hint!).\n\n",

	'CreditsText3'		=>
"Be sure to check out other stuff from
Apocalypse Production
and
Degec Entertainment\n\n",

	'Story1Text'		=>
"Mi, a Gonosz Gyilkos Fekete Ellenszenves S�t�t(elm�j�) T�zm�gusok istenei nem t�rhetj�k tov�bb a gonosztev�s hi�ny�t.

Egy k�ldet�st adunk nekik amelyik olyan �rd�gien gonosz, amilyet a vil�g m�g sosem l�tott!

M�ltatlan alattval�ink parancsa:
ELPUSZT�TANI
A SZOMBATOT
hogy az emberis�g az 5 munkanap �s egy sz�nnapos h�t s�t�t kor�ba s�llyedjen... �R�KRE!\n\n\n\n\n\n\n\n\n",

	'Story2Text',		=>
"Whenever EVIL looms on the horizon, the good guys are there to save the day. Son Goku, the protector of Earth and Humanity went to the rescue...

Only to become ROADKILL on his way to the Mortal Szombat tournament! It was Cumi's first time behind the wheel, after all...\n\n\n\n\n\n\n\n\n",
};







$::French = { 'LanguageCode' => 2,

# In-game text

	'MAX COMBO!!!'		=> 'MAX COMBO!!!',
	'%d-hit combo!'		=> '%d-coups combo!',		# e.g. 3-hit combo!
	'%d%% damage'		=> '%d%% degats',		# e.g. 30% damage
	'Round %d'			=> 'Round %d',
	'REW'				=> 'REMB',
	'REPLAY'			=> 'RALENTI',
	'DEMO'				=> 'DEMO',
	'Press F1 to skip...'=> 'Appuyez sur F1 pour annuler',
	'HURRY UP!'			=> 'DEPECHEZ-VOUS !',
	'TIME IS UP!'		=> 'LE TEMPS EST ECOULE',

	'Final Judgement'	=> 'Jugement Final',
	'Continue?'			=> 'Continuer ?',
	'SPLAT!'			=> 'SPLAT',
	'Choose A Fighter Dammit' => 'Choisis un combattant Mildiou !',

# Menu items

	"Main Menu"			=> "Menu Principal",
	"~SINGLE PLAYER GAME"=> "JEU SOLO",
	"~MULTI PLAYER GAME"=> "JEU MULTI",
	"~SURRENDER GAME"	=> "Jeu JeMeRends",
	"~OPTIONS"			=> "OPTIONS",
	"~INFO"				=> "INFO",
	"QUIT"				=> "QUITTER",
	"~OK"				=> "OK",
	"~LANGUAGE: "		=> "LANGUE: ",

	"Options"			=> "Options",
	"~FULLSCREEN ON"	=> "Plein �cran ON",
	"~FULLSCREEN OFF"	=> "Plein �cran OFF",
	"GAME SPEED: "		=> "Vitesse de jeu: ",
	"GAME TIME: "		=> "Temps de jeu: ",
	"STAMINA: "			=> "Vitalit�: ",
	"~SOUND"			=> "SON",
	"~RIGHT PLAYER KEYS"=> "Touches joueur droite",
	"~LEFT PLAYER KEYS"	=> "Touches joueur gauche",
	
	"Sound"				=> 'Son',
	"CHANNELS: "		=> "CANAUX: ",	# Mono / Stereo
	"SOUND QUALITY: "	=> "Echantillonage: ",	# KHz of playback rate
	"SOUND FIDELITY: "	=> "Qualit� sonore: ",	# 8 bit or 16 bit
	"MUSIC VOLUME: "	=> "Volume musical: ",	# OFF or numeric
	"EFFECTS VOLUME: "	=> "Volume effets : ",	# OFF or numeric
	

# Menu options

	"BABY"				=> "BEBE",
	"VERY LOW"			=> "TRES BAS",
	"LOW"				=> "BAS",
	"NORMAL"			=> "NORMAL",
	"HIGH"				=> "ELEVE",
	"VERY HIGH"			=> "TRES ELEVE",
	"NEAR IMMORTAL"		=> "PRESQUE IMMORTEL",

	"SNAIL RACE"		=> "COURSE D'ESCARGOT",
	"SLOW"				=> "LENT",
	"NORMAL"			=> "NORMAL",
	"TURBO"				=> "TURBO",
	"KUNG-FU MOVIE"		=> "FILM DE KUNG-FU",

	# Sound / Channels
	"MONO"				=> "MONO",
	"STEREO"			=> "STEREO",

	# Sound / Mixing rate settings
	"LOW"				=> "BAS",
	"MEDIUM"			=> "MOYEN",
	"HIGH"				=> "HAUT",
	
	# Sound volume
	"OFF"				=> "OFF",
	
# Key configuration

	'%s player-\'%s\'?'	=> '%s joueur-\'%s\'?',		# The first %s becomes Left or Right. The second %s is up/down/high punch/...
	'Left'				=> 'Gauche',
	'Right'				=> 'Droite',
	"up",				=> "haut",
	"down",				=> "bas",
	"left",				=> "gauche",
	"right",			=> "droite",
	"block",			=> "bloquer",
	"low punch",		=> "poing bas",
	"high punch",		=> "poing haut",
	"low kick",			=> "Coup de pied bas",
	"high kick"			=> "Coup de pied haut",
	'Thanks!'			=> "Merci !",
	
# Demo screens,

	'Fighter Stats'		=> 'Statistiques Combattant',
	'Unfortunately this fighter is not yet playable.' => 'Malheureusement ce combattant n\'est pas encore jouable',
	'Name: '			=> 'Nom: ',
	'Team: '			=> 'Equipe: ',
	'Style: '			=> 'Style: ',
	'Age: '				=> 'Age: ',
	'Weight: '			=> 'Poids: ',
	'Height: '			=> 'Taille: ',
	'Shoe size: '		=> 'Pointure: ',
	
	"Credits"			=> "Cr�dits",
	"CreditsText1"		=>
"OPENMORTAL CREDITS


-- L'EQUIPE D'OPENMORTAL EST --


Programmation - UPi
Musique - Purple Motion
Musique - XTD / Mystic
Graphiques - UPi
German translation - ??
French translation - Vampyre
Spanish translation - EdsipeR
Portuguese translation - Vinicius Fortuna

-- Acteurs --

Boxer - Zoli
Cumi - Dans son r�le
Descant - Dans son r�le
Fureszes Orult - Ambrus
Grizli - Dans son r�le
Kinga - Dans son r�le
Macy - Dans son r�le
Misi - Dans son r�le
Rising-san - Surba
Sirpi - Dans son r�le
Taka Ito - Bence
Tokeletlen Katona - Dani
Watasiwa Baka Janajo - Ulmar
Black Dark Evil Mage - UPi

-- HEBERGEMENT --

sourceforge.net
apocalypse.rulez.org
freshmeat.net

OpenMortal est un Copyright 2003 de l'Equipe OpenMortal \
Distribu� sous Licence GNU General Public Licence Version 2\n\n",

	'CreditsText2'		=>
'Merci � Midway de ne pas nous harceler avec des trucs l�gaux (jusqu\'ici), bien que '.
'nous avons s�rement viol� au moins 50 de leurs brevets, copyrights internationaux, et marques d�pos�es.


OpenMortal a besoin de votre aide ! Si vous pouvez contribuer � la musique, graphiques, programmation am�lior�e, '.
'personnages suppl�mentaires, du fric, de la bi�re, des pizzas, ou toute autre forme de nourriture, SVP, envoyez-nous un email '.
'� upi@apocalypse.rulez.org! La m�me adresse accepte actuellement les commentaires et '.
"les emails de fans aussi (astuce, astuce !).\n\n",

	'CreditsText3'		=>
"Soyez certains de v�rifier les autres programmes de
Apocalypse Production
et
Degec Entertainment\n\n",

	'Story1Text'		=>
"Nous, les Dieux des Diaboliques Tueurs Noirs Antipathiques Faibles (humour) Mages de Feu, ne tol�rons plus longtemps le manque de faiseurs de mal.

Nous les avons une fois de plus envoy�s dans un mission si diabolique, si Luciferatique que le monde ne sera plus jamais le m�me!

Nous ordonnons � nos disciples indignes de
DETRUIRE LE SAMEDI
et de plonger l'humanit� dans un Moyen-Age de 5 jours de travail et 1 jour de vacances par semaine... A TOUT JAMAIS!\n\n\n\n\n\n\n\n\n",

	'Story2Text',		=>
"Bien que le Mal lorgne l'horizon, les gentils doivent sauver le jour ! Son Goku, le protecteur de la Terre et de l'Humanit� viennent � la rescousse...

Seulement afin de devenir le TUEUR IMPITOYABLE sur le long chemin du tournoi Mortal Szombat ! C'�tait la premi�re apparition de Cumi, apr�s tout...\n\n\n\n\n\n\n\n\n",
};










$::Spanish = { 'LanguageCode' => 1,

# In-game text

	'MAX COMBO!!!'		=> 'MAX COMBO!!!',
	'%d-hit combo!'		=> '%d-hit combo!',		# e.g. 3-hit combo!
	'%d%% damage'		=> '%d%% da�o',		# e.g. 30% damage
	'Round %d'			=> 'Round %d',
	'REW'				=> 'RETROCEDER',
	'REPLAY'			=> 'REPRODUCIR',
	'DEMO'				=> 'DEMO',
	'Press F1 to skip...'=>'Presiona F1 para saltar...',
	'HURRY UP!'			=> 'APURATE!',
	'TIME IS UP!'		=> 'SE ACABO EL TIEMPO!',

	'Final Judgement'	=> 'Juicio Final',
	'Continue?'			=> 'Continuar?',
	'SPLAT!'			=> 'SPLAT!',
	'Choose A Fighter Dammit' => 'Escoge un Peleador',
    
# Menu items

	"Main Menu"			=> "Menu Principal",
	"~SINGLE PLAYER GAME"=>"~1 SOLO JUGADOR ",
	"~MULTI PLAYER GAME"=> "~MULTIJUGADOR ",
	"~SURRENDER GAME"	=> "~ENTREGA ",
	"~OPTIONS"			=> "~OPCIONES",
	"~INFO"				=> "~INFO",
	"QUIT"				=> "SALIR",
	"~OK"				=> "~OK",
	"~LANGUAGE: "		=> "~LENGUAJE: ",

	"Options"			=> "Opciones",
	"~FULLSCREEN ON"	=> "~PANTALLA COMPLETA ON",
	"~FULLSCREEN OFF"	=> "~PANTALLA COMPLETA OFF",
	"GAME SPEED: "		=> "VELOCIDAD DEL JUEGO: ",
	"GAME TIME: "		=> "TIEMPO DEL JUEGO: ",
	"STAMINA: "			=> "STAMINA: ",
	"~SOUND"			=> "~SONIDO",
	"~RIGHT PLAYER KEYS"=> "~TECLAS JUGADOR DERECHO",
	"~LEFT PLAYER KEYS"	=> "~TECLAS JUGADOR IZQUIERDO",

	"Sound"				=> "Sonido",
	"CHANNELS: "		=> "CANALES: ",					# Mono / Stereo
	"SOUND QUALITY: "	=> "CALIDAD DEL SONIDO: ",		# KHz of playback rate
	"SOUND FIDELITY: "	=> "FIDELIDAD DEL SONIDO: ",	# 8 bit or 16 bit
	"MUSIC VOLUME: "	=> "VOLUMEN DE LA MUSICA: ",	# OFF or numeric
	"EFFECTS VOLUME: "	=> "VOLUMEN DE LOS EFECTOS: ",	# OFF or numeric
	

# Menu options

	"BABY"				=> "BEBE",
	"VERY LOW"			=> "MUY LENTO",
	"LOW"				=> "LENTO",
	"NORMAL"			=> "NORMAL",
	"HIGH"				=> "ALTO",
	"VERY HIGH"			=> "MUY ALTO",
	"NEAR IMMORTAL"		=> "CERCA DE LA INMORTALIDAD",

	"SNAIL RACE"		=> "CARRERA DE CARACOL",
	"SLOW"				=> "LENTO",
	"NORMAL"			=> "NORMAL",
	"TURBO"				=> "TURBO",
	"KUNG-FU MOVIE"		=> "PELICULA DE KUNG-FU",

	# Sound / Channels
	"MONO"				=> "MONO",
	"STEREO"			=> "STEREO",

	# Sound / Mixing rate settings
	"LOW"				=> "BAJO",
	"MEDIUM"			=> "MEDIO",
	"HIGH"				=> "ALTO",

	# Sound volume
	"OFF"				=> "OFF",
	
# Key configuration

	'%s player-\'%s\'?'	=> '%s jugador-\'%s\'?',		# The first %s becomes Left or Right. The second %s is up/down/high punch/...
	'Left'				=> 'Izquierda',
	'Right'				=> 'Derecha',
	"up",				=> "arriba",
	"down",				=> "abajo",
	"left",				=> "izquierda",
	"right",			=> "derecha",
	"block",			=> "bloquear",
	"low punch",		=> "pu�o bajo",
	"high punch",		=> "pu�o alto",
	"low kick",			=> "patada baja",
	"high kick"			=> "patada alta",
	'Thanks!'			=> 'Gracias!',
	
# Demo screens,

	'Fighter Stats'		=> 'Estad�sticas del Peleador',
	'Unfortunately this fighter is not yet playable.' => 'Desafortunadamente este jugador no esta habilitado.',
	'Name: '			=> 'Nombre: ',
	'Team: '			=> 'Equipo: ',
	'Style: '			=> 'Estilo: ',
	'Age: '				=> 'Edad: ',
	'Weight: '			=> 'Peso: ',
	'Height: '			=> 'Altura: ',
	'Shoe size: '		=> 'Tama�o de zapato: ',

	"Credits"			=> "Creditos",
	"CreditsText1"		=>
"CREDITOS


-- EL EQUIPO DE OPENMORTAL ES --


CODING - UPi
MUSIC - Purple Motion
MUSIC - XTD / Mystic
GRAPHICS - UPi
German translation - ??
French translation - Vampyre
Spanish translation - EdsipeR
Portuguese translation - Vinicius Fortuna

-- CAST --

Boxer - Zoli
Cumi - como el mismo
Descant - como el mismo
Fureszes Orult - Ambrus
Grizli - como el mismo
Kinga - como ella misma
Macy - como ella misma
Misi - como el mismo
Rising-san - Surba
Sirpi - como el mismo
Taka Ito - Bence
Tokeletlen Katona - Dani
Watasiwa Baka Janajo - Ulmar
Black Dark Evil Mage - UPi

-- HOSTING --

sourceforge.net
apocalypse.rulez.org
freshmeat.net

OpenMortal es Marca registrada 2003 de OpenMortal Team\
Distribuido bajo la GNU General Public Licence Version 2\n\n",

	'CreditsText2'		=>
'Gracias a Midway por no  aplastarnos con su equipo legal'.
'Seguramente nosotros violamos al menos 50 de sus patentes, marcas registradas internacionales y trademarks registradas .

OpenMortal necesita de tu ayuda! Si tu puedes contribuir con musica, imagenes, improvisaci�n de codigo, '.
'caracteres adicionales, dinero, cerveza, pizza o cualquier otra cosa consumible, por favor escribenos '.
'a upi@apocalypse.rulez.org! La misma direcci�n esta actualmente aceptando comentarios y '.
"tambien emails de fans (hint, hint!).\n\n",

	'CreditsText3'		=>
"Estate atento para verificar otro stuff desde
Apocalypse Production
y
Degec Entertainment\n\n",

	'Story1Text'		=>
"Nosotros, los Dioses del Endemoniado Negro Antipatico Asesino  Dim no tolero mas lo endemoniado.

Enviamos a ellos en una misi�n tan diab�lica, tan endemoniada que el mundo nunca ser� el mismo de nuevo!

Ordenamos a nuestros indignos seguidores a 

DESTRUIR EL SABADO
y plagar la humanidad en una epoca oscura de 5 dias de trabajo y uno de descanso por semana.... POR SIEMPRE!\n\n\n\n\n\n\n\n\n",

	'Story2Text',		=>
"Whenever EVIL looms on the horizon, the good guys are there to save the day. Son Goku, the protector of Earth and Humanity went to the rescue...

Only to become ROADKILL on his way to the Mortal Szombat tournament! It was Cumi's first time behind the wheel, after all...\n\n\n\n\n\n\n\n\n",
};










$::Portuguese = { 'LanguageCode' => 4,

# In-game text

	'MAX COMBO!!!'		=> 'COMBO M�XIMO!!!',
	'%d-hit combo!'		=> 'Combo de %d golpes!',  
	'%d%% damage'		=> 'Dano de %d%%',		
	'Round %d'		=> 'Round %d',
	'REW'			=> 'REBOBINANDO',
	'REPLAY'		=> 'REPLAY',
	'DEMO'			=> 'DEMO',
	'Press F1 to skip...'	=> 'Pressione F1 para resumir...',
	'HURRY UP!'		=> 'APRESSEM-SE!',
	'TIME IS UP!'		=> 'TEMPO ESGOTADO!',

	'Final Judgement'	=> 'Julgamento Final',
	'Continue?'		=> 'Continuar?',
	'SPLAT!'		=> 'SPLAT!',
	'Choose A Fighter Dammit' => 'Escolha um Lutador',

# Menu items

	"Main Menu"			=> "Menu Principal",
	"~SINGLE PLAYER GAME"		=> "JOGO ~INDIVIDUAL",
	"~MULTI PLAYER GAME"		=> "JOGO ~MULTIPLAYER" ,
	"~SURRENDER GAME"		=> "~Entregar-se",
	"~OPTIONS"			=> "~OP��ES",
	"~INFO"				=> "IN~FORMA��ES",
	"QUIT"				=> "SAIR",
	"~OK"				=> "~OK",
	"~LANGUAGE: "			=> "~L�ngua: ",

	"Options"			=> "Op��es",
	"~FULLSCREEN ON"		=> "TELA CHEIA ATIVADA",
	"~FULLSCREEN OFF"		=> "TELA CHEIA DESATIVADA",
	"GAME SPEED: "			=> "VELOCIDADE DE JOGO: ",
	"GAME TIME: "			=> "TEMPO DE JOGO: ",
	"STAMINA: "			=> "RESIST�NCIA: ",
	"~SOUND"			=> "~AUDIO",
	"~RIGHT PLAYER KEYS"		=> "TECLAS, JOGADOR DA ~DIREITA",
	"~LEFT PLAYER KEYS"		=> "TECLAS, JOGADOR DA ~ESQUERDA",
	
	"Sound"				=> "Audio",
	"CHANNELS: "		=> "CANAIS: ",	# Mono / Stereo
	"SOUND QUALITY: "	=> "QUALIDADE DE SOM: ",	# KHz of playback rate
	"SOUND FIDELITY: "	=> "FIDELIDADE DE SOM: ",# 8 bit or 16 bit
	"MUSIC VOLUME: "	=> "VOLUME DA M�SICA: ",	# OFF or numeric
	"EFFECTS VOLUME: "	=> "VOLUME DOS EFEITOS: ",	# OFF or numeric
	

# Menu options

	"BABY"				=> "BEB�",
	"VERY LOW"			=> "MUITO BAIXA",
	"LOW"				=> "BAIXA",
	"NORMAL"			=> "NORMAL",
	"HIGH"				=> "ALTA",
	"VERY HIGH"			=> "MUITO ALTA",
	"NEAR IMMORTAL"			=> "QUASE IMORTAL",

	"SNAIL RACE"			=> "CORRIDA DE LESMAS",
	"SLOW"				=> "DEVAGAR",
	"NORMAL"			=> "NORMAL",
	"TURBO"				=> "TURBO",
	"KUNG-FU MOVIE"			=> "FILME DE KUNG-FU",

	# Sound / Channels
	"MONO"				=> "MONO",
	"STEREO"			=> "EST�REO",

	# Sound / Mixing rate settings
	# CLASHES WITH MENU OPTIONS!!!
	"LOW"				=> "BAIXA",
	"MEDIUM"			=> "M�DIA",
	"HIGH"				=> "ALTA",
	
	# Sound volume
	"OFF"				=> "DESLIGADO",
	
# Key configuration

	'%s player-\'%s\'?'	=> 'Jogador da %s-\'%s\'?',		# The first %s becomes Left or Right. The second %s is up/down/high punch/...
	'Left'				=> 'Esquerda',
	'Right'				=> 'Direita',
	"up",				=> 'cima',
	"down",				=> 'baixo',
	"left",				=> 'esquerda',
	"right",			=> 'direita',
	"block",			=> 'defesa',
	"low punch",		=> 'soco baixo',
	"high punch",		=> 'soco alto',
	"low kick",			=> 'chute baixo',
	"high kick"			=> 'chute alto',
	'Thanks!'			=> 'Obrigado!',
	
# Demo screens,

	'Fighter Stats'		=> 'Ficha T�cnica',
	'Unfortunately this fighter is not yet playable.' => 'Infelizmente ainda n�o � poss�vel jogar com esse lutador',
	'Name: '			=> 'Nome: ',
	'Team: '			=> 'Time: ',
	'Style: '			=> 'Estilo: ',
	'Age: '				=> 'Idade: ',
	'Weight: '			=> 'Peso: ',
	'Height: '			=> 'Altura: ',
	'Shoe size: '		=> 'Cal�ado: ',
	
	"Credits"			=> 'Cr�ditos',
	"CreditsText1"		=>
"CR�DITOS DE OPENMORTAL


-- EQUIPE OPENMORTAL --


PROGRAMADOR - UPi
M�SICA - Purple Motion
M�SICA - XTD / Mystic
GR�FICOS - UPi
Tradu��o para o Alem�o - ??
Tradu��o para o Franc�s - Vampyre
Tradu��o para o Espanhol - EdsipeR
Tradu��o para o Portugu�s - Vinicius Fortuna

-- ELENCO --

Boxer - Zoli
Cumi - ele mesmo
Descant - ele mesmo
Fureszes Orult - Ambrus
Grizli - ele mesmo
Kinga - ela mesma
Macy - ela mesma
Misi - ele mesmo
Rising-san - Surba
Sirpi - ele mesmo
Taka Ito - Bence
Tokeletlen Katona - Dani
Watasiwa Baka Janajo - Ulmar
Black Dark Evil Mage - UPi

-- HOSPEDAGEM --

sourceforge.net
apocalypse.rulez.org
freshmeat.net

OpenMortal � marca registrada de 2003 da Equipe OpenMortal
Distribu�do segundo a Licen�a P�blica Geral GNU Vers�o 2\n\n",

	'CreditsText2'		=>
'Obrigado � Midway por n�o nos amea�ar com assuntos legais at� agora, apesar '.
'de n�s certamente termos violado pelo menos umas 50 de suas patentes, direitos de c�pia internacionais e marcas registradas.


OpenMortal precisa de sua ajuda! Se voc� puder contribuir com m�sicas, gr�icos, c�digos aperfei�oados, '.
'personagens extras, dinheiro, cerveja, pizza ou qualquer outro bem-de-consumo, por favor escreva para n�s em upi@apocalypse.rulez.org! '.
"Atualmente o mesmo endere�o tamb�m est� aceitando coment�rios e mensagens de f�s.\n\n",

	'CreditsText3'		=>
"N�o se esque�a de dar uma olhada nos outros produtos da
Apocalypse Production
e da Degec Entertainment\n\n",

	'Story1Text'		=>
"N�s, os Deuses dos Magos de Fogo Malignos Assassinos Sombrios Antip�ticos Est�pidos n�o mais toleramos a falta de maldade no mundo.

N�s os mandamos em uma miss�o t�o diab�lica, t�o maligna, que o mundo nunca mais ser� o mesmo!

N�s ordenamos nossos indignos seguidores que
DESTU�SSEM O S�BADO,
condenando a humanidade a uma era sombria, com 5 dias �teis e apenas 1 dia de folga por semana... PARA SEMPRE!\n\n\n\n\n\n\n\n\n",

	'Story2Text',		=>
"Sempre que o Mal surge no horizonte, os her�is est�o l� para salvar o dia. Son Goku, o protetor da Terra e da Humanidade, veio ent�o para nos salvar...

Apenas para ser MORTO ATROPELADO em seu caminho para o torneio de Mortal Szombat! Bem, afinal era o primeiro dia de dire��o de Cumi...\n\n\n\n\n\n\n\n\n",
};










%::LanguageCodes =
(
	'en'	=> $::English,
	'hu'	=> $::Hungarian,
	'fr'	=> $::French,
	'es'	=> $::Spanish,
	'pt'	=> $::Portuguese, 
);

=comment
SetLanguage sets the two-character language code.
=cut

$::Language = $::English;
$::Language = $::Hungarian;
$::Language = $::French;
$::Language = $::Spanish;
$::Language = $::Portuguese;

sub SetLanguage($)
{
	my ($NewLanguageCode) = @_;
	$::LanguageCode = $NewLanguageCode;
	
	if ( defined $::LanguageCodes{$::LanguageCode} )
	{
		$::Language = $::LanguageCodes{$::LanguageCode};
	}
	else
	{
		print "Language $::LanguageCode is not available. Reverting to English.\n";
		$::Language = $::English;
		$::LanguageCode = 'en';
	}
	$::LanguageNumber = $::Language->{'LanguageCode'};
}



sub Translate($)
{
	my $text = shift;
	# print "The translation of '$text' is ";

	$text = $::Language->{$text} if defined $::Language->{$text};

	# print "'$text'.\n";

	$::Translated = $text;
}


1;


# "Connect to game", "Create game"
# "Network mode: ",  "Connect to: ", "Start Network Play!", "Cancel"
# "Connection closed."
# "Press Escape to abort"
# "Press Escape for the menu"
# "You must have port 14882 open for this to work."