#!/usr/bin/perl -w

use strict;


=comment

Damage entries are string => amount of damage associations.
The string is a derivation of the state and optionally the
name of the fighter. A full name takes precedence over the
partial name.

E.g. if we are looking for the damage for "Maci HighPunch",
we first try the full name, then only "HighPunch".

The public method is :
GetDamage( fighter, state )


=cut


%::Damage = (
	'JumpKick'			=> 18,
	'JumpPunch'			=> 16,
	'KneelingPunch'		=> 6,
	'KneelingKick'		=> 12,
	'HighPunch'			=> 6,		# 17
	'LowPunch'			=> 6,		# 17
	'HighKick'			=> 15,		# 7
	'LowKick'			=> 15,		# 7
	'GroinKick'			=> 20,		# 5
	'KneeKick'			=> 15,		# 7
	'Elbow'				=> 12,		# 8
	'Spin'				=> 19,		# 6
	'Sweep'				=> 12,		# 8
	'Grenade'			=> 15,		# 7
	'Uppercut'			=> 19,		# 5 + e
	'KneelingUppercut'	=> 19,		# 5 + e
	'Throw'				=> 19,		# 5 + e

	'ScissorKick Maci'	=> 18,		# 5.5
);


sub GetDamage($$)
{
	my ($fighter, $state) = @_;
	my ($state2);
	
	return $::Damage{"$state $fighter"} if defined $::Damage{"$state $fighter"};
	
	$state2 = substr( $state, 0, index( $state, " " ) );
	
	return $::Damage{"$state2 $fighter"} if defined $::Damage{"$state2 $fighter"};
	return $::Damage{"$state2"} if defined $::Damage{"$state2"};
	
	print "Not in database: '$state $fighter' => ,\n";
	return 10;	# Fallback value...
}


return 1;
