# package Ulmar;

# Ulmar's images are scaled: 0.75

require 'DataHelper.pl';
use strict;

sub LoadUlmar
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar );

@Frames = LoadFrames( "ULMARDATA.DAT.pl", 270, 75 );
%FrameLookup = CreateFrameLookup( scalar @Frames - 1,
"start",		30,		"stand",		6,		"walk",			12, "junk",1,
"turn",			8,		"falling",		14,		"laying",		1,
"getup",		20,		"hurt",			13,		"swept",		9,
"won",			14,		"block",		6,		"kneeling",		4,
"onknees",		4,		"kneelingpunch",6,		"kneelingkick",	5,
"fun",			7,		"threat",		5,		"highpunch",	11,
"lowpunch",		10,		"highkick",		6,		"junkb",1,
"lowkick",		6,		"sweep",		10,
"groinkick",	7,		"kneekick",		7,		"elbow",		7,
"spin",			11,		"grenade",		10,		"uppercut",		9,
"throw",		34,		"highpunched",	6,		"lowpunched",	7,
"groinkicked",	8,		"kneelingpunched",5,	"kneelingkicked", 6,
"thrown",		10,		"specsomething",9,
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
'hpunchB'=>'Spin',
'lkickB'=>'Sweep',

'lpunchBD'=>'Grenade',
'lpunchFBF'=>'SpecSomething',
};


$Con2 = {
'lkick'=>'KneelingKick',
'hkick'=>'KneelingKick',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>5,	'S'=>'+start 1-8' },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,_stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat,-threat', 'CON'=>$Con },
WalkingFrames( \%FrameLookup, \@Frames, 0, 111, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch',}  ),

# 2. OFFENSIVE MOVES

{ 'N'=>'KneelingPunch',	'DEL'=>5,	'S'=>'+kneelingpunch,-kneelingpunch', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKick',	'DEL'=>5,	'S'=>'+kneelingkick,-kneelingkick', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingUppercut','DEL'=>5,	'S'=>'kneeling 4-3,uppercut 6-n,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'HighPunch',		'DEL'=>5,	'S'=>'+highpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>5,	'S'=>'+lowpunch',
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
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin,-spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep,-sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade,-grenade',
	'DEL12'=>15, 'DOODAD10'=>$Doodad },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'SpecSomething',	'DEL'=>5,	'S'=>'+specsomething', },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'+falling, laying 1',
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

{ 'N'=>'Swept',			'DEL'=>5,	'S'=>'+swept,-swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked, -kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched, -highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched,-lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked, -groinkicked' },
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

%States = ( FindShorthands( \%States ), %States );

%::UlmarStates = %States;
@::UlmarFrames = @Frames;
}

LoadUlmar();

return 1;

