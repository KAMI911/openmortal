require 'Collision.pl';
require 'DataHelper.pl';
require 'Damage.pl';


package Fighter;



use strict;

=comment

Fighter's members are:

NAME	string	The name of the character, e.g. "Ulmar".
FRAMES	array	The character's frame description.
STATES	hash	The character's state description.
NUMBER	int		Player number (either 0 or 1)
X		int		The fighter's current anchor, horizontal coordinate.
Y		int		The fighter's current anchor, vertical coordinate.
ST		string	The name of the fighter's current state.
FR		int		The number of the fighter's current frame (same as STATES->{ST}->{F}).
DEL		int		The amount of time before the character moves to the next state.
NEXTST	string	The name of the next state after this one (calculated by Advance and Event, user by Update).
DIR		int		-1: the character is facing left; 1: if the character is facing right.
PUSHY	int		The character's vertical momentum, from jumping/hits.
PUSHX	int		The character's horizontal momentum, from hits.
HP		int		Hit points, from 100 down to 0.
IDLE	int		The amount of game time since the player is ready.
CHECKEHIT int	1 if the hit needs to be checked soon.
DELIVERED int	1 if a hit was delivered in this state.
COMBO	int		The number of consecutive hits delivered to this fighter.
COMBOHP	int		The amount of HP delivered in the last combo.
OTHER	hash	A reference to the other Fighter



new
Reset
Advance
CheckHit
IsHitAt
Event
Update



Needs to be subclassed: Reset, NextState

=cut




sub new {
	my ($class) = @_;
	
	my $self = {
		'NUMBER'=> 0,
		'ST'	=> 'Start',
		'FR'	=> 0,
		'DEL'	=> 0,
		'DIR'	=> 1,
		'PUSHY' => 0,
		'PUSHX'	=> 0,
	};
	
	bless $self, $class;
	return $self;
}

sub Reset {
	my ($self, $name, $frames, $states) = @_;
	
	die "Insufficient parameters." unless defined $self;# and defined $frames and defined $states;
	
	my ($number);
	$number = $self->{NUMBER};

	print STDERR "Resetting fighter $number\n";

	$self->{NAME}	= $name if defined $name;
	$self->{FRAMES}	= $frames if defined $frames;
	$self->{STATES}	= $states if defined $states;
	$self->{X}		= (( $number ? 540 : 100 ) << $::GAMEBITS) + $::BgPosition;
	$self->{Y}		= $::GROUND2;
	$self->{ST}		= 'Start';
	$self->{FR}		= $self->GetCurrentState()->{F};
	$self->{DEL}	= 0;
	$self->{DIR}	= ($number ? -1 : 1);
	$self->{PUSHY}	= 0;
	$self->{PUSHX}	= 0;
	$self->{HP}		= 100;
	$self->{IDLE}	= 0;
	$self->{CHECKHIT} = 0;
	$self->{DELIVERED} = 0;
	$self->{COMBO} 	= 0;
	$self->{COMBOHP}= 0;
}



=comment

Advance should be called once every game tick. It advances the fighter,
preparing his variables for drawing him in the next tick. Advance also
processes the input if it is appropriate to do so in the current state.

Advance prepares the NEXTST attribute of the player. It does not modify
the current ST yet, because that may depend on hits, event, etc. Advance
should be followed by CheckHit and Event calls, and finally Update.

=cut

