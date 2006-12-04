# package Zoli;

# Zoli's images are scaled:
# First 47 images: 0.75
# Rest of the images: 0.78 (strange zoomout effect)

require 'DataHelper.pl';

# @ISA = qw(Fighter);

@ZoliFrames = LoadFrames( "Zoli.dat.txt", 340, 200 );
%ZoliFrame = CreateFrameLookup( scalar @ZoliFrames - 1,
"start",		21,		"stand",		4,		"walk",			13,
"turn",			9,		"falling",		11,		"laying",		1,
"getup",		14,		"hurt",			16,		"swept",		6,
"won",			11,		"block",		5,		"kneeling",		4,
"onknees",		1,		"kneelingpunch",7,		"kneelingkick",	4,
"fun",			7,		"threat",		9,		"highpunch",	5,
"lowpunch",		6,		"highkick",		8,		"lowkick",		6,
"groinkick",	6,		"kneekick",		6,		"elbow",		5,
"spin",			11,		"grenade",		15,		"uppercut",		9,
"throw",		16,		"highpunched",	9,		"lowpunched",	4,
"groinkicked",	13,		"kneelingpunched",4,	"kneelingkicked", 3,
"thrown",		4,		"specpunch",	13,
);


=comment
$ZoliDoodad =
	{	'T'		=> 1,
		'W'		=> 64,
		'H'		=> 64,
		'SX'	=> 15,
		'SY'	=> 0,
		'FRAMES'=> 6,
		'SA'	=> 1/25,
	};
=cut

$ZCon = {
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
'lkickFB'=>'GroinKick',
'lkickF'=>'KneeKick',
'hpunchF'=>'Elbow',
'hpunchB'=>'SpinPunch',
'lpunchBD'=>'Grenade',
'hpunchD'=>'Uppercut',
'hpunchFF'=>'SpecPunch',
};




