# package Bence;

# Bence's images are scaled by a factor of ?

require 'DataHelper.pl';
use strict;

sub LoadBence
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Bence';

@Frames = LoadFrames( "$codename.dat.txt", 420, 310 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		23,		"stand",		4,		"walk",			12,
"turn",			8,		"falling",		18,		"laying",		1,
"getup",		12,		"hurt",			8,		"swept",		8,
"won",			8,		"block",		5,		"kneeling",		4,
"onknees",		4,		"kneelingpunch",5,
"fun",			5,		"threat",		6,
"highpunch",	6,		"lowpunch",		5,		"highkick",		7,
"lowkick",		6,		"sweep",		7,		"groinkick",	6,
"kneekick",		6,		"elbow",		7,		"spin",			10,
"grenade",		11,		"uppercut",		7,		"throw",		22,
"specpunch",	14,		"pierce",		8,
"highpunched",	5,		"lowpunched",	6,		"groinkicked",	8,
"kneelingpunched",4,	"kneelingkicked", 5,	"thrown",		8,
);


# Bence has no kneelingkick... substitute kneelingpunch.  This is for Jumping..
for ( $st = 1; $st <= 5; ++$st ) { $FrameLookup{"kneelingkick$st"} = $FrameLookup{"kneelingpunch$st"}; }


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
'lpunchFBFB'=>'SpecPunch',
'hpunchFB'=>'Pierce',
};


$Con2 = {
'lkick'=>'KneelingPunch',
'hkick'=>'KneelingPunch',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>7,	'S'=>'+start 1-8',
	'DEL7'=>15, 'DEL14'=>15, },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,-stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, 'DELTAXN'=>80 },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won, won 7-5, won 6-8, won 7-5',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won 5',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat,-threat', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 1, 130, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch'} ),

# 2. OFFENSIVE MOVES

{ 'N'=>'KneelingPunch',	'DEL'=>5,	'S'=>'+kneelingpunch,-kneelingpunch', 'SITU'=>'Crouch',
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
	'HIT5'=>'Highhit', 'HIT6'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin,-spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep,-sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade, grenade n-8, grenade 5-1',
	'DEL11'=>15, 'DOODAD11'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT5'=>'Uppercut', 'HIT6'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'SpecPunch',		'DEL'=>5,	'S'=>'+specpunch, specpunch 4-13, specpunch 4-1',
	'HIT'=>'Hit' },
{ 'N'=>'Pierce',		'DEL'=>5,	'S'=>'+pierce,-pierce',
	'HIT'=>'Highhit' },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'falling 1-11, falling 15-12, falling 12-n, laying 1',
	'MOVE'=>1,
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

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 19 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 0, 0 );
#TravelingStates( \%FrameLookup, \@Frames, \%States, "won", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

# %::BenceStates = %States;
# @::BenceFrames = @Frames;

::RegisterFighter( {
	'ID'			=> 7,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 6750636,
} );

}

LoadBence();
# QuickSave('Bence', \@::BenceFrames, \%::BenceStates);

return 1;