sub Advance {
	my ($self, $in) = @_;
	
	my ($stname,		# The name of the current state.
		$st,			# The descriptor of the current state.
		$move,			# The move associated with the current state.
		$i, $j,			# temp var.
		@body );		# collision detection
	
	$self->{NEXTST} = '';
	$stname = $self->{ST};

	$st = $self->{'STATES'}->{$stname};
	
	# 1. DECREMENT 'DEL' PROPERTY
	
	$self->{'DEL'}--;
	if ( $st->{SITU} eq 'Ready' )
	{
		$self->{IDLE} = $self->{IDLE} + 1;
	}
	else
	{
		$self->{IDLE} = 0;
	}

	# 2. UPDATE THE HORIZONTAL POSITION
	
	# 2.1. 'MOVE' PLAYER IF NECESSARY
	
	if ( defined $st->{'MOVE'} )
	{	
		$move = $st->{'MOVE'}*$self->{'DIR'} * 6;
		$self->{'X'} += $move;
	}
	
	# 2.2. 'PUSH' PLAYER IF NECESSARY

	if ( $self->{PUSHX} )
	{
		$i = ($self->{PUSHX}+7) / 8; 		# 1/1; syntax highlight..
		$self->{X} += $i;
		$self->{PUSHX} -= $i if $self->{PUSHY} == 0;
	}
	
	# 2.3. MAKE SURE THE TWO FIGHTERS DON'T "WALK" INTO EACH OTHER
	
	if ( $self->{Y} >= $::GROUND2 and
		$self->{OTHER}->{Y} >= $::GROUND2 )
	{
		my ( $centerX, $centerOtherX, $pushDir );
		$centerX = $self->GetCenterX;
		$centerOtherX = $self->{OTHER}->GetCenterX;
		
		if ( abs($centerX - $centerOtherX) < 60 )
		{
			$pushDir = ($centerX > $centerOtherX) ?  1 : -1;
			$self->{X} += 10 * $pushDir;
			$self->{OTHER}->{X} -= 10 * $pushDir;
		}
	}


	# 2.4. HORIZONTAL BOUNDS CHECKING
	
	$self->{X} = $::BgPosition + $::MOVEMARGIN2
		if $self->{X} < $::BgPosition + $::MOVEMARGIN2;
	$self->{X} = $::BgPosition + $::SCRWIDTH2 - $::MOVEMARGIN2
		if $self->{X} > $::BgPosition + $::SCRWIDTH2 - $::MOVEMARGIN2;
		
	# 3. FLYING OR FALLING
	
	if ( $self->{Y} < $::GROUND2 )
	{
		$self->{PUSHY} += 3;
		$self->{Y} += $self->{PUSHY};
		
		if ($self->{Y} >= $::GROUND2)
		{
			# LANDING
			if ( $st->{SITU} eq 'Falling')
			{
				push @::Sounds, ('splat.wav') if $self->{PUSHY} > 40;
				# print "PUSHY = ", $self->{PUSHY}, "; ";
				if ( $self->{PUSHY} > 30 )
				{
					# Bouncing after a fall
					$self->{PUSHY} = - $self->{PUSHY} / 2;			# 1/1
					$self->{Y} = $::GROUND2 - 1;
				}
				else
				{
					# Getting up after a fall.
					$self->{PUSHY} = 0;
					$self->{Y} = $::GROUND2;
					$self->{DEL} = 0;
					$self->{NEXTST} = 'Getup';
					$self->{NEXTST} = 'Dead' if $self->{HP} <= 0;
				}
				return;
			}
			else
			{
				push @::Sounds, ('thump.wav');
				$self->{PUSHY} = 0;
				$self->{Y} = $::GROUND2;
				if ( substr($self->{ST},0,4) eq 'Jump' )
				{
					# Landing after a jump
					$self->{DEL} = 0;
					$self->{NEXTST} = 'Stand';
				}
				else
				{
					print "Landed; state: ", $self->{ST}, "\n";
				}
			return;
			}
		}
	}
	
	return if $self->{'DEL'} > 0;
	
	# 4. DELAYING OVER. MAKE END-OF-STATE CHANGES.

	# 4.1. 'DELTAX' displacement if so specified

	if ($st->{DELTAX})
	{
		$self->{X} += $st->{DELTAX} * $self->{DIR} * $::GAMEBITS2;
	}
	
	# 4.2. 'TURN' if so specified
	
	if ($st->{TURN})
	{
		$self->{DIR} = - $self->{DIR};

		# Swap input keys..
		$i = $in->{Keys}->[2];
		$in->{Keys}->[2] = $in->{Keys}->[3];
		$in->{Keys}->[3] = $i;
	}
	
	# 5. CALCULATE THE NEXT STATE

	my ($nextst, $con, $input, $mod, $action, $dist);

	$nextst = $st->{NEXTST};
	$con = $st->{CON};
	$dist = ($self->{OTHER}->{X} - $self->{X}) * $self->{DIR};
	
	if ( defined $con )
	{
		$self->ComboEnds();
		
		# 5.1. The current state has connections!
		
		($input,$mod) = $in->GetAction();
		$mod = '' unless defined $mod;
		$action = $input;
		
		# Try to find the best matching connection.
		for ( $i = length($mod); $i>=0; --$i )
		{
			$action = $input . substr( $mod, 0, $i );
			last if defined $con->{$action};
		}
		
		if ( defined $con->{$action} )
		{
			# print "dist = $dist. ";
			if ( ($action eq 'hpunch' or $action eq 'lpunch') and ($dist>=0) and ($dist<800) )
			{
				$action = 'hpunchF' if defined $con->{'hpunchF'};
			}
			elsif ( ($action eq 'hkick' or $action eq 'lkick') and ($dist>=0) and ($dist<900) )
			{
				$action = 'lkickF' if defined $con->{'lkickF'};
			}
			$nextst = $con->{$action};
			$in->ActionAccepted;
		}
	}

	$self->{'NEXTST'} = $nextst;
}