@ZoliStates1 = (
{ 'N'=>'Start',			'DEL'=>5,	'S'=>'+start' },
{ 'N'=>'Stand',			'DEL'=>10,	'S'=>'+stand', 'CON'=>$ZCon, 'SITU'=>'Ready' },
{ 'N'=>'Walk',			'DEL'=>5,	'S'=>'+walk, walk 3-1', 'CON'=>$ZCon },
{ 'N'=>'Back',			'DEL'=>5,	'S'=>'walk 1-3,walk 13-1', 'CON'=>$ZCon },
{ 'N'=>'Turn',			'DEL'=>5,	'S'=>'+turn',
						'TURNN'=>1, },
{ 'N'=>'Falling',		'DEL'=>5,	'S'=>'+falling',
	'DEL11'=>500, 'NEXT11'=>'Laying', 'SITU'=>'Falling' },	#, 'DELTAX'=>-20, },
{ 'N'=>'Laying',		'DEL'=>1000,'S'=>'+laying',
	'SITU'=>'Falling' },
{ 'N'=>'Getup',			'DEL'=>5,	'S'=>'+getup',
	'SITU'=>'Falling',										# 'DELTAX'=>11,
	'CON8'=>{'down'=>'Kneeling 5'},
	'CON8'=>{'down'=>'Kneeling 5'},
},
{ 'N'=>'Dead',			'DEL'=>10000, 'S'=>'laying 1',
	'SITU'=>'Falling' },

{ 'N'=>'Hurt',			'DEL'=>8,	'S'=>'+hurt' },
{ 'N'=>'Swept',			'DEL'=>5,	'S'=>'+swept,-swept' },
{ 'N'=>'Won',			'DEL'=>8,	'S'=>'+won',
	'NEXTST11'=> 'Won2' },
{ 'N'=>'Won2',			'DEL'=>1000,'S'=>'won 11',
	'NEXTST'=>'Won2',
},
BlockStates( FindLastFrame( \%ZoliFrame, 'block' ), 5 ),
{ 'N'=>'Kneeling',		'DEL'=>5,	'S'=>'+kneeling,+onknees,-kneeling',
	'SITU'=>'Crouch',
	'NEXTST1'=>'Stand',			'CON1'=>{"down"=>"Kneeling 2"},
	'NEXTST2'=>'Kneeling 1',	'CON2'=>{"down"=>"Kneeling 3"},
	'NEXTST3'=>'Kneeling 2',	'CON3'=>{"down"=>"Kneeling 4"},
	'NEXTST4'=>'Kneeling 3',	'CON4'=>{"down"=>"Kneeling 5"},
	'NEXTST5'=>'Kneeling 4',	'CON5'=>{"down"=>"Kneeling 5",
	'lkick'=>'KneelingKick', 'hkick'=>'KneelingKick', 'hpunch'=>'Uppercut 6', 'lpunch'=>'KneelingPunch' },
	'CON6'=>{"down"=>"Kneeling 5"},
	'CON7'=>{"down"=>"Kneeling 6"},
	'CON8'=>{"down"=>"Kneeling 7"},
	'CON9'=>{"down"=>"Kneeling 8"},
},	# Should be much lighter
{ 'N'=>'KneelingPunch',	'DEL'=>5,	'S'=>'+kneelingpunch,-kneelingpunch',
	'SITU'=>'Crouch',
	'HIT7'=>'Hit', 'NEXTST14'=>'Kneeling 5' },
{ 'N'=>'KneelingKick',	'DEL'=>8,	'S'=>'+kneelingkick,-kneelingkick',
	'SITU'=>'Crouch',
	'HIT4'=>'Hit', 'NEXTST8'=>'Kneeling 5' },
{ 'N'=>'KneelingPunched', 'DEL'=>5,	'S'=>'+kneelingpunched,-kneelingpunched',
	'SITU'=>'Crouch', 'NEXTST8'=>'Kneeling 5' },
{ 'N'=>'KneelingKicked', 'DEL'=>5,	'S'=>'+kneelingkicked,-kneelingkicked',
	'SITU'=>'Crouch', 'NEXTST6'=>'Kneeling 5' },
{ 'N'=>'KneelingKicked','DEL'=>5,	'S'=>'+kneelingkicked,-kneelingkicked' },

JumpStates( \%ZoliFrame,
	{'lkick'=>'JumpKick', 'hkick'=>'JumpKick',
	'lpunch'=>'JumpPunch', 'hpunch'=>'JumpPunch'} ),


{ 'N'=>'Fun',			'DEL'=>8,	'S'=>'fun 7-6,+fun', 'CON'=>$ZCon, 'SITU'=>'Ready' },
{ 'N'=>'Threat',		'DEL'=>8,	'S'=>'threat 1-7', 'CON'=>$ZCon, 'SITU'=>'Ready' },		# Threat 8-9 are bad.
{ 'N'=>'HighPunch',		'DEL'=>5,	'S'=>'+highpunch,-highpunch',
	'HIT5'=>'Highhit' },
{ 'N'=>'LowPunch',		'DEL'=>5,	'S'=>'+lowpunch,-lowpunch',
	'HIT6'=>'Hit' },
{ 'N'=>'HighKick',		'DEL'=>5,	'S'=>'+highkick,-highkick',
	'HIT8'=>'Hit' },
{ 'N'=>'LowKick',		'DEL'=>5,	'S'=>'+lowkick,-lowkick',
	'HIT6'=>'Leghit' },
{ 'N'=>'GroinKick',		'DEL'=>5,	'S'=>'+groinkick,-groinkick',
	'HIT6'=>'Groinhit' },
{ 'N'=>'KneeKick',		'DEL'=>5,	'S'=>'+kneekick,-kneekick',
	'HIT6'=>'Hit' },
{ 'N'=>'Elbow',			'DEL'=>5,	'S'=>'+elbow,-elbow',
	'HIT5'=>'Highhit' },
{ 'N'=>'SpinPunch',		'DEL'=>4,	'S'=>'+spin,-spin',
	'HIT11'=>'Highhit' },
{ 'N'=>'Grenade',		'DEL'=>5,	'S'=>'+grenade',
	'DEL12'=>15, 'DOODAD12'=>'ZoliShot' },
{ 'N'=>'Uppercut',		'DEL'=>5,	'S'=>'+uppercut,-uppercut',
	'HIT9'=>'Uppercut' },
{ 'N'=>'Throw',			'DEL'=>8,	'S'=>'+throw' },
{ 'N'=>'HighPunched',	'DEL'=>8,	'S'=>'+highpunched' },
{ 'N'=>'LowPunched',	'DEL'=>5,	'S'=>'+lowpunched,-lowpunched' },
{ 'N'=>'GroinKicked',	'DEL'=>7,	'S'=>'+groinkicked' },
{ 'N'=>'Thrown',		'DEL'=>5,	'S'=>'+thrown' },
{ 'N'=>'SpecPunch',		'DEL'=>5,	'S'=>'+specpunch', 'DEL8'=>15, #'DELTAX13'=>74,
	'HIT8'=>'Uppercut' },
);



