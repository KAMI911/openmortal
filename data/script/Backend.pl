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
$MOVEMARGIN2= 50 << $GAMEBITS;				# The logical border for figthers.

$BGWIDTH2	= 1920 << $GAMEBITS;			# The logical background width
$BGHEIGHT2	= 480 << $GAMEBITS;				# The logical background height

$GROUND2	= 160 << $GAMEBITS;				# The logical ground level.
$DELMULTIPLIER = 1;							# DEL values in states are multiplied by this.

$BgMax		= $BGWIDTH2 - $SCRWIDTH2;		# The logical maximum camera position
$BgSpeed	= 0;							# The current speed of the background movement
$BgPosition	= $BgMax >> 1;					# The logical camera position
$BgScrollEnabled = 1;						# Can the background scroll?
$HitPointScale = 10;						# Scale the hit points by this.
$NextDoodad = 0;							# The next doodad to process


require 'PlayerInput.pl';
require 'Fighter.pl';
require 'FighterStats.pl';
require 'Doodad.pl';
require 'Keys.pl';
require 'State.pl';


=comment
#require 'Kolos.pl';
require 'Zoli.pl';
require 'UPi.pl';
#require 'Ulmar.pl';
#require 'Cumi.pl';

print "Kolos frames :", scalar @KolosFrames,"\n";
print "Kolos states :", scalar keys %KolosStates,"\n";
print "Zoli frames :", scalar @ZoliFrames,"\n";
print "Zoli states :", scalar keys %ZoliStates,"\n";
print "UPi frames :", scalar @UPiFrames,"\n";
print "UPi states :", scalar keys %UPiStates,"\n";
print "Ulmar frames :", scalar @UlmarFrames,"\n";
print "Ulmar states :", scalar keys %UlmarStates,"\n";
print "Cumi frames :", scalar @CumiFrames,"\n";
print "Cumi states :", scalar keys %CumiStates,"\n";
=cut


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
$time = 0;		# Current game time
$over = 0;		# Is the game over?
$ko = 0;		# Is one fighter knocked down?


$dx = 0;		# Doodad X position
$dy = 0;		# Doodad Y position
$dt = 0;		# Doodad type
$df = 0;		# Doodad frame number



=comment

JUDGEMENT METHODS

=cut


sub JudgementStart
{
	$bgx = 0;
	$bgy = ( $SCRHEIGHT2 - $BGHEIGHT2 ) >> 1;
	$BgSpeed = 0;
	$BgPosition = 0;
	$BgScrollEnabled = 0;
	$OverTimer = 0;
	$JudgementMode = 1;
	($JudgementWinner) = @_;
	
	$time = 0;
	$over = 0;
	
	$Fighter1->Reset();
	$Fighter1->{X} =  ($JudgementWinner ? 150 : 520 ) * $GAMEBITS2;
	$Fighter1->{DIR} = ($JudgementWinner ? 1 : -1 );
	$Fighter1->{NEXTST} = 'Stand';
	$Fighter1->Update();
	
	$Fighter2->Reset();
	$Fighter2->{X} = ($JudgementWinner ? 520 : 150 ) * $GAMEBITS2;
	$Fighter2->{DIR} = ($JudgementWinner ? -1 : 1 );
	$Fighter2->{NEXTST} = 'Stand';
	$Fighter2->Update();
	
	$Input1->Reset();
	$Input2->Reset();
}


=comment

PLAYER SELECTION METHODS 

=cut

sub SelectStart
{
	$bgx = 0;
	$bgy = ( $SCRHEIGHT2 - $BGHEIGHT2 ) >> 1;
	$BgSpeed = 0;
	$BgPosition = 0;
	$BgScrollEnabled = 0;
	$OverTimer = 0;
	$JudgementMode = 0;	
	
	$time = 0;
	$over = 0;
	
	$Fighter1->Reset(); #\@ZoliFrames,\%ZoliStates);
	$Fighter1->{X} = 80 * $GAMEBITS2;
	$Fighter1->{NEXTST} = 'Stand';
	$Fighter1->Update();
	
	$Fighter2->Reset(); #\@ZoliFrames,\%ZoliStates);
	$Fighter2->{X} = 560 * $GAMEBITS2;
	$Fighter2->{NEXTST} = 'Stand';
	$Fighter2->Update();
	
	$Input1->Reset();
	$Input2->Reset();
}


