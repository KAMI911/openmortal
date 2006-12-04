# package Sleepy;


require 'DataHelper.pl';
use strict;

sub LoadSleepy
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Sleepy';

@Frames = LoadFrames( "$codename.dat.txt", 245, 50 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
'start', 21,
'stand', 4,
'walk', 13,
'turn', 8,
'falling', 18,
'laying', 1,
'getup', 21,
'funb', 12,
'hurt', 10,
'won', 16,
'block', 6,
'kneeling', 3, 'onknees', 1,
'kneelingpunch', 4,
'kneelingkick', 5,
'kneelingpunched', 5,
'kneelingkicked', 4,
'fun', 8,
'threat', 9,
'highpunch', 14,
'lowpunch', 10,
'highkick', 7,
'lowkick', 6,
'groinkick', 9,
'kneekick', 14,
'elbow', 19,
'spin', 18,
'sweep', 13,
'uppercut', 17,
'grenade', 14,
'highpunched', 11,
'lowpunched', 5,
'throw', 16,
'swept', 6,
'groinkicked', 11,
'thrown', 27,
'specpunch', 13,
'pillow', 20,
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
'lkickB'=>'Sweep',

'lpunchBD'=>'Grenade',
'lpunchFF'=>'Pillow',
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

{ 'N'=>'Start',			'DEL'=>5,	'S'=>'+start' },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,-stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Funb',			'DEL'=>8,	'S'=>'+funb', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>5,	'S'=>'+threat,-threat', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 0, 110, $Con ),
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
{ 'N'=>'KneelingUppercut','DEL'=>5,	'S'=>'kneeling 3-2,uppercut 2-n',
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
{ 'N'=>'KneeKick',		'DEL'=>5,	'S'=>'+kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade,_grenade',
	'DEL12'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'SpecPunch',		'DEL'=>5,	'S'=>'+specpunch,-specpunch',
	'HIT'=>'Uppercut',	
	'MOVE8'=>1, 'MOVE9'=>2, 'MOVE10'=>3, 'MOVE11'=>3, 'MOVE12'=>3, 'MOVE13'=>3, 'PUSH14'=>10, },
{ 'N'=>'Pillow',		'DEL'=>5,	'S'=>'+pillow',
	'HIT'=>'Hit' },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'+falling',
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'laying n',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>5,	'S'=>'+getup',
	'SITU'=>'Falling',
	'CON11'=>{'down'=>'Onknees'},
	'CON12'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>8,	'S'=>'+swept,-swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked,-kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>4,	'S'=>'+highpunched, -highpunched' },
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

TravelingStates( \%FrameLookup, \@Frames, \%States, "falling", 0, 0 );
TravelingStates( \%FrameLookup, \@Frames, \%States, "won", 0, 0 );

%States = ( FindShorthands( \%States ), %States );

# %::UPiStates = %States;
# @::UPiFrames = @Frames;

::RegisterFighter( {
	'ID'			=> 105,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 6809466,
        'NAME'  =>'Sleepy',
        'TEAM'  =>'Mindegy',
        'STYLE' =>'sleep-fu',
        'AGE'   =>'25',
        'WEIGHT'=>'80kg',
        'HEIGHT'=>'185cm',
        'SHOE'  =>'Atlagmamusz',
        'STORY' =>
'az most nincs',
        
        'EMAIL' =>'orca@wap.hu',
} );

}

LoadSleepy();

return 1;

