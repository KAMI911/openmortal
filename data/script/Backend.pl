#!/usr/bin/perl -w

# PERL backend for the game

=comment

This is the main perl backend for Mortal Szombat. It loads the other files,
compiles characters, creates shared variables.



1. FIGHTER ACTIONS

1.1. BASIC MOVEMENT        1.2. OFFENSIVE MOVES         1.3. HURT MOVES

Start                      KneelingPunch                Falling
Stand                      KneelingKick                 Laying
Walk                       KneelingUppercut             Getup
Back                       HighPunch                    HighPunched
Turn                       LowPunch                     LowPunched
Hurt                       HighKick                     GroinKicked
Block                      LowKick                      KneelingPunched
Kneeling                   Sweep                        KneelingKicked
Onknees                    GroinKick                    Thrown
Jump                       KneeKick                     Dead
JumpFW                     Elbow
JumpBW                     Grenade
Fun                        Uppercut
Threat                     Throw
                           [fighter-specific stuff]
                           
=cut


=comment

MAIN CONSTANTS

=cut

$GAMEBITS	= 3;							# Number of oversampling bits in coordinates
$GAMEBITS2	= 1 << $GAMEBITS;				# 2^(oversampling)
$SCRWIDTH	= 640;							# The physical horizontal screen resolution
$SCRHEIGHT	= 480;							# The physical vertical screen resolution
$SCRWIDTH2	= ($SCRWIDTH << $GAMEBITS);		# The logical horizontal screen resolution
$SCRHEIGHT2	= ($SCRHEIGHT << $GAMEBITS);	# The logical vertical screen resolution
$MOVEMARGIN2= 50 << $GAMEBITS;				# The logical border for fighters.

$BGWIDTH2	= 1920 << $GAMEBITS;			# The logical background width
$BGHEIGHT2	= 480 << $GAMEBITS;				# The logical background height

$GROUND2	= 160 << $GAMEBITS;				# The logical ground level.
$DELMULTIPLIER = 1;							# DEL values in states are multiplied by this.
$MAXCOMBO	= 5;							# Maximum combo count.

$BgMax		= $BGWIDTH2 - $SCRWIDTH2;		# The logical maximum camera position
$BgSpeed	= 0;							# The current speed of the background movement
$BgPosition	= $BgMax >> 1;					# The logical camera position
$BgScrollEnabled = 1;						# Can the background scroll?
$HitPointScale = 10;						# Scale the hit points by this.
$NextDoodad = 0;							# The next doodad to process
$Debug		= 0;


require 'PlayerInput.pl';
require 'Fighter.pl';
require 'FighterStats.pl';
require 'Doodad.pl';
require 'Keys.pl';
require 'State.pl';
require 'Translate.pl';
require 'Rewind.pl';



=comment

MAIN OBJECTS

=cut


$Fighter1 = Fighter->new();
$Fighter2 = Fighter->new();

$Fighter1->{NUMBER} = 0;
$Fighter1->{OTHER} = $Fighter2;

$Fighter2->{NUMBER} = 1;
$Fighter2->{OTHER} = $Fighter1;

@Fighters = ( $Fighter1, $Fighter2 );


=comment

VARIABLES FOR DRAWING THE SCENE

=cut

$gametick = 0;	# Number of Advance()'s called since the start.
$p1x = 10;		# Player 1 X position
$p1y = 100;		# Player 1 Y position
$p1f = 10;		# Player 1 frame
$p1h = 100;		# Player 1 HP

$p2x = 400;		# Player 2 X position
$p2y = 100;		# Player 2 Y position
$p2f = -40;		# Player 2 frame. Negative means flipped.
$p2h = 100;		# Player 2 HP

$bgx = 0;		# Background X position
$bgy = 0;		# Background Y position
$over = 0;		# Is the game over?
$ko = 0;		# Is one fighter knocked down?


