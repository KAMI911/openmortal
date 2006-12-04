# package Tejszin;


require 'DataHelper.pl';
use strict;

sub LoadTejszin
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Tejszin';

@Frames = LoadFrames( "$codename.dat.txt", 360, 200 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
"start", 23,
"stand", 2,
"walk", 8,
"turn", 6,
"falling", 8,
"laying", 1,
"getup", 7,
"hurt", 8,
"won", 38, 'dummyw', 8,
"block", 6, 'dummybl', 5,
"kneeling", 4,
"onknees", 2,
"standup", 4,
"kneelingpunch", 4, 'dummyknp', 4,
"kneelingkick", 5, 'dummyknk', 6,
"kneelingpunched", 5,
"kneelingkicked", 7,
"fun", 15,
"threat", 23,
"highpunch", 8,
"eelbow", 6,
"lowpunch", 7,
"highkick", 9,
"lowkick", 8,
"groinkick", 6,
"kneekick", 6,
"elbow", 4, 'dummyel', 2,
"spin", 11,
"sweep", 10,
"grenade", 14,
"uppercut", 7,
"throw", 12,
"highpunched", 6,
"lowpunched", 6,
"groinkicked", 7,
"swept", 6,
"thrown", 7,
"throwngetup", 9,
"specpunch", 16,
"thrust", 11,

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
'lpunchFF'=>'Thrust',
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
{ 'N'=>'Hurt',			'DEL'=>10,	'S'=>'+hurt' },
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
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch'} ),

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
{ 'N'=>'GroinKick',		'DEL'=>8,	'S'=>'+groinkick',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>7,	'S'=>'+kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>7,	'S'=>'+elbow,-elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade',
	'DEL12'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>6,	'S'=>'+uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'SpecPunch',		'DEL'=>6,	'S'=>'+specpunch',
	'HIT'=>'Hit', },
{ 'N'=>'Thrust',		'DEL'=>6,	'S'=>'+thrust',
	'HIT'=>'Uppercut',	'MOVE'=>3, },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>10,	'S'=>'+falling',
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'laying n',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>8,	'S'=>'_falling',
	'SITU'=>'Falling',
	'CON4'=>{'down'=>'Onknees'},
	'CON5'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>8,	'S'=>'+swept,-swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>8,	'S'=>'+kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>7,	'S'=>'+kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'highpunched n-2, +highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched' },
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
	'ID'			=> 101,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 9469281,
	
        'NAME'  =>'[10g] Tej-szin',
        'TEAM'  =>'[10g]',
        'STYLE' =>'ultrabrutal',
        'AGE'   =>'160',
        'WEIGHT'=>'130kg',
        'HEIGHT'=>'180cm',
        'SHOE'  =>'45',
        'STORY' =>
'long,long, time ago....:)',
        
        'EMAIL' =>'lameron@sch.bme.hu',
	
} );

}

LoadTejszin();

return 1;

