# package Sirpi;

# Sirpi's images are scaled by a factor of 0.76

require 'DataHelper.pl';
use strict;

sub LoadSirpi
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Sirpi';

@Frames = LoadFrames( "$codename.dat.txt", 250, 110 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		20,		"stand",		7,		"walk",			13,
"turn",			9,		"falling",		14,		"laying",		1,
"getup",		16,		"hurt",			13,		"swept",		13,
"won",			10,		"block",		5,
"fun",			8,		"threat",		9,
"kneeling",		4,		"onknees",		4,
"highpunch",	5,		"lowpunch",		6,		"highkick",		7,
"lowkick",		6,		"groinkick",	7,		"sweep",		9,
"kneekick",		7,		"elbow",		6,		"spin",			14,
"kneelingpunch",5,		"kneelingkick",	6,
"grenade",		9,		"uppercut",		8,		"throw",		22,
"highpunched",	5,		"lowpunched",	5,		"groinkicked",	9,
"kneelingpunched",4,	"kneelingkicked", 6,	"thrown",		14,
"specpunch",	13,		"funb",			12,
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

'lpunchFD'=>'Grenade',
'hpunchFF'=>'SpecPunch',
};


$Con2 = {
'lkick'=>'KneelingKick',
'hkick'=>'KneelingKick',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>7,	'S'=>'+start' },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,_hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won, won n-6, won 7-n, won n-6, won 7-n', 'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'fun 1-6, fun 6-4, fun 7-8, fun 8-7, fun 4-6, fun 6-1', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Funb',			'DEL'=>8,	'S'=>'+funb, -funb', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>8,	'S'=>'+threat, threat 3-1', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 2, 80, $Con ),
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
{ 'N'=>'KneelingUppercut','DEL'=>5,	'S'=>'kneeling 4-3,uppercut 5-n,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'HighPunch',		'DEL'=>5,	'S'=>'+highpunch,-highpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>5,	'S'=>'+lowpunch,-lowpunch',
	'HIT'=>'Hit' },
{ 'N'=>'HighKick',		'DEL'=>5,	'S'=>'+highkick,-highkick',
	'HIT'=>'Hit' },
{ 'N'=>'LowKick',		'DEL'=>5,	'S'=>'+lowkick,-lowkick',
	'HIT'=>'Leghit' },
{ 'N'=>'GroinKick',		'DEL'=>5,	'S'=>'+groinkick,-groinkick',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>5,	'S'=>'+kneekick,-kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow,-elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep,-sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade,-grenade',
	'DEL12'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'SpecPunch',		'DEL'=>5,	'S'=>'+specpunch',
	'HIT'=>'Highhit' },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'falling 1-12, falling 12-10, falling 13-n, laying 1',
	'DEL10'=>7, 'DEL11'=>7, 'DEL12'=>7, 'DEL13'=>7, 'DEL14'=>7, 'DEL15'=>7,
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

{ 'N'=>'Swept',			'DEL'=>5,	'S'=>'+swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked, -kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched, -highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched,-lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked,-groinkicked' },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },

);



# 2. CREATE STATES

foreach $framedesc (@States1)
{
	AddStates( \%States, \%FrameLookup, $framedesc );
}



# Automatically add NEXTST for states which don't have one.

FixStates( \%FrameLookup, \%States );

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 15 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "swept", 0, 0 );	#?
TravelingStates( \%FrameLookup, \@Frames, \%States, "spin", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "specpunch", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

# %::SirpiStates = %States;
# @::SirpiFrames = @Frames;

::RegisterFighter( {
	'ID'			=> 5,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 5587262,
} );

}

LoadSirpi();

return 1;

