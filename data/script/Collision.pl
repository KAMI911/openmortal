#!/usr/bin/perl -w

use strict;


sub ccw # (three points) return -1, 0, or 1
{
    my(@points) = @_;
    my($rv) = 0;
    my($dx1, $dx2, $dy1, $dy2, $p0x, $p0y, $p1x, $p1y, $p2x, $p2y);

    #($p0x, $p0y) = split(",", $points[0]);
    #($p1x, $p1y) = split(",", $points[1]);
    #($p2x, $p2y) = split(",", $points[2]);
    ($p0x, $p0y, $p1x, $p1y, $p2x, $p2y) = @points;

    $dx1 = $p1x - $p0x;
    $dy1 = $p1y - $p0y;
    $dx2 = $p2x - $p0x;
    $dy2 = $p2y - $p0y;

    switch:
    {
        $rv =  1, last if $dx1 * $dy2 > $dy1 * $dx2;
        $rv = -1, last if $dx1 * $dy2 < $dy1 * $dx2;
        $rv = -1, last if ($dx1 * $dx2 < 0.0) || ($dy1 * $dy2 < 0.0);
        $rv =  1, last if ($dx1 * $dx1 + $dy1 * $dy1) < ($dx2 * $dx2 + $dy2 * $dy2);
    }

    $rv;
}




sub intersect
{
	my (@lines) = @_;
	# @lines is l1_b, l1_e, l2_b, l2_e

	&ccw( $lines[0], $lines[1], $lines[2], $lines[3], $lines[4], $lines[5] ) *
	&ccw( $lines[0], $lines[1], $lines[2], $lines[3], $lines[6], $lines[7] ) <=0 and
	&ccw( $lines[4], $lines[5], $lines[6], $lines[7], $lines[0], $lines[1] ) *
	&ccw( $lines[4], $lines[5], $lines[6], $lines[7], $lines[2], $lines[3] ) <=0;
}



sub inside
{
	my ($px, $py, $poly) = @_;
	my ($n) = scalar @{$poly};		# # of coordinates
	my ($intersections, $i, $j, $x1, $y1, $x2, $y2, $dx, $dy, $m);
	
	$intersections = 0;
	$py += 0.5;
	$px += 0.5;
	
	for($i = 0; $i < $n; $i+=2)
	{
		$j = $i+2;					# $i is the index of the startpoint, $j is the endpoint
		$j = 0 if $i>=($n-2);
		$x1 = $poly->[$i  ] - $px;
		$y1 = $poly->[$i+1] - $py;
		$x2 = $poly->[$j  ] - $px;
		$y2 = $poly->[$j+1] - $py;

		# Line is completely to the right of the Y axis
		next if( ($x1>0) && ($x2>0) );

		# Line doesn't intersect the X axis at all
		next if( (($y1<=>0)==($y2<=>0)) && (($y1!=0)&&($y2!=0)) );

		# Special case, if the Y on the bottom=0 ignore this
		# intersection (otherwise line endpt counts as 2 hits, not 1)
		if ($y2>$y1) {
			next if $y2==0;
		} elsif ($y1>$y2) {
			next if $y1==0;
		} else {
		# Horizontal span overlaying the X axis. Consider it an
		# intersection iff it extends to the left side of X axis
			$intersections++ if ( ($x1 < 0) || ($x2 < 0) ) and ($y1==0);
			next;
		}

		# We know line must intersect the X axis, so see where
		$dx = $x2 - $x1;

		# Special case.. if a vertical line, it intersects
		unless ($dx){
			$intersections++ if ( $y1 < 0 ) and ( $y2 > 0 );
			$intersections++ if ( $y2 < 0 ) and ( $y1 > 0 );
			next;
		}

		$dy = $y2 - $y1;
		$m = $dy / $dx;
		$b = $y2 - $m * $x2;
		next if ( ( (0 - $b) / $m ) > 0 );

		$intersections++;
	}

	# If there are an odd # of intersections to the left of the origin
	# (the clicked-on point) then it is within the polygon

	#print "inside ($px, $py), (", join(',',@{$poly}), ") = $intersections\n";

	return $intersections % 2;
}

	

sub Collisions
{
	my ($polyref1, $polyref2) = @_;
	
	my ($len1, $len2, $i, $j);
	$len1 = scalar( @{$polyref1} ) ;
	$len2 = scalar( @{$polyref2} ) ;

	# print "Collisions: (", join(',',@{$polyref1}),") <=> (", join(',',@{$polyref2}), ")\n";
	
	# Check inside
	return 1 if inside( $polyref1->[0], $polyref1->[1], $polyref2 );
	return 1 if inside( $polyref2->[0], $polyref2->[1], $polyref1 );
	
	# Check intersect
	
	for ( $i=0; $i<$len1; $i+=2 )
	{
		for ( $j=0; $j<$len2; $j+=2 )
		{
			return 1 if intersect(
				$polyref1->[$i], $polyref1->[$i+1], $polyref1->[($i+2) % $len1], $polyref1->[($i+3) % $len1],
				$polyref2->[$j], $polyref2->[$j+1], $polyref2->[($j+2) % $len2], $polyref2->[($j+3) % $len2] );
		}
	}

	return 0;
}

=comment
require DynaLoader;
use MszCollision;

sub Collisions
{
	my ($polyref1, $polyref2) = @_;
	return MszCollision::CheckCollision($polyref1, $polyref2);
}
=cut


return 1;

=comment

My 400MHz PII runs
3000 Collisions in 7.16s
This is ~ 419 Collision per second.
Borderline usefulness.


The testcode is:

my ($poly1, $poly2, $poly3);

$poly1 = [356,188, 378,188, 378,232, 356,233,];
$poly2 = [333,233, 388,233, 388,360, 333,360,];
$poly3 = [333,350, 338,350, 383,350, 388,350,411,478,323,478,];


for ( my $i=0; $i<1000; ++$i )
{
	Collisions( $poly1, $poly2 );
	Collisions( $poly1, $poly3 );
	Collisions( $poly2, $poly3 );
}


print "Collisions( 1, 2): ", Collisions( $poly1, $poly2 ), "\n";
print "Collisions( 1, 3): ", Collisions( $poly1, $poly3 ), "\n";
print "Collisions( 2, 3): ", Collisions( $poly2, $poly3 ), "\n";

=cut
