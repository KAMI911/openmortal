# package Maci;

# UPi's images are scaled by a factor of 0.8

require 'DataHelper.pl';
use strict;

sub LoadMaci
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar );

@Frames = LoadFrames( "MACIDATA.DAT.pl", 316, 90 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		23,		"stand",		5,		"walk",			14,
"turn",			9,		"falling",		12,		"laying",		1,
"getup",		18,		"hurt",			13,		"swept",		8,
"won",			17,		"block",		5,		"kneeling",		4,
"onknees",		4,		"kneelingpunch",5,		"kneelingkick",	6,
"fun",			9,		"threat",		8,
"highpunch",	5,		"lowpunch",		6,		"highkick",		7,
"lowkick",		6,		"sweep",		8,		"groinkick",	10,
"kneekick",		7,		"elbow",		6,		"spin",			11,
"grenade",		10,		"uppercut",		8,		"throw",		15,
"highpunched",	8,		"lowpunched",	6,		"groinkicked",	14,
"kneelingpunched",8,	"kneelingkicked", 6,	"thrown",		12,
"scissorkick",	15
);


$Doodad =
	{	'T'		=> 1,
		'W'		=> 64,
		'H'		=> 64,
		'SX'	=> 15,
		'SY'	=> 0,
		'FRAMES'=> 6,
		'SA'	=> 1/25,
	};


$Con = {
'forw'=>'Walk',
'back'=>'Back',
'jump'=>'Jump',
'jumpfw'=>'JumpFW',
'jumpbw'=>'JumpBW',
'block'=>'Block',
'down'=>'Kneeling',

'hpunch'=>'HighPunch',
'lpunch'=>'LowPunch',
'hkick'=>'HighKick',
'lkick'=>'LowKick',
'lkickBF'=>'GroinKick',
'lkickF'=>'KneeKick',
'hpunchD'=>'Uppercut',
'hpunchF'=>'Elbow',
'hkickB'=>'Spin',
'lkickB'=>'Sweep',

'hkickFF'=>'ScissorKick',
'lpunchBD'=>'Grenade',

};


$Con2 = {
'lkick'=>'KneelingKick',
'hkick'=>'KneelingKick',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>5,	'S'=>'+start' },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,_stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt, hurt 2-1' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>7,	'S'=>'+fun,-fun', 'CON'=>$Con },
{ 'N'=>'Threat',		'DEL'=>7,	'S'=>'threat 2-n, threat n-2', 'CON'=>$Con },
WalkingFrames( \%FrameLookup, \@Frames, 1, 106, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch'} ),

# 2. OFFENSIVE MOVES

{ 'N'=>'KneelingPunch',	'DEL'=>5,	'S'=>'+kneelingpunch,-kneelingpunch', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKick',	'DEL'=>5,	'S'=>'+kneelingkick,-kneelingkick', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingUppercut','DEL'=>5,	'S'=>'kneeling 4-3,uppercut 6-n,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'HighPunch',		'DEL'=>5,	'S'=>'+highpunch,-highpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>5,	'S'=>'+lowpunch,-lowpunch',
	'HIT'=>'Hit' },
{ 'N'=>'HighKick',		'DEL'=>5,	'S'=>'+highkick,-highkick',
	'HIT'=>'Hit' },
{ 'N'=>'LowKick',		'DEL'=>5,	'S'=>'+lowkick,-lowkick',
	'HIT'=>'Leghit' },
{ 'N'=>'GroinKick',		'DEL'=>5,	'S'=>'groinkick 1-7,groinkick 7-1, groinkick 8-10, groinkick 10-8',
	'HIT'=>'Groinhit', 'DEL15'=>8, 'DEL16'=>8, 'DEL17'=>8, 'DEL18'=>8, 'DEL19'=>8, 'DEL20'=>8,  },
{ 'N'=>'KneeKick',		'DEL'=>5,	'S'=>'+kneekick,-kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow,-elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>7,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep,-sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade, grenade 1',
	'DEL7'=>15, 'DOODAD'=>$Doodad },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'ScissorKick',	'DEL'=>5,	'S'=>'+scissorkick',
	'HIT'=>'Uppercut',
	'MOVE5'=>3, 'MOVE6'=>4, 'MOVE7'=>4, 'MOVE8'=>4, 'MOVE9'=>4, 'MOVE10'=>4, 'MOVE11'=>3},

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'+falling, laying 1',
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'+laying',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>5,	'S'=>'+getup',
	'SITU'=>'Falling',
	'CON9'=>{'down'=>'Onknees'},
	'CON10'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>5,	'S'=>'+swept,-swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked, -kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched,-lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked' },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },

);



# 2. CREATE STATES

foreach $framedesc (@States1)
{
	AddStates( \%States, \%FrameLookup, $framedesc );
}



# Automatically add NEXTST for states which don't have one.

FixStates( \%FrameLookup, \%States );

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 13 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "spin", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

%::MaciStates = %States;
@::MaciFrames = @Frames;
}

LoadMaci();

return 1;