$doodad_x = -1;			# Doodad X position
$doodad_y = -1;			# Doodad Y position
$doodad_t = -1;			# Doodad type
$doodad_f = -1;			# Doodad frame number
$doodad_dir = 0;		# The direction of the doodad; 1: normal; -1: flipped
$doodad_gfxowner = -1;	# 0: first player; 1: second player; 2: Global doodad
$doodad_text = '';		# The text of type 0 doodads.


sub ResetGame($)
{
	my ($bgposition) = @_;
	$gametick		= 0;
	$over			= 0;
	$ko				= 0;
	
	$bgx			= 0;
	$bgy			= 0;
	$BgSpeed		= 0;
	$BgPosition		= $bgposition;
	$BgScrollEnabled = $bgposition != 0;
	
	$OverTimer		= 0;
	$JudgementMode	= 0;
	$Debug			= 0;
	
	@Doodads = ();
	@Sounds = ();
	
	ResetEarthquake();
	ResetRewind();

	$Fighter1->Reset() if $Fighter1->{OK};
	$Fighter2->Reset() if $Fighter2->{OK};
	$Input1->Reset();
	$Input2->Reset();
}


=comment

JUDGEMENT METHODS

=cut


sub JudgementStart($)
{
	ResetGame(0);
	# $bgy = ( $SCRHEIGHT2 - $BGHEIGHT2 ) >> 1;
	$JudgementMode = 1;
	($JudgementWinner) = @_;
	
	$Fighter1->{X} =  ($JudgementWinner ? 150 : 520 ) * $GAMEBITS2;
	$Fighter1->{DIR} = ($JudgementWinner ? 1 : -1 );
	$Fighter1->{NEXTST} = 'Stand';
	$Fighter1->Update();
	
	$Fighter2->{X} = ($JudgementWinner ? 520 : 150 ) * $GAMEBITS2;
	$Fighter2->{DIR} = ($JudgementWinner ? -1 : 1 );
	$Fighter2->{NEXTST} = 'Stand';
	$Fighter2->Update();
}


=comment

PLAYER SELECTION METHODS 

=cut

sub SelectStart
{
	ResetGame(0);
	
	if ( $Fighter1->{OK} )
	{
		$Fighter1->{X} = 80 * $GAMEBITS2;
		$Fighter1->{NEXTST} = 'Stand';
		$Fighter1->Update();
	}
	if ( $Fighter2->{OK} )
	{
		$Fighter2->{X} = 560 * $GAMEBITS2;
		$Fighter2->{NEXTST} = 'Stand';
		$Fighter2->Update();
	}
}


sub SetPlayerNumber
{
	my ($player, $fighterenum) = @_;
	my ($f);
	
	DeleteDoodadsOf( $player );
	
	$f = $player ? $Fighter2 : $Fighter1;
	
	$f->Reset($fighterenum);
	
	$f->{NEXTST} = 'Stand';
	$f->Update();
	
	GetFighterStats($fighterenum);
	$::PlayerName = $::Name
}


sub PlayerSelected
{
	my ($number) = @_;
	my ($f) = $number ? $Fighter2 : $Fighter1;
	
	$f->Event( 'Won' );
	$f->Update();
}



=comment

EARTHQUAKE RELATED METHODS

=cut



@QUAKEOFFSET = ( 0,  6, 11, 15, 
				16, 15, 11,  6, 
				 0, -6,-11,-15,
			   -16,-15,-11, -6, 0, 6 );


sub ResetEarthquake
{
	$QuakeAmplitude = 0;
	$QuakeOffset = 0;
	$QuakeX = 0;
	$QuakeY = 0;
}


sub AddEarthquake
{
	my ($amplitude) = @_;
	$QuakeAmplitude += $amplitude;
	$QuakeAmplitude = 20 if ( $QuakeAmplitude > 20 );
}


