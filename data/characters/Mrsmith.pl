# package Mrsmith;


require 'DataHelper.pl';
use strict;

sub LoadMrsmith
{
	my ( @Frames, %FrameLookup, %Frames, %States, %Shorthands,
		$Doodad, $Con, $Con2,
		@States1,
		$framedesc, $key, $value, $framename, $nextst, $st, $lastchar, $nextchar, $codename );

$codename = 'Mrsmith';

@Frames = LoadFrames( "$codename.dat.txt", 150, 70 );
%FrameLookup = CreateFrameLookup( scalar @Frames -1,
'start', 10,
'stand', 2,
'walk', 6,
'turn', 7,
'falling', 6,
'laying', 1,
'getup', 12,
'hurt', 5,
'won', 25,
'block', 4, 'dummyc', 2,
'dummya', 4, 'kneeling', 4, 'dummyb', 1,
'onknees', 3,
'kneelingpunch', 4, 'dummyknp', 2,
'kneelingkick', 4, 'dummyknk', 4,
'kneelingpunched', 6,
'kneelingkicked', 8,
'fun', 27,
'threat', 7,
'highpunch', 5,
'lowpunch', 9,
'highkick', 8,
'lowkick', 7,
'groinkick', 6,
'kneekick', 5,
'elbow', 4,
'spin', 7,
'sweep', 8,
'grenade', 9,
'uppercut', 11,
'throw', 13,
'highpunched', 9,
'lowpunched', 7,
'groinkicked', 7,
'swept', 12,
'thrown', 7,
'speciala', 7,
'specialb', 7,
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

{ 'N'=>'Start',			'DEL'=>7,	'S'=>'+start', 'DEL6'=>12, },
{ 'N'=>'Stand',			'DEL'=>7,	'S'=>'+stand,-stand', 'CON'=>$Con, 'SITU'=>'Ready' },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',	'TURNN'=>1, },
{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt,-hurt' },
{ 'N'=>'Won',			'DEL'=>10,	'S'=>'+won',	'NEXTSTN'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won n',	'NEXTST'=>'Won2', },
{ 'N'=>'Fun',			'DEL'=>5,	'S'=>'+fun', 'CON'=>$Con, 'DEL20'=> 16, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>7,	'S'=>'+threat,-threat', 'CON'=>$Con, 'SITU'=>'Ready' },
WalkingFrames( \%FrameLookup, \@Frames, 0, 8, $Con ),
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
{ 'N'=>'HighPunch',		'DEL'=>12,	'S'=>'+highpunch',
	'HIT'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>7,	'S'=>'+lowpunch',
	'HIT'=>'Hit' },
{ 'N'=>'HighKick',		'DEL'=>7,	'S'=>'highkick 1-5, highkick 5-1',
	'HIT'=>'Hit' },
{ 'N'=>'LowKick',		'DEL'=>7,	'S'=>'lowkick 1-4, lowkick 4-1',
	'HIT'=>'Leghit' },
{ 'N'=>'GroinKick',		'DEL'=>10,	'S'=>'+groinkick,-groinkick', 'DEL3'=>'12',
	'HIT'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>8,	'S'=>'kneekick 5-3, kneekick 3-5',
	'HIT'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>8,	'S'=>'+elbow',
	'HIT'=>'Highhit' },
{ 'N'=>'Spin',			'DEL'=>8,	'S'=>'+spin',
	'HIT'=>'Hit' },
{ 'N'=>'Sweep',			'DEL'=>7,	'S'=>'+sweep',
	'HIT'=>'Hit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade,-grenade',
	'DEL12'=>15, 'DOODAD'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>7,	'S'=>'+uppercut',
	'HIT'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },


#{ 'N'=>'KillKenny',		'DEL'=>5,	'S'=>'+killkenny', },
#{ 'N'=>'SpecPunch',		'DEL'=>6,	'S'=>'+specpunch,-specpunch',
#	'HIT'=>'Uppercut', },


# 3. HURT MOVES

{ 'N'=>'Falling',		'DEL'=>12,	'S'=>'+falling, falling 5',
	'DELN'=>500, 'NEXTN'=>'Laying', 'SITU'=>'Falling', },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'laying n',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>7,	'S'=>'+getup',
	'SITU'=>'Falling',
	'CON5'=>{'down'=>'Onknees'},
	'CON6'=>{'down'=>'Onknees'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling', 'NEXTST'=>'Dead' },

{ 'N'=>'Swept',			'DEL'=>8,	'S'=>'+swept' },

{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked',
	'SITU'=>'Crouch', 'NEXTSTN'=>'Onknees' },

{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>8,	'S'=>'+groinkicked', 'DEL3'=>16 },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },

{ 'N'=>'SpecPunch',		'DEL'=>7,	'S'=>'+specialb,-specialb',
	'HIT'=>'Uppercut', }
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
	'ID'			=> 103,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@Frames,
	'STATES'		=> \%States,
	'CODENAME'		=> $codename,
	'DATASIZE'		=> 6259470,
	
	    'NAME'  =>'Mr. Smith',
        'TEAM'  =>'Spontán',
        'STYLE' =>'beer-fu',
        'AGE'   =>'23',
        'WEIGHT'=>'80kg',
        'HEIGHT'=>'185cm',
        'SHOE'  =>'44',
        'STORY' =>
'Mr. Smith comes from the shadows, where he and his evil, disgraced family of de
mons resided for such a dementing long time. Now, as the last member of his inge
nious breed, Mr. Smith takes on the tournament to reclaim every of all possessio
ns his family lost under the dark ages of The Returning Of The Good Wizards. Uns
toppable and strong enough to beat a Black Dragon by his own hands, Mr. Smith se
eks trouble wherever he thinks there could be one.',
        
        'EMAIL' =>'zaphir@sch.bme.hu',

} );

}

LoadMrsmith();

return 1;

