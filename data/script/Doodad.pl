=comment

Doodad members are:

POS		(int,int)	The Doodad's logical position.
SIZE	(int,int)	The Doodad's physical size.
SPEED	(int,int)	The Doodad's logical speed.
ACCEL	(int,int)	The Doodad's logical acceleration.
T		int			The Doodad's type. 0 means text.
F		int				The Doodad's frame.
OWNER	int			The Doodad's owner (0 for player 1, 1 for player 2, other means no owner)
GFXOWNER int		The Doodad's graphics owner (0 for player 1, 1 for player 2, other means global)
SA		int			Animation speed
FRAMES	int			The number of frames.
LIFETIME int		The amount of time before this doodad dies. -1 means infinite.
TEXT	string		The text displayed in a text doodad.
HOSTILE	int			1 if the doodad can collision with players and damage them.



Doodad types are:
0		Text
1		Zoli's shot


=cut


use strict;

require 'Collision.pl';


%::DoodadDefinitions = (
'ZoliShot' => {
	'W'  => 64,   'H'  => 64,   'T'  => 1,
	'SX' => 48,   'SY' => -5,   'AX' => 0,    'AY' => 1,
	'SA' => 1/25, 'FRAMES'=>6,  'HOSTILE'=> 1,                },

'UPiShot' => {
	'W'  =>170,   'H'  => 22,   'T'  => 2,
	'SX' => 48,   'SY' => 0,    'AX' => 0,    'AY' => 0,
	'SA' => 1/10, 'FRAMES'=>5,  'HOSTILE'=> 1,                },

'Explosion' => {
	'W'  => 58,   'H'  => 80,   'T'  => 3,
	'SX' => 0,    'SY' => 0,    'AX' => 0,    'AY' => 0,
	'SA' => 1/10, 'FRAMES'=>15, 'HOSTILE'=> 0,                },
);


=comment

CreateDoodad creates a new doodad, and appends it to the global doodad list,
@Doodads. The parameters are:
	x		int		The logical horizontal position of the center of the doodad
	y		int		The logical vertical position of the center of the doodad
	t		string	The type of the doodad (doodad types are described above)
	owner	int		The number of the player who 'owns' the doodad (0 or 1, anything else means no owner)

=cut

sub CreateDoodad($$$$)
{
	my ($x, $y, $t, $owner) = @_;
	my ($doodad, $doodaddef, $w, $h);

	$doodaddef = $::DoodadDefinitions{$t};
	if ( defined $doodaddef )
	{
		$w = $doodaddef->{W};
		$h = $doodaddef->{H};
		$t = $doodaddef->{T};
	}
	else
	{
		$w = $h = $t = 0;
		$doodaddef = {};
	}
	
	$doodad = {
		'X'		=> $x - $w * $::GAMEBITS2 / 2,		#1/1
		'Y'		=> $y - $h * $::GAMEBITS2 / 2,		#1/1
		'W'		=> $w,
		'H'		=> $h,
		'T'		=> $t,
		'F'		=> 0,
		'OWNER'	=> $owner,
		'SX'	=> 0,
		'SY'	=> 0,
		'SA'	=> 0,
		'FRAMES'=> 0,
		'LIFETIME'=> -1,
		'HOSTILE'=> 1,
		%{$doodaddef}
	};
	
	push @::Doodads, ($doodad);
	return $doodad;
}


sub CreateTextDoodad
{
	my ($x, $y, $owner, $text) = @_;
	my ($self);
	$self = CreateDoodad($x, $y, 0, $owner);
	$self->{'TEXT'} = $text;
	return $self;
}



=comment
UpdateDoodad is called once every game tick.

It should return 0 if the doodad is still active.
If UpdateDoodad returns nonzero, it will be removed in this tick and
deleted.
=cut

sub UpdateDoodad
{
	my ($doodad) = @_;

	if ( $doodad->{LIFETIME} >= 0 )
	{
		$doodad->{LIFETIME} -= 1;
		if ( $doodad->{LIFETIME} < 0 )
		{
			# Doodad dies.
			return 1;
		}
	}

	$doodad->{X} += $doodad->{SX};
	$doodad->{Y} += $doodad->{SY};
	$doodad->{F} += $doodad->{SA};

	while ( $doodad->{F} > $doodad->{FRAMES} )
	{
		$doodad->{F} -= $doodad->{FRAMES};
	}
	
	if ( $doodad->{X} > $::BGWIDTH2 ) { return 1; }
	if ( $doodad->{X} < $doodad->{W} * $::GAMEBITS2 ) { return 1; }
	if ( $doodad->{Y} > $::SCRHEIGHT2 ) { return 1; }
	if ( $doodad->{Y} < $doodad->{H} * $::GAMEBITS2 ) { return 1; }
	
	if ( $doodad->{HOSTILE} )
	{
	#	CheckHit($doodad);
	}
	
	return 0;
}


=comment
sub CheckHit($)
{
	my ( $self ) = @_;

	my ( @poly, $x, $y, $w, $h );
	$x = $self->{X} / $::GAMEBITS2;
	$y = $self->{Y} / $::GAMEBITS2;
	$w = $self->{W};
	$h = $self->{H};
	
	@poly = (
		$x, $y,
		$x+$w, $y,
		$x+$w, $y+$h,
		$x, $y+$h );

	if ( $self->{OWNER} != 0 )
	{
		if ( $::Fighter1->IsHitAt( \@poly ) )
		{
			DoHitPlayer($self, $::Fighter1);
		}
	}
	
	if ( $self->{OWNER} != 1 )
	{
		if ( $::Fighter2->IsHitAt( \@poly ) )
		{
			DoHitPlayer($self, $::Fighter2);
		}
	}

}


sub DoHitPlayer($$)
{
	my ($self,$player) = @_;

	$player->
}
=cut

return 1;