# 2. CREATE STATES: %ZoliStates

foreach $framedesc (@ZoliStates1)
{
	AddStates( \%ZoliStates, \%ZoliFrame, $framedesc );
}

#$ZoliStates{'Standby'} =
#{ 'F'=>'stand1',	'DEL'=> 25,	'CON'=> $KCon, 'NEXTST'=>'JumpFW 1' };



# Automatically add STUFF to Walk and Back

for ( $i=4; $i<=13; ++$i )
{
	$j = $ZoliFrame{"walk$i"};
	OffsetFrame( $ZoliFrames[$j], -($i-4)*16, 0 );
}

#for ( $i=1; $i<=11; ++$i )
#{
#	$j = $ZoliFrame{"falling$i"};
#	OffsetFrame( $ZoliFrames[$j], $i*18, 0 );
#}

#for ( $i=1; $i<=14; ++$i )
#{
#	$j = $ZoliFrame{"getup$i"};
#	OffsetFrame( $ZoliFrames[$j], 198 - $i*11, 0 );
#}
#$j = $ZoliFrame{"laying1"};
#OffsetFrame( $ZoliFrames[$j], 198, 0 );

for ( $i=1; $i<=3; ++$i )
{
	$j = $i+1;
	$ZoliStates{"Walk $i"}->{NEXTST} = "Stand";
	$ZoliStates{"Walk $i"}->{CON} = { %{$ZCon}, 'forw'=>"Walk $j" };
	$ZoliStates{"Back $i"}->{NEXTST} = "Stand";
	$ZoliStates{"Back $i"}->{CON} = { %{$ZCon}, 'back'=>"Back $j" };
}

for ( $i=4; $i<=13; ++$i )
{
	$j = $i+1;
	$j = 4 if $j == 14;
	$ZoliStates{"Walk $i"}->{MOVE} = 4;
	$ZoliStates{"Walk $i"}->{NEXTST} = "Walk 14";
	$ZoliStates{"Walk $i"}->{CON} = { %{$ZCon}, 'forw'=>"Walk $j" };
	$ZoliStates{"Back $i"}->{MOVE} = -4;
	$ZoliStates{"Back $i"}->{NEXTST} = "Back 14";
	$ZoliStates{"Back $i"}->{CON} = { %{$ZCon}, 'back'=>"Back $j" };
}


# Automatically add NEXTST for states which don't have one.

while (($key, $value) = each %ZoliStates) {
	$framename = $value->{'F'};
	unless ( $framename =~/^\d+$/ )
	{
		# Convert non-numeric frames to their numeric counterparts.
		die "Can't find image $framename in frame $key" unless defined $ZoliFrame{ $framename };
		$value->{'F'} = $ZoliFrame{ $value->{'F'} };
	}
	
	($st,$lastchar) = $key =~ /(\w+)\s+(\d+)/;
	
	unless ( defined $value->{'NEXTST'} )
	{
		$nextchar = $lastchar + 1;
		$nextst = "$st $nextchar";
		unless ( defined $ZoliStates{$nextst} ) {
			# print "Go to Standby after $key\n";
			$nextst = 'Stand';
		}
		$value->{'NEXTST'} = $nextst;
	}

	print "$key has no lastchar" unless defined $lastchar;
	if ( $lastchar == 1 )
	{
		$ZoliShorthands{$st} = $ZoliStates{$key};
	}
}


TravelingStates( \%ZoliFrame, \@ZoliFrames, \%ZoliStates, "falling", 1, 12 );
TravelingStates( \%ZoliFrame, \@ZoliFrames, \%ZoliStates, "getup", 1, 14 );
TravelingStates( \%ZoliFrame, \@ZoliFrames, \%ZoliStates, "specpunch", 0, 0 );
#$frameLookup, $frameArray, $states, $frameName, $from, $to

%ZoliStates = ( %ZoliShorthands, %ZoliStates );

::RegisterFighter( {
	'ID'			=> 3,
	'GENDER'		=> 1,
	'DATAVERSION'	=> 1,
	'STARTCODE'		=> sub {},
	'FRAMES'		=> \@ZoliFrames,
	'STATES'		=> \%ZoliStates,
	'CODENAME'		=> 'Zoli',
	'DATASIZE'		=> 4486206,
} );


return 1;