sub CheckHit
{
	my ($self) = @_;

	return 0 unless $self->{CHECKHIT};
	$self->{CHECKHIT} = 0;
	
	my ($st,		# A reference to the next state.
		$nextst,	# The name of the next state.
		@hit,		# The hit array
		$i,
	);
	
	$st = $self->{STATES}->{$self->{ST}};
	
	return 0 unless $st->{HIT};
	return 0 unless defined $self->{FRAMES}->[$st->{F}]->{hit};
	
	@hit = @{$self->{FRAMES}->[$st->{F}]->{hit}};
	if ( $self->{DIR}<0 )
	{
		::MirrorPolygon( \@hit );
	}
	::OffsetPolygon( \@hit,
		$self->{X} / $::GAMEBITS2,
		$self->{Y} / $::GAMEBITS2 );
	
	$i = $self->{OTHER}->IsHitAt( \@hit );
	$self->{DELIVERED} = 1 if $i;
	print "Collision = $i; ";

	if ( $i == 0 )
	{
		# Make the 'Woosh' sound - maybe.
		$nextst = $st->{NEXTST};
		print "NEXTST = $nextst";
		$nextst = $self->{STATES}->{$nextst};
		print "NEXTST->HIT = ", $nextst->{HIT}, "\n";
		push @::Sounds, ('woosh.wav') unless $nextst->{HIT} and defined $self->{FRAMES}->[$nextst->{F}]->{hit};
	}
	
	return $i;
}




sub IsHitAt
{
	my ($self, $poly) = @_;

	my ($frame, @a);
	$frame = $self->{FRAMES}->[$self->{FR}];

	::OffsetPolygon( $poly, -$self->{X} / $::GAMEBITS2, -$self->{Y} / $::GAMEBITS2 );
	if ( $self->{DIR}<0 )
	{
		::MirrorPolygon( $poly );
	}

#	print "IsHitAt (", join(',',@{$poly}),")\n        (",
#		join(',',@{$frame->{head}}),")\n        (",
#		join(',',@{$frame->{body}}),")\n        (",
#		join(',',@{$frame->{legs}}),")\n";
	
	return 1 if ::Collisions( $poly, $frame->{head} );
	return 2 if ::Collisions( $poly, $frame->{body} );
	return 3 if ::Collisions( $poly, $frame->{legs} );
	return 0;
}



=comment
Event($self, $event, $eventpar): Handles the following events:
'Won', 'Hurt', 'Thread', 'Fun', 'Turn'
=cut


sub Event($$$)
{
	my ($self, $event, $eventpar) = @_;
	
	my ($st);

	$st = $self->GetCurrentState();
	
	if ( ($st->{SITU} eq 'Ready' or $st->{SITU} eq 'Stand') and $event eq 'Won' )
	{
		$self->{NEXTST} = 'Won' unless substr($self->{ST}, 0, 3) eq 'Won' ;
		return;
	}
	
	if ( $st->{SITU} eq 'Ready' and $self->{NEXTST} eq '' )
	{
		if ( $event =~ /Hurt|Threat|Fun|Turn/ )
		{
			if ( $event eq 'Fun' and defined $self->{STATES}->{'Funb'} and rand() < 0.5 )
			{
				$event = 'Funb';
			}
			$self->{NEXTST} = $event;
		}
	}
}


=comment
Event($self, $event, $eventpar): Handles the following events:
'Highhit', 'Uppercut', 'Hit', 'Groinhit', 'Leghit', 'Fall'
=cut


