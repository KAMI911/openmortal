# package Judy;

# Judy's images are scaled by a factor of 0.8

require 'DataHelper.pl';
use strict;

sub LoadJudy
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Judy';

@Frames = LoadFrames( "$codename.dat.txt", 425, 90 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
'start',		14,	'stand',		5,	'walk',			16,
'turn',			8,	'falling',		17,	'laying',		1,
'getup',		18,	'hurt',			7,	'swept',		7,
'fun',			7,	'won',			18,	'block',		5,
'kneeling',		4,	'onknees',		4,	'kneelingpunch',5,
'kneelingkick',	5,	'funb',			8,	'threat',		8,
'highpunch',	4,	'lowpunch',		5,	'groinkick',	7,
'kneekick',		6,	'elbow',		5,	'spin',			10,
'grenade',		15,	'uppercut',		9,	'throw',		22,
'highpunched',	5,	'lowpunched',	5,	'groinkicked',	9,
'kneelingpunched',5,'kneelingkicked',5,	'thrown',		13,
'sweep',		7,	'pillow',		12,	'highkick',		6,
'lowkick',		6,
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
'hpunchFB'=>'Pillow',
};


$Con2 = {
'lkick'=>'KneelingKick',
'hkick'=>'KneelingKick',
'hpunch'=>'KneelingUppercut',
'lpunch'=>'KneelingPunch',
};



@States1 = (
# 1. BASIC MOVES

{ 'N'=>'Start',			'DEL'=>7,	'S'=>'+start', 'DEL4'=>20, },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,_stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Funb',			'DEL'=>8,	'S'=>'+funb,-funb', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat,-threat', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 1, 200, $Con ),
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
{ 'N'=>'GroinKick',		'DEL'=>5,	'S'=>'+groinkick,-groinkick',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>5,	'S'=>'+kneekick,-kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow,-elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin,-spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>5,	'S'=>'+kneeling, +sweep, sweep 2-1',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade',
	'DEL11'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'Pillow',		'DEL'=>5,	'S'=>'+pillow',
	'DOODAD'=>'ZoliShot' },

#{ 'N'=>'CraneKick',		'DEL'=>5,	'S'=>'+cranekick, -cranekick',
#	'HIT'=>'Uppercut', 'JUMP4'=>60, 'PUSHX4'=>12*16 },
#{ 'N'=>'BurningHands',	'DEL'=>5,	'S'=>'+burninghands', },

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
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked,-groinkicked' },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },

);

# Slight modification to Judy: The jumpkick sequence will be replaced 
# with the sweeping move

foreach $framedesc (@States1) {
	if ( $framedesc->{N} eq 'JumpKick' ) {
		$framedesc->{S} =~ s/\+\s*kneelingkick/sweep 2-5/g;
		$framedesc->{S} =~ s/-\s*kneelingkick/sweep 5-2/g;
		$framedesc->{S} =~ s/kneelingkick\s+n/sweep 5/g;
		# +kneelingkick,kneelingkick n, kneelingkick n, kneelingkick n,-kneelingkick,-kneeling
		last;
	}
}



# 2. CREATE STATES

foreach $framedesc (@States1)
{
	AddStates( \%States, \%FrameLookup, $framedesc );
}



# Automatically add NEXTST for states which don't have one.

FixStates( \%FrameLookup, \%States );

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "getup", 0, 0 );
#TravelingStates( \%FrameLookup, \@Frames, \%States, "won", 0, 0 );

%States = ( FindShorthands( \%States ), %States );


::RegisterFighter( {
	'ID'			=> 16,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 6462957,
} );

}

LoadJudy();

1;

