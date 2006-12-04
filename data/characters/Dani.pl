# package Dani;

# Dani's images are scaled by a factor of 0.75

require 'DataHelper.pl';
use strict;

sub LoadDani
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Dani';

@Frames = LoadFrames( "$codename.dat.txt", 280, 100 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start",		20,		"stand",		5,		"walk",			18,
"falling",		13,		"laying",		1,
"getup",		21,		"hurt",			8,		"swept",		13,
"won",			11,		"block",		5,		"kneeling",		4,
"onknees",		1,		"kneelingkick",	5,		"kneelingpunch",4,
"kneelingpunched",5,	"kneelingkicked", 5,	
"fun",			12,		"threat",		5,
"highpunch",	4,		"lowpunch",		5,		"highkick",		6,
"lowkick",		6,		"groinkick",	6,
"kneekick",		6,		"elbow",		5,
"sweep",		8,		"spin",			11,
"grenade",		14,		"uppercut",		8,		"throw",		25,
"highpunched",	12,		"lowpunched",	6,		"groinkicked",	8,
"thrown",		16,
"ram",			9,		"poke",			6,		"stab",			11,
"capdoodad",	1,
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
'hpunchFF'=>'Ram',
'lpunchBDB'=>'Stab',
'lkickFB'=>'Poke',

};


$Con2 = {
'lkick'=>'KneelingKick',
'hkick'=>'KneelingKick',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>6,	'S'=>'+start',
	'DEL13'=>12, },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,_stand', 'CON'=>$Con, 'SITU'=>'Ready', },
{ 'N'=>'Turn',			'DEL'=>6,	'S'=>'spin 1-3, spin 3-1',	'TURN3'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun', 'CON'=>$Con, 'SITU'=>'Ready',
	'DEL7'=>20 },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat,-threat', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 0, 141, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch' } ),

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
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade, uppercut 2-1',
	'DEL12'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'Ram',			'DEL'=>7,	'S'=>'+ram,-ram',
	'HIT'=>'Uppercut',	'DEL9'=>16,
	'MOVE6'=>3, 'MOVE7'=>4, 'MOVE8'=>4, 'MOVE9'=>4, 'MOVE10'=>4, 'MOVE11'=>4, 'MOVE12'=>3 },
{ 'N'=>'Poke',			'DEL'=>7,	'S'=>'+poke, -poke',
	'HIT'=>'Highhit' },
{ 'N'=>'Stab',			'DEL'=>6,	'S'=>'+stab, -stab', 'DEL11'=>16,
	'HIT'=>'Fall' },


# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'falling 1-12, falling 12-10, falling 13, laying 1',
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
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked, -kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched,-lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked, -groinkicked', 'DEL8'=>16, },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },

);



# 2. CREATE STATES

foreach $framedesc (@States1)
{
	AddStates( \%States, \%FrameLookup, $framedesc );
}



# Automatically add NEXTST for states which don't have one.

FixStates( \%FrameLookup, \%States );

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 1, 14 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "hurt", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "ram", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "swept", 0, 0 );

%States = ( FindShorthands( \%States ), %States );


::RegisterFighter( {
	'ID'			=> 12,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 6462957,
} );

}

LoadDani();

return 1;

