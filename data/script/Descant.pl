# package Descant;


require 'DataHelper.pl';
use strict;

sub LoadDescant
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar );

@Frames = LoadFrames( "DESCANTDATA.DAT.pl", 280, 200 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		13,		"stand",		4,		"walk",			10,
"turn",			9,		"falling",		16,		"laying",		1,
"getup",		15,		"hurt",			7,		"swept",		10,
"won",			20,		"block",		5,		"kneeling",		4,
"onknees",		4,		"kneelingpunch",5,		"kneelingkick",	5,
"fun",			13,		"threat",		7,
"highpunch",	5,		"lowpunch",		6,		"highkick",		7,
"lowkick",		6,		"sweep",		11,		"groinkick",	6,
"kneekick",		6,		"elbow",		5,
"grenade",		10,		"uppercut",		7,		"throw",		23,
"highpunched",	5,		"lowpunched",	6,		"groinkicked",	9,
"kneelingpunched",5,	"kneelingkicked", 5,	"thrown",		8,
"knife",		17,		"specpunch",	19,		"hipshot",		9,
"funb",			11,
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
'lkickB'=>'Sweep',

'lpunchBD'=>'Grenade',
'hpunchDF'=>'Knife',
'hpunchFF'=>'SpecPunch',
'lpunchBB'=>'HipShot',
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
{ 'N'=>'Stand',			'DEL'=>10,	'S'=>'+stand,_stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt, -hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>10,	'S'=>'+fun', 'CON'=>$Con },
{ 'N'=>'Funb',			'DEL'=>8,	'S'=>'+funb,-funb', 'CON'=>$Con },
{ 'N'=>'Threat',		'DEL'=>7,	'S'=>'+threat,-threat', 'CON'=>$Con },
WalkingFrames( \%FrameLookup, \@Frames, 1, 100, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch',
	'hpunchFF'=>'JumpFire'} ),

# 2. OFFENSIVE MOVES

{ 'N'=>'KneelingPunch',	'DEL'=>7,	'S'=>'+kneelingpunch,-kneelingpunch', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKick',	'DEL'=>7,	'S'=>'+kneelingkick,-kneelingkick', 'SITU'=>'Crouch',
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
{ 'N'=>'GroinKick',		'DEL'=>5,	'S'=>'+groinkick,-groinkick',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>5,	'S'=>'+kneekick,-kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow,-elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Sweep',			'DEL'=>8,	'S'=>'+sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade,_grenade',
	'DEL12'=>15, 'DOODAD'=>$Doodad },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },
{ 'N'=>'Knife',			'DEL'=>7,	'S'=>'+knife' },
{ 'N'=>'SpecPunch',		'DEL'=>7,	'S'=>'+specpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'HipShot',		'DEL'=>7,	'S'=>'+hipshot, -hipshot',
	'DOODAD9'=>$Doodad },



# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'+falling, laying 1',
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'+laying',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>5,	'S'=>'+getup',
	'SITU'=>'Falling',
	'CON8'=>{'down'=>'Onknees'},
	'CON9'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>7,	'S'=>'+swept',
	'MOVE'=>-1 },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked, -kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched, -highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched, -lowpunched' },
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

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 17 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "swept", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

#%::DescantStates = %States;
#@::DescantFrames = @Frames;

::RegisterFighter( {
	'ID'			=> 9,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'DATAFILE'		=> 'DESCANTDATA.DAT',
	'DATASIZE'		=> 5877193,
} );
}

LoadDescant();

return 1;

