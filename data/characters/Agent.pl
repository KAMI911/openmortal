# package Agent;


require 'DataHelper.pl';
use strict;

sub LoadAgent
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Agent';

@Frames = LoadFrames( "$codename.dat.txt", 245, 70 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
'stand', 1,
'dummy', 3,
'start', 7,
'walk', 10,
'turn', 9,
'falling', 9,
'laying', 1,
'hurt', 6,
'won', 23,
'block', 4,
'kneeling', 3,
'onknees', 1,
'kneelingpunch', 4,
'kneelingkick', 6,
'kneelingpunched', 6,
'kneelingkicked', 4,
'fun', 11,
'funb', 6,
'threat', 7,
'highpunch', 8,
'lowpunch', 8,
'highkick', 4,
'lowkick', 3,
'groinkick', 4,
'kneekick', 8,
'elbow', 6,
'spin', 13,
'sweep', 9,
'grenade', 6,
'uppercut', 8,
'throw', 14,
'highpunched', 5,
'lowpunched', 9,
'groinkicked', 6,
'swept',3,
'thrown', 14,
'killkenny', 16,
'specpunch', 7
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
'lpunchFF'=>'KillKenny',
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

{ 'N'=>'Start',			'DEL'=>5,	'S'=>'+start,-start' },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'+fun,-fun', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Funb',			'DEL'=>8,	'S'=>'+funb,-funb', 'CON'=>$Con, 'SITU'=>'Ready' },
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
{ 'N'=>'HighPunch',		'DEL'=>7,	'S'=>'+highpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>7,	'S'=>'+lowpunch',
	'HIT'=>'Hit' },
{ 'N'=>'HighKick',		'DEL'=>7,	'S'=>'+highkick,_highkick',
	'HIT'=>'Hit' },
{ 'N'=>'LowKick',		'DEL'=>7,	'S'=>'+lowkick,_lowkick',
	'HIT'=>'Leghit' },
{ 'N'=>'GroinKick',		'DEL'=>7,	'S'=>'+groinkick,-groinkick',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>6,	'S'=>'+kneekick',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow,-elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>5,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade,-grenade',
	'DEL12'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>6,	'S'=>'+uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },

{ 'N'=>'KillKenny',		'DEL'=>5,	'S'=>'+killkenny', 
	'HIT'=>'Highhit', },
{ 'N'=>'SpecPunch',		'DEL'=>6,	'S'=>'+specpunch,-specpunch',
	'HIT'=>'Uppercut', 'CODE7'=>'::AddEarthquake(25);' },

# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'+falling',
	'DEL16'=>7, 'DEL17'=>7, 'DEL18'=>7, 'DEL19'=>7, 'DEL20'=>7, 'DEL21'=>7,
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'laying n',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>5,	'S'=>'_falling',
	'SITU'=>'Falling',
	'CON8'=>{'down'=>'Onknees'},
	'CON8'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>8,	'S'=>'+swept,-swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked,_kneelingkicked',
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
	'ID'			=> 100,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 4542272,
	'GAMEVERSION'	=> 0.4,
	
	'NAME'		=>'Agent Boogie',
	'TEAM'		=>'Schonherz',
	'STYLE'		=>'Neo-bang-fu',
	'AGE'		=>'22',
	'WEIGHT'	=>'80kg',
	'HEIGHT'	=>'180cm',
	'SHOE'		=>'44',
	'STORY'		=>
'Once upon a time, there was a competition named after the infamous hostel Schonherz. In this competition 
several hundred different tasks are sent out to be solved by a similar number of students. Creating this 
character was part of such a task.',
	
	'NAME.hu'	=>'Agent Krampusz',
	'TEAM.hu'	=>'Schonherz',
	'STYLE.hu'	=>'Neo-puff-fu',
	'AGE.hu'	=>'22',
	'WEIGHT.hu'	=>'80kg',
	'HEIGHT.hu'	=>'180cm',
	'SHOE.hu'	=>'44',
	'STORY.hu'	=>
'Once upon a time, there was a competition named after the infamous hostel Schonherz. In this competition 
several hundred different tasks are sent out to be solved by a similar number of students. Creating this 
character was part of such a task.',
} );

}

LoadAgent();

return 1;