sub HitEvent($$$)
{
	my ($self, $event, $eventpar) = @_;
	
	my ($st, $blocked, $damage);

	$st = $self->GetCurrentState();
	
	# Do events: Highhit, Uppercut, Hit, Groinhit, Leghit, Fall

	$eventpar = '' unless defined $eventpar;		# Supress useless warning
	$damage = ::GetDamage( $self->{OTHER}->{NAME}, $self->{OTHER}->{ST} );
	$blocked = $st->{BLOCK};
	$blocked = 0 if ( $self->IsBackTurned() );
	
	print "Event '$event', '$eventpar'\n";
	print "Blocked.\n" if ( $blocked );

	# Hit point adjustment here.
	
	$self->{HP} -= $blocked ? $damage >> 3 : $damage;
	
	# Turn if we must.

	$self->{DIR} = ( $self->{X} > $self->{OTHER}->{X} ) ? -1 : 1;
	
	# Handle the unfortunate event of the player "dying".

	if ( $self->{HP} <= 0 )
	{
		push @::Sounds, ('bowling.voc');
		$self->{NEXTST} = 'Falling';
		$self->{PUSHX} = -20 * 3 * $self->{DIR};
		$self->{PUSHY} = -80;
		$self->{Y} -= 10;
		$self->{COMBO} +=  1;
		$self->{COMBOHP} += $damage;
		$self->ComboEnds();
		$::ko = 1;
		return;
	}

	# Handle blocked attacks.

	if ( $blocked )
	{
		push @::Sounds, ('thump.wav');
		$self->{PUSHX} = - $damage * 5 * $self->{DIR};
		return;
	}

	# Handle the rest of the events.

	if ( $event eq 'Uppercut' ) { push @::Sounds, ('evil_laughter.voc'); }
	elsif ($event eq 'Groinhit') { push @::Sounds, ('woman_screams.voc'); }
	 { push @::Sounds, ('thump3.voc'); }
	
	$self->{COMBO} +=  1;
	$self->{COMBOHP} += $damage;
	$damage *= $self->{COMBO};		# Only for the purpose of pushing
	
	if ( $st->{SITU} eq 'Crouch' )
	{
		 if ( $event eq 'Uppercut' or $event eq 'Fall' )
		 {
			$self->{NEXTST} = 'Falling';
			$self->{PUSHX} = -48 * 3 * $self->{DIR};
			$self->{PUSHY} = -100;
			$self->{Y} -= 10;
		 }
		 else
		 {
			if ($eventpar == 1) {
				$self->{NEXTST} = 'KneelingPunched';
			} else {
				$self->{NEXTST} = 'KneelingKicked';
			}
			$self->{PUSHX} = - $damage * 20 * $self->{DIR};
		 }
		 return;
	}

	if ( $st->{SITU} eq 'Falling' )
	{
		$self->{PUSHY} -= 50;
		$self->{PUSHY} = -80 if $self->{PUSHY} > -80;
		$self->{Y} -= 10;
		return;
	}

	if ( $self->{Y} < $::GROUND2 )
	{
		$self->{NEXTST} = 'Falling';
		$self->{PUSHY} -= 50;
		$self->{PUSHX} = -48 * 3 * $self->{DIR};
		return;
	}
	
	if ( $event eq 'Highhit' )
	{
		$self->{NEXTST} = 'HighPunched';
		$self->{PUSHX} = - $damage * 20 * $self->{DIR};
	}
	elsif ( $event eq 'Hit' )
	{
		if ($eventpar == 1) {
			$self->{NEXTST} = 'HighPunched';
		} elsif ($eventpar == 2) {
			$self->{NEXTST} = 'LowPunched';
		} else {
			$self->{NEXTST} = 'Swept';
		}
		$self->{PUSHX} = - $damage * 20 * $self->{DIR};
	}
	elsif ( $event eq 'Groinhit' )
	{
		$self->{NEXTST} = 'GroinKicked';
		$self->{PUSHX} = - $damage * 20 * $self->{DIR};
	}
	elsif ( $event eq 'Leghit' )
	{
		$self->{NEXTST} = 'Swept';
		$self->{PUSHX} = - $damage * 20 * $self->{DIR};
	}
	elsif ( $event eq 'Uppercut' or $event eq 'Fall' )
	{
		$self->{NEXTST} = 'Falling';
		$self->{PUSHX} = -48 * 3 * $self->{DIR};
		$self->{PUSHY} = -100;
		$self->{Y} -= 10;
	}
	else
	{
		die "Unknown event: $event, $eventpar";
	}
}




sub GetCurrentState
{
	my ($self, $statename) = @_;
	my ($stateref);

	die unless defined $self;

	$statename = $self->{ST} unless defined $statename;
	$stateref = $self->{STATES}->{$statename};
	return $stateref;
}


