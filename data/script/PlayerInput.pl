package PlayerInput;

$KEYTIMEOUT = 80;

sub new {
	my ($class, $name) = @_;
	my $self = {
		'InputQueue' =>		[],
		'InputPointer' =>	0,
		'Keys' =>			[0,0,0,0,0,0,0,0,0],
		'KeyTimer' =>		0,
	};
	bless ($self, $class);
	return $self;
}

sub Reset {
	my ($self) = @_;
	
	$self->ClearInput;
	$self->{'Keys'} = [0,0,0,0,0,0,0,0,0];
	$self->{'KeyTimer'} = 0;
}

sub RewindData {
	my ($self) = @_;
	my ($rewinddata);
	$rewinddata = {
		InputQueue		=> [ @{$self->{InputQueue}} ],
		InputPointer	=> $self->{InputPointer},
		Keys			=> [ @{$self->{Keys}} ],
		KeyTimer		=> $self->{KeyTimer},
	};
	bless ($rewinddata, PlayerInput);
	return $rewinddata;
}

sub ClearInput {
	my ($self) = @_;
	
	$self->{'InputQueue'} = [];
	$self->{'InputPointer'} = 0;
}

sub Advance {
	my ($self) = @_;
	my ($ip, $lastkey);
	
	$self->{'KeyTimer'} ++;
	$ip = $self->{'InputPointer'};
	
	if ($ip > 0) {
		$lastkey = $self->{'InputQueue'}->[$ip-1];
		return if ( ($lastkey<5) and ($self->{'Keys'}->[$lastkey]) );
		
		if ( $self->{'KeyTimer'} > $KEYTIMEOUT )
		{
			$self->{'KeyTimer'} = 0;
			$self->ClearInput;
			print ".";
		}
	}
}

sub KeyDown {
	my ($self, $key) = @_;
	
	${$self->{'Keys'}}[$key] = 1;
	${$self->{'InputQueue'}}[$self->{'InputPointer'}] = $key;
	$self->{'InputPointer'} += 1;
	$self->{'KeyTimer'} = 0;
}

sub KeyUp {
	my ($self, $key) = @_;
	
	${$self->{'Keys'}}[$key] = 0;
}




=comment
GetDefaultAction is used by GetAction. It returns a "default"
action when the last key is no longer pressed. The default
action can be (in order of preference)
* crouching
* blocking
* jumping (fw and bw)
* walking
=cut

sub GetDefaultAction {
	my ($self) = @_;
	my $k = $self->{'Keys'};
	
	return 'down' if ($k->[1]);
	return 'block'	if ($k->[4]);
	if ( ${$k}[0] )			# Up
	{
		return 'jumpfw' if ${$k}[3];
		return 'jumpbw' if ${$k}[2];
		return 'jump';
	}
	return 'forw'	if ($k->[3]);
	return 'back'	if ($k->[2]);
	return '';
}



=comment

GetAction returns two scalars: input and modifier.

Input is any of the following:
	lpunch		Low punch
	hpunch		High punch
	lkick		Low kick
	hkick		High kick
	jump		Jump
	jumpfw		Jump forwards (up+forward)
	jumpbw		Jump backwards  (up+back)
	forw		Forwards (walk)
	back		Back (walk)
	down		Down (crouch)
	block		Block
	''			no action

Modifier applies to action buttons (punch and kick) only. It lists the
non-action buttons pressed before the action button, in reverse order.
The modifiers are U,D,B,F,X for up, down, back, forwards or block.

E.G. if the player presses down, forwards, low punch, the output is:
('lpunch', 'FD').

=cut

sub GetAction {
	my ($self) = @_;
	
	my $k = $self->{'Keys'};
	my $q = $self->{'InputQueue'};
	my $p = $self->{'InputPointer'}-1;
	
	if ($p<0)								# No keys pushed
	{
		return $self->GetDefaultAction();
	}
	
	my $lastkey = ${$q}[$p];
	unless ( ${$k}[$lastkey] )
	{
		# Not pushed anymore
		return $self->GetDefaultAction();
	}
	
	if ($lastkey >=5)		# punch or kick
	{
		my ($act, $mod, $i);
		$mod = '';
		
		if    ($lastkey==5) { $act = 'lpunch'; }
		elsif ($lastkey==6) { $act = 'hpunch'; }
		elsif ($lastkey==7) { $act = 'lkick'; }
		else { $act = 'hkick'; }
		
		# Add previous keys
		
		for ( $i = $p-1; $i>=0; $i-- )
		{
			$lastkey = ${$q}[$i];			# Previous key
			last if $lastkey>=5;			# Only movement keys count
			if    ($lastkey==0) { $mod .= 'U'; }
			elsif ($lastkey==1) { $mod .= 'D'; }
			elsif ($lastkey==2) { $mod .= 'B'; }
			elsif ($lastkey==3) { $mod .= 'F'; }
			elsif ($lastkey==4) { $mod .= 'X'; }
		}
		
		print "$act $mod\n";
		return ($act,$mod);
	}
	
	if ( ${$k}[0] )			# Up
	{
		return 'jumpfw' if ${$k}[3];
		return 'jumpbw' if ${$k}[2];
		return 'jump';
	}
	
	return 'forw'	if $lastkey==3;
	return 'back'	if $lastkey==2;
	return 'down'	if $lastkey==1;
	return 'block'	if $lastkey==4;
}


sub ActionAccepted
{
	my ($self) = @_;
	
	my $k = $self->{'Keys'};
	my $q = $self->{'InputQueue'};
	my $p = $self->{'InputPointer'}-1;
	
	return '' if ($p<0);					# No keys pushed
	my $lastkey = ${$q}[$p];
	return '' unless ${$k}[$lastkey];		# Not pushed anymore
	
	$self->ClearInput if $lastkey>=5;
}


package main;


=comment

The following are interface functions and variables for the C++ code and
Backend.pl

=cut

sub CreatePlayerInputs()
{
	my ($i);
	@::Inputs=();
	for ( $i=0; $i<$MAXPLAYERS; ++$i )
	{
		$::Inputs[$i] = new PlayerInput;
	}
}


sub KeyDown
{
	my ($player, $key) = @_;
	#print "KeyDown ($player, $key)\n";
	
	if ( ($key==2) or ($key==3) )
	{
		# Map left and right to backward and forward based on DIR
		my ($dir, $p);
		$p = $Fighters[$player];
		$dir = ${$p}{'DIR'};
		$key = (5 - $key) if ($dir<0);
	}
	
	$::Inputs[$player]->KeyDown( $key );
}


sub KeyUp
{
	my ($player, $key) = @_;
	#print "KeyUp ($player, $key)\n";

	if ( ($key==2) or ($key==3) )
	{
		# Map left and right to backward and forward based on DIR
		my ($dir, $p);
		$p = $Fighters[$player];
		$dir = ${$p}{'DIR'};
		$key = (5 - $key) if ($dir<0);
	}
	
	$::Inputs[$player]->KeyUp( $key );
}


return 1;
