# package UPi;

# UPi's images are scaled by a factor of 0.8

require 'DataHelper.pl';
use strict;

sub LoadUPi
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar );

@Frames = LoadFrames( "UPIDATA.DAT.pl", 275, 90 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		18,		"stand",		5,		"walk",			14,
"turn",			8,		"falling",		17,		"laying",		1,
"getup",		16,		"hurt",			12,		"swept",		6,
"won",			19,		"block",		5,		"kneeling",		4,
"onknees",		4,		"kneelingpunch",5,		"kneelingkick",	5,
"fun",			5,		"funb",			3,		"threat",		6,
"highpunch",	4,		"lowpunch",		6,		"highkick",		6,
"lowkick",		5,		"sweep",		7,		"groinkick",	6,
"kneekick",		7,		"elbow",		7,		"spin",			10,
"grenade",		18,		"uppercut",		8,		"throw",		15,
"highpunched",	6,		"lowpunched",	5,		"groinkicked",	19,
"kneelingpunched",4,	"kneelingkicked", 7,	"thrown",		13,
"cranekick",	9,		"burninghands",	13,		"kneelingfire",	12,
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
'hkickFF'=>'CraneKick',
'hpunchUB'=>'BurningHands',
};


$Con2 = {
'lkick'=>'KneelingKick',
'hkick'=>'KneelingKick',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
'hpunchFF'=>'KneelingFire',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>5,	'S'=>'start 1-8, start 8-1, start 9-n' },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,_stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con },
{ 'N'=>'Funb',			'DEL'=>8,	'S'=>'+funb,-funb', 'CON'=>$Con },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat,-threat', 'CON'=>$Con },
WalkingFrames( \%FrameLookup, \@Frames, 4, 110, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch',
	'hpunchFF'=>'JumpFire'} ),

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
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade',
	'DEL12'=>15, 'DOODAD'=>$Doodad },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'CraneKick',		'DEL'=>5,	'S'=>'+cranekick, _cranekick',
	'HIT'=>'Uppercut', 'JUMP4'=>60, 'PUSHX4'=>12*16 },
{ 'N'=>'BurningHands',	'DEL'=>5,	'S'=>'+burninghands', },
{ 'N'=>'KneelingFire',	'DEL'=>5,	'S'=>'+kneelingfire', 'SITU'=>'Crouch',
	'DOODAD'=>$Doodad, 'NEXTSTN'=>'Onknees' },
{ 'N'=>'JumpFire',		'DEL'=>3,	'S'=> '+kneelingfire, -kneeling',
	'DOODAD'=>$Doodad, 'DELN'=>100 },

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

{ 'N'=>'Swept',			'DEL'=>5,	'S'=>'+swept,-swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'kneelingkicked 3-n, kneelingkicked n-3',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'highpunched 2-n, -highpunched' },
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

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 18 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 1, 16 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "won", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

%::UPiStates = %States;
@::UPiFrames = @Frames;
}

LoadUPi();

return 1;

