# package Grizli;

# Grizli's images are scaled by a factor of 0.763

require 'DataHelper.pl';
use strict;

sub LoadGrizli
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar );

@Frames = LoadFrames( "GRIZLIDATA.DAT.pl", 250, 100 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		14,		"stand",		6,		"walk",			15,
"turn",			9,		"falling",		15,		"laying",		1,
"getup",		16,		"hurt",			9,		"kneeling",		4,
"onknees",		5,		"kneelingpunch",5,		"kneelingkick",	5,
"fun",			9,		
"highpunch",	9,		"lowpunch",		6,		"highkick",		6,
"lowkick",		6,		"sweep",		15,		"groinkick",	7,
"kneekick",		8,		"elbow",		12,		"spin",			17,
"grenade",		10,		"uppercut",		17,		"throw",		19,
"highpunched",	11,		"lowpunched",	6,		"groinkicked",	8,
"kneelingpunched",5,	"kneelingkicked", 4,	"thrown",		14,
"specpunch",	6,		"nunchaku",		15,
"swept",		16,		"won",			20,		"threat",		12,
"block",		6,		"stomp",		11,
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

'lpunchBD'=>'Grenade',
'hpunchFF'=>'Poke',
'lkickDD'=>'Stomp',
'hpunchBFB'=>'Nunchaku',
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
{ 'N'=>'Stand',			'DEL'=>10,	'S'=>'+stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'DELTAXN'=>-10, 'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat', 'CON'=>$Con },
WalkingFrames( \%FrameLookup, \@Frames, 0, 75, $Con ),
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
{ 'N'=>'KneelingUppercut','DEL'=>5,	'S'=>'kneeling 4-3,uppercut 6-n',
	'HIT'=>'Uppercut' },
{ 'N'=>'HighPunch',		'DEL'=>5,	'S'=>'+highpunch',
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
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade',
	'DEL12'=>15, 'DOODAD'=>$Doodad },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'Poke',			'DEL'=>5,	'S'=>'+specpunch,_specpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'Stomp',			'DEL'=>7,	'S'=>'+stomp',
	'HIT'=>'Fall',		'CODE6'=>'::AddEarthquake(25);', 'SOUND6'=>'crashhh.voc' },
{ 'N'=>'Nunchaku',		'DEL'=>7,	'S'=>'+nunchaku',
	'HIT'=>'Hit' },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'falling 1-11, falling 15-12, falling 12-n, laying 1',
	'DEL16'=>7, 'DEL17'=>7, 'DEL18'=>7, 'DEL19'=>7, 'DEL20'=>7, 'DEL21'=>7,
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'+laying',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>5,	'S'=>'+getup',
	'SITU'=>'Falling',
	'CON8'=>{'down'=>'Onknees'},
	'CON8'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>5,	'S'=>'+swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'kneelingkicked 3-n, kneelingkicked n-3',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched' },
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

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 18 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 1, 16 );
#TravelingStates( \%FrameLookup, \@Frames, \%States, "won", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

%::GrizliStates = %States;
@::GrizliFrames = @Frames;
}

LoadGrizli();

return 1;