sub ComboEnds
{
	my ($self) = @_;
	
	if ($self->{COMBO})
	{
		if ( $self->{COMBO} > 1 )
		{
			my ( $head, $doodad, $x, $y );
			$head = $self->{FRAMES}->[$self->{FR}]->{head};
			$x = $self->{X} + $head->[0] * $::GAMEBITS2 * $self->{DIR};
			$y = $self->{Y} + $head->[1] * $::GAMEBITS2;
			$doodad = ::CreateTextDoodad( $x, $y - 30 * $::GAMEBITS2,
				$self->{NUMBER},
				$self->{COMBO} . "-hit combo!" );
			$doodad->{LIFETIME} = 80;
			$doodad->{SY} = -3;
			$doodad->{SX} = -3;
			
			$doodad = ::CreateTextDoodad( $x, $y - 10 * $::GAMEBITS2,
				$self->{NUMBER},
				int($self->{COMBOHP}*$::HitPointScale/10) . "% damage" );
			$doodad->{LIFETIME} = 80;
			$doodad->{SY} = -3;
			$doodad->{SX} = +3;
			
			print $self->{COMBO}, "-hit combo for ", $self->{COMBOHP}, " damage.\n";

			push @::Sounds, ('ba_gooock.voc');
		}
		
		$self->{COMBO} = 0;
		$self->{COMBOHP} = 0;
	}
}


=comment

Update moves the fighter to his next state, if there is a NEXTST set.
NEXTST should be calculated before calling this method. No further
branching or CON checking is done.

If the next state has JUMP, the character is set aflying.

=cut

sub Update
{
	my ($self) = @_;

	my ($nextst,		# The name of the next state
		$st);			# The descriptor of the next state.
	
	# Is there a next state defined?
	$nextst = $self->{'NEXTST'};
	# If there isn't, no updating is necessary.
	return unless $nextst;

	# ADMINISTER THE MOVING TO THE NEXT STATE
	
	$st = $self->GetCurrentState( $nextst );
	$self->{'ST'} = $nextst;
	$self->{'FR'} = $st->{'F'};
die "ERROR IN STATE $nextst" unless defined $st->{DEL};
	$self->{'DEL'} = $st->{'DEL'} * $::DELMULTIPLIER;

	# HANDLE THE JUMP and PUSH ATTRIBUTE
	
	if ( defined ($st->{'JUMP'}) )
	{
		$self->{'PUSHY'} = -($st->{'JUMP'});
		$self->{'Y'} += $self->{'PUSHY'};
	}
	if ( defined ($st->{'PUSHX'}) )
	{
		$self->{'PUSHX'} += $st->{'PUSHX'} * $self->{DIR};
	}

	# HANDLE THE HIT ATTRIBUTE

	if ( defined ($st->{HIT}) )
	{
		$self->{CHECKHIT} = 1 unless $self->{DELIVERED};
	}
	else
	{
		$self->{DELIVERED} = 0;
	}

	# HANDLE THE SOUND ATTRIBUTE

	if ( defined $st->{SOUND} )
	{
		push @::Sounds, ($st->{SOUND});
	}

	# HANDLE DOODADS

	if ( defined ($st->{DOODAD}) )
	{
		# Create a doodad (probably a shot)
		my ($frame, $hit, $doodad, $ddata);
		
		$frame = $self->{FRAMES}->[$self->{FR}];
		$hit = $frame->{hit};
		if ( defined $hit )
		{
			$ddata = $st->{DOODAD};
		
			$doodad = ::CreateDoodad(
				$self->{X} + $hit->[0] * $::GAMEBITS2 * $self->{DIR},
				$self->{Y} + $hit->[1] * $::GAMEBITS2,
				$ddata->{W},
				$ddata->{H},
				$ddata->{T},
				$self->{NUMBER} );
			$doodad->{SX} = $ddata->{SX} * $self->{DIR};
			$doodad->{SY} = $ddata->{SY} * $self->{DIR};
			$doodad->{FRAMES} = $ddata->{FRAMES};
			$doodad->{SA} = $ddata->{SA};
		}
	}

}


=comment
Returns the characters 'centerline' in physical coordinates.
=cut

sub GetCenterX
{
	my ($self) = @_;
	my ($body, $x);
	
	$body = $self->{FRAMES}->[$self->{FR}]->{body};
	$x = $body->[0] + $body->[2] + $body->[4] + $body->[6];
	return $self->{X} / $::GAMEBITS2 + $x / 4 * $self->{DIR};
}


=comment
Is my back turned to my opponent? Returns true if it is.
=cut

sub IsBackTurned
{
	my ($self) = @_;
	
	return ( ($self->{X} - $self->{OTHER}->{X}) * ($self->{DIR}) > 0 );
}


return 1;