sub SetPlayerNumber
{
	my ($player, $number) = @_;
	my ($f);
	
	$f = $player ? $Fighter2 : $Fighter1;

	if    ( $number eq 1 ) { $f->Reset('Ulmar', \@UlmarFrames,\%UlmarStates); }
	elsif ( $number eq 2 ) { $f->Reset('UPi', \@UPiFrames,\%UPiStates); }
	elsif ( $number eq 3 ) { $f->Reset('Zoli', \@ZoliFrames,\%ZoliStates); }
	elsif ( $number eq 4 ) { $f->Reset('Cumi', \@CumiFrames,\%CumiStates); }
	elsif ( $number eq 5 ) { $f->Reset('Sirpi', \@SirpiFrames,\%SirpiStates); }
	elsif ( $number eq 6 ) { $f->Reset('Maci', \@MaciFrames,\%MaciStates); }
	elsif ( $number eq 7 ) { $f->Reset('Bence', \@BenceFrames,\%BenceStates); }
	elsif ( $number eq 9 ) { $f->Reset('Descant', \@DescantFrames,\%DescantStates); }
	elsif ( $number eq 8 ) { $f->Reset('Grizli', \@GrizliFrames,\%GrizliStates); }
	else {
		# Fallback
		$f->Reset('Zoli', \@ZoliFrames,\%ZoliStates);
	}
	
	$f->{NEXTST} = 'Stand';
	$f->Update();
}


sub PlayerSelected
{
	my ($number) = @_;
	my ($f) = $number ? $Fighter2 : $Fighter1;
	
	$f->Event( 'Won' );
	$f->Update();
}



=comment

GAME BACKEND METHODS

=cut

sub GameStart
{
	my ( $MaxHP ) = @_;
	
	$bgx = ( $SCRWIDTH2 - $BGWIDTH2) >> 1;
	$bgy = ( $SCRHEIGHT2 - $BGHEIGHT2 ) >> 1;
	$BgSpeed = 0;
	$BgPosition = $BgMax >> 1;
	$BgScrollEnabled = 1;
	$HitPointScale = 1000 / $MaxHP;
	
	$time = 0;
	$Fighter1->Reset();
	$Fighter1->{HP} = $MaxHP;
	$Input1->Reset();
	$Fighter2->Reset();
	$Fighter2->{HP} = $MaxHP;
	$Input2->Reset();
	$over = 0;
	$ko = 0;
	$OverTimer = 0;
	$JudgementMode = 0;	

	@Doodads = ();
	@Sounds = ();

	$p1h = $p2h = 0;
}



sub GetFighterData
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
		$doodad_x = $doodad_y = $doodad_t = $doodad_f = -1;
		return;
	}

	my ($doodad) = $Doodads[$NextDoodad];
	$doodad_x = $doodad->{X} / $GAMEBITS2 - $bgx;
	$doodad_y = $doodad->{Y} / $GAMEBITS2 - $bgy;
	$doodad_t = $doodad->{T};
	$doodad_f = $doodad->{F};
	$doodad_text = $doodad->{TEXT};
	++$NextDoodad;
}



sub UpdateDoodads
{
	my ($i, $j, $doodad);
	
	for ($i=0; $i<scalar @Doodads; ++$i)
	{
		$doodad = $Doodads[$i];
		$j = UpdateDoodad( $doodad );
		if ( $j )
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
	if ( ($fighter->{IDLE} > 150) and (rand(350) < 1) )
	{
		$fighter->Event("Fun");
	}
	if ( ($fighter->{IDLE} > 150) and (rand(350) < 1) )
	{
		$fighter->Event("Threat");
	}
}



sub GameAdvance
{
	my ($hit1, $hit2);
	
	$time += 4/1000;
	$NextDoodad = 0;
	$NextSound = 0;
	
	# 1. ADVANCE THE PLAYERS
	
	$Input1->Advance();
	$Input2->Advance();
	$Fighter1->Advance( $Input1 );
	$Fighter2->Advance( $Input2 );
	$hit2 = $Fighter1->CheckHit();
	$hit1 = $Fighter2->CheckHit();
	
	# 2. Events come here
	
	DoFighterEvents( $Fighter1, $hit1 );
	DoFighterEvents( $Fighter2, $hit2 );
	UpdateDoodads();
	$Fighter1->Update();
	$Fighter2->Update();
	
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