sub AdvanceEarthquake
{
	if ( $QuakeAmplitude <= 0.2 )
	{
		$QuakeAmplitude = $QuakeX = $QuakeY = 0;
		return;
	}
	
	$QuakeAmplitude -= $QuakeAmplitude / 30 + 0.1;
	$QuakeOffset = ( $QuakeOffset + 1 ) % 16;
	$QuakeX = $QUAKEOFFSET[$QuakeOffset] * $QuakeAmplitude / 16;
	$QuakeY = $QUAKEOFFSET[$QuakeOffset + 1] * $QuakeAmplitude / 16;	# 1/1
	
	$bgx -= $QuakeX;
	$bgy -= $QuakeY;
	$p1x += $QuakeX;
	$p1y += $QuakeY;
	$p2x += $QuakeX;
	$p2y += $QuakeY;
	
	# Do not quake doodads for now.
}



=comment

GAME BACKEND METHODS

=cut

sub GameStart($$)
{
	my ( $MaxHP, $debug ) = @_;

	ResetGame( $BgMax >> 1);
	
	$bgx = ( $SCRWIDTH2 - $BGWIDTH2) >> 1;
	$bgy = ( $SCRHEIGHT2 - $BGHEIGHT2 ) >> 1;
	$BgScrollEnabled = 1;
	$HitPointScale = 1000 / $MaxHP;			# 1/1
	$Debug = $debug;

	$Fighter1->{HP} = $MaxHP;
	$Fighter2->{HP} = $MaxHP;
	
	$p1h = $p2h = 0;
}



=comment

Takes a Fighter object, and returns the data relevant for the frontend.

Parameters:
$fighter	The fighter which is to be converted to frontend data.

Returns:
$x			The physical X coordinate of the fighter
$y			The physical Y coordinate of the fighter
$fnum		The frame number of the fighter. A negative sign means the
			fighter is flipped horizontally (facing left)

=cut

sub GetFighterData($)
{
	my ($fighter) = @_;
	my ($x, $y, $fnum, $f, $hp);
	
	$fnum = $fighter->{FR};
	$f = $fighter->{FRAMES}->[$fnum];
	$x = $fighter->{X} / $GAMEBITS2 + $f->{'x'};
	$y = $fighter->{Y} / $GAMEBITS2 + $f->{'y'};
	
	if ($fighter->{DIR}<0)
	{
		$fnum = -$fnum;
		$x -= $f->{x}*2 + $f->{w};
	}
	
	return ($x, $y, $fnum);
}



sub GetNextDoodadData
{
	if ( $NextDoodad >= scalar @Doodads )
	{
		$doodad_x = $doodad_y = $doodad_t = $doodad_f = $doodad_dir = $doodad_gfxowner = -1;
		$doodad_text = '';
		return;
	}

	my ($doodad) = $Doodads[$NextDoodad];
	$doodad_x = $doodad->{POS}->[0] / $GAMEBITS2 - $bgx;
	$doodad_y = $doodad->{POS}->[1] / $GAMEBITS2 - $bgy;
	$doodad_t = $doodad->{T};
	$doodad_f = $doodad->{F};
	$doodad_dir = $doodad->{DIR};
	$doodad_gfxowner = $doodad->{GFXOWNER};
	$doodad_text = $doodad->{TEXT};
	
	
	if ($doodad_gfxowner < 2 )
	{
		$doodad_x += $Fighters[$doodad_gfxowner]->{FRAMES}->[$doodad_f]->{'x'} * $doodad_dir;
		$doodad_y += $Fighters[$doodad_gfxowner]->{FRAMES}->[$doodad_f]->{'y'};
	}
	
	++$NextDoodad;
}



sub UpdateDoodads
{
	my ($i, $j, $doodad);
	
	for ($i=0; $i<scalar @Doodads; ++$i)
	{
		$doodad = $Doodads[$i];
		$j = Doodad::UpdateDoodad( $doodad );
		if ( $j )
		{
			# Remove this Doodad
			splice @Doodads, $i, 1;
			--$i;
		}
	}
}



sub DeleteDoodadsOf($)
{
	my ($owner) = @_;
	my ($i, $doodad);
	for ($i=0; $i<scalar @Doodads; ++$i)
	{
		$doodad = $Doodads[$i];
		if ( $doodad->{OWNER} == $owner )
		{
			# Remove this Doodad
			splice @Doodads, $i, 1;
			--$i;
		}
	}
}



