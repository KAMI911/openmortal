# package Jozsi;


require 'DataHelper.pl';
use strict;

sub LoadJozsi
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Jozsi';

@Frames = LoadFrames( "$codename.dat.txt", 245, 60 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
'start', 19,
'stand', 7,
'walk', 13,
'turn', 11,
'falling', 15,
'laying', 1,
'getup', 11,
'hurt', 14,
'won', 10,
'block', 5,
'kneeling', 4,
'onknees', 4,
'kneelingpunch', 3, 'junkknp', 4,
'kneelingkick', 4, 'junkknk', 2,
'kneelingpunched', 4,
'kneelingkicked', 5,
'fun', 10,
'threat', 10,
'highpunch', 7,
'lowpunch', 8,
'highkick', 9,
'lowkick', 6,
'groinkick', 8,
'kneekick', 5,
'elbow', 8,
'spin', 10,
'fallsomething', 12,
'grenade', 7,
'uppercut', 10,
'throw', 12,
'highpunched', 7,
'lowpunched', 6,
'groinkicked', 8,
'swept', 7,
'thrown', 8,
'friend', 31,
'specpunch', 20,
);



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

'lpunchBD'=>'Grenade',
'hpunchFF'=>'SpecPunch',
'lpunchFF'=>'Friend',
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
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'stand 3-7, stand 7-3', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 0, 110, $Con ),
BlockStates( FindLastFrame( \%FrameLookup, 'block' ), 5 ),
KneelingStates( FindLastFrame( \%FrameLookup, 'kneeling' ),
	FindLastFrame( \%FrameLookup, 'onknees' ), 7, $Con2 ),
JumpStates( \%FrameLookup,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch',} ),

# 2. OFFENSIVE MOVES

{ 'N'=>'KneelingPunch',	'DEL'=>5,	'S'=>'+kneelingpunch,-kneelingpunch', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKick',	'DEL'=>5,	'S'=>'+kneelingkick,-kneelingkick', 'SITU'=>'Crouch',
	'HIT'=>'Hit', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingUppercut','DEL'=>5,	'S'=>'kneeling 3-2,uppercut 2-n',
	'HIT'=>'Uppercut' },
{ 'N'=>'HighPunch',		'DEL'=>7,	'S'=>'+highpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>7,	'S'=>'+lowpunch',
	'HIT'=>'Hit' },
{ 'N'=>'HighKick',		'DEL'=>7,	'S'=>'+highkick',
	'HIT'=>'Hit' },
{ 'N'=>'LowKick',		'DEL'=>7,	'S'=>'+lowkick',
	'HIT'=>'Leghit' },
{ 'N'=>'GroinKick',		'DEL'=>7,	'S'=>'+groinkick',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>10,	'S'=>'+kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>6,	'S'=>'+elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>6,	'S'=>'+spin',
	'HIT'=>'Hit' },
#{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep',
#	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>7,	'S'=>'+grenade',
	'DEL5'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>7,	'S'=>'+uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'SpecPunch',		'DEL'=>6,	'S'=>'+specpunch',
	'HIT'=>'Highhit', },
{ 'N'=>'Friend',		'DEL'=>5,	'S'=>'+friend',
	'HIT'=>'Uppercut', },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>7,	'S'=>'+falling',
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'laying n',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>6,	'S'=>'+getup',
	'SITU'=>'Falling',
	'CON4'=>{'down'=>'Onknees'},
	'CON5'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>8,	'S'=>'+swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked 1-4,kneelingkicked 4-1',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>7,	'S'=>'+lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked,groinkicked 3-1' },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },

);



# 2. CREATE STATES

foreach $framedesc (@States1)
{
	AddStates( \%States, \%FrameLookup, $framedesc );
}



# Automatically add NEXTST for states which don't have one.

FixStates( \%FrameLookup, \%States );

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "won", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

# %::UPiStates = %States;
# @::UPiFrames = @Frames;

::RegisterFighter( {
	'ID'			=> 104,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 6004229,
        'NAME'  =>'Jozsi a hegyrol',
        'TEAM'  =>'Good',
        'STYLE' =>'cserno-fu',
        'AGE'   =>'60',
        'WEIGHT'=>'70kg',
        'HEIGHT'=>'185cm',
        'SHOE'  =>'45',
        'STORY' =>
'Jozsi, the ex-designer, after 56hours of non-stop image-editing unexpectedly ju
st broke down, left his office, and went to live in the mountains. He tried to l
ive in peace with the nature, far from the civilization, but he couldn\'t defeat 
a voice in his head: REVENGE!
So now he\'s here...',

} );

}

LoadJozsi();

return 1;