sub GetNextSoundData()
{
	# print "GetSoundData: ", scalar @Sounds, join ' ,', @Sounds, "\n";
	$sound = pop @Sounds;
	$sound = '' unless defined $sound;
}



sub DoFighterEvents
{
	my ($fighter, $hit) = @_;
	
	if ( $JudgementMode )
	{
		if ( $fighter->{NUMBER} == $JudgementWinner )
		{
			$fighter->Event("Won");
		}
		else
		{
			$fighter->Event("Hurt");
		}
		return;
	}
	
	if ( $hit )
	{
		$fighter->HitEvent( $fighter->{OTHER}->GetCurrentState()->{HIT}, $hit );
		return;
	}
	
	#if ( ($fighter->{X} - $fighter->{OTHER}->{X}) * ($fighter->{DIR}) > 0 )
	if ( $fighter->IsBackTurned )
	{
		$fighter->Event("Turn");
	}
	if ( $fighter->{OTHER}->{ST} eq 'Dead' )
	{
		$fighter->Event("Won");
	}
	if ( $fighter->{IDLE} > 150 )
	{
		my ($r) = $fighter->QuasiRandom( 353 );
		$fighter->Event("Fun") if $r==1;
		$fighter->Event("Threat") if $r==2;
	}
}




sub GameAdvance
{
	my ($hit1, $hit2);
	
	$gametick += 1;
	
	$NextDoodad = 0;
	$NextSound = 0;
	
	# 1. ADVANCE THE PLAYERS
	
	$Input1->Advance();
	$Input2->Advance();
	$Fighter1->Advance( $Input1 ) if $Fighter1->{OK};
	$Fighter2->Advance( $Input2 ) if $Fighter2->{OK};
	$hit2 = $Fighter1->CheckHit() if $Fighter1->{OK};
	$hit1 = $Fighter2->CheckHit() if $Fighter2->{OK};
	
	# 2. Events come here
	
	DoFighterEvents( $Fighter1, $hit1 ) if $Fighter1->{OK};
	DoFighterEvents( $Fighter2, $hit2 ) if $Fighter2->{OK};
	UpdateDoodads();
	$Fighter1->Update() if $Fighter1->{OK};
	$Fighter2->Update() if $Fighter2->{OK};
	
	if ( $OverTimer == 0 and
		($Fighter1->{ST} eq 'Dead' or $Fighter1->{ST} eq 'Won2') and
		($Fighter2->{ST} eq 'Dead' or $Fighter2->{ST} eq 'Won2') )
	{
		$OverTimer = 1;
	}
	elsif ( $OverTimer > 0 )
	{
		$OverTimer++;
		$over = 1 if ( $OverTimer > 200 )
	}
	
	# 3. DO THE BACKGROUND SCROLLING THING

	if ( $BgScrollEnabled )
	{
		$BgOpt = ($Fighter1->{X} + $Fighter2->{X}) / 2;		# 1/1 Stupid kdevelop syntax hightlight.
		if ( ($BgOpt - $BgSpeed*50 - $BgPosition) > (320 << $GAMEBITS)) { $BgSpeed++; }
		if ( ($BgOpt - $BgSpeed*50 - $BgPosition) < (320 << $GAMEBITS)) { $BgSpeed--; }
	
		$BgPosition+=$BgSpeed;
		if ($BgPosition<0) { $BgPosition = $BgSpeed = 0; }
		if ($BgPosition>$BgMax) { $BgPosition = $BgMax; $BgSpeed = 0; }
		
		# print "Pos:$BgPosition\tOpt:$BgOpt\tSpd:$BgSpeed\t";
	}
	
	# 4. SET GLOBAL VARIABLES FOR THE C++ DRAWING TO READ
	
	($p1x, $p1y, $p1f) = GetFighterData( $Fighter1 );
	($p2x, $p2y, $p2f) = GetFighterData( $Fighter2 );

	$phTarget = $Fighter1->{HP}*$HitPointScale;
	if ( $p1h < $phTarget ) { $p1h += 5; }
	if ( $p1h > $phTarget ) { $p1h -= 3; }
	$p1h = $phTarget if abs($p1h - $phTarget) < 3;
	$p1h = 0 if $p1h < 0;
	
	$phTarget = $Fighter2->{HP}*$HitPointScale;
	if ( $p2h < $phTarget ) { $p2h += 5; }
	if ( $p2h > $phTarget ) { $p2h -= 3; }
	$p2h = $phTarget if abs($p2h - $phTarget) < 3;
	$p2h = 0 if $p2h < 0;

	$bgx = $BgPosition >> $GAMEBITS;
	$p1x -= $bgx;
	$p2x -= $bgx;
	$bgy = 0;
	
	AdvanceEarthquake();
	
	# 5. DEBUG POLYGONS
	
	if ( $Debug )
	{
		$fr1 = $Fighter1->{FRAMES}->[ $Fighter1->{FR} ];
		@p1head = @{ $fr1->{head} };
		MirrorPolygon( \@p1head ) if $Fighter1->{DIR} < 0;
		OffsetPolygon( \@p1head,  $Fighter1->{X} / $GAMEBITS2 - $bgx, $Fighter1->{Y} / $GAMEBITS2 - $bgy );
		@p1body = @{ $fr1->{body} };
		MirrorPolygon( \@p1body ) if $Fighter1->{DIR} < 0;
		OffsetPolygon( \@p1body,  $Fighter1->{X} / $GAMEBITS2 - $bgx, $Fighter1->{Y} / $GAMEBITS2 - $bgy );
		@p1legs = @{ $fr1->{legs} };
		MirrorPolygon( \@p1legs) if $Fighter1->{DIR} < 0;
		OffsetPolygon( \@p1legs,  $Fighter1->{X} / $GAMEBITS2 - $bgx, $Fighter1->{Y} / $GAMEBITS2 - $bgy );
		if ( defined $fr1->{hit} )
		{
			@p1hit = @{ $fr1->{hit} };
			MirrorPolygon( \@p1hit ) if $Fighter1->{DIR} < 0;
			OffsetPolygon( \@p1hit,  $Fighter1->{X} / $GAMEBITS2 - $bgx, $Fighter1->{Y} / $GAMEBITS2 - $bgy );
		}
		else
		{
			undef @p1hit;
		}
		
		$fr2 = $Fighter2->{FRAMES}->[ $Fighter2->{FR} ];
		@p2head = @{ $fr2->{head} };
		MirrorPolygon( \@p2head ) if $Fighter2->{DIR} < 0;
		OffsetPolygon( \@p2head,  $Fighter2->{X} / $GAMEBITS2 - $bgx, $Fighter2->{Y} / $GAMEBITS2 - $bgy );
		@p2body = @{ $fr2->{body} };
		MirrorPolygon( \@p2body ) if $Fighter2->{DIR} < 0;
		OffsetPolygon( \@p2body,  $Fighter2->{X} / $GAMEBITS2 - $bgx, $Fighter2->{Y} / $GAMEBITS2 - $bgy );
		@p2legs = @{ $fr2->{legs} };
		MirrorPolygon( \@p2legs) if $Fighter2->{DIR} < 0;
		OffsetPolygon( \@p2legs,  $Fighter2->{X} / $GAMEBITS2 - $bgx, $Fighter2->{Y} / $GAMEBITS2 - $bgy );
		if ( defined $fr2->{hit} )
		{
			@p2hit = @{ $fr2->{hit} };
			MirrorPolygon( \@p2hit ) if $Fighter2->{DIR} < 0;
			OffsetPolygon( \@p2hit,  $Fighter2->{X} / $GAMEBITS2 - $bgx, $Fighter2->{Y} / $GAMEBITS2 - $bgy );
		}
		else
		{
			undef @p2hit;
		}
	}

}


