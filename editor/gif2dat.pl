#!/usr/bin/perl -w

#
# gif2dat.pl
# For OpenMortal http://openmortal.sf.net
# 
# The purpose of this script is converting an animated gif file
# (usually, a character) into a dat and dat.txt files, usable by OpenMortal.
# THIS PROGRAM COMES WITH NO WARRANTY OR SUPPORT OF ANY KIND.
# It is intended to be an internal tool. If it works for you, lovely.
#
# The conversion requires:
#   libungif-progs package (Mandrake naming)
#   allegro (/usr/bin/dat tool required)
#
# The "magic number" is the transparent color in the gif (this will be 
# color 0 in the output file).
#

if (!$ARGV[0])
{
	print "Usage: $0 gifname [datname] [magiccolor]
If datname is not supplied, gif2dat.dat and gif2dat.txt are written.
The default magic color is 63\n";
	die;
}

`rm _GIF* FRAME* PALETTE.PCX`;

$gifname = $ARGV[0];
$datname = $ARGV[1];
$txtname = $datname ? "$datname.txt" : 'gif2dat.txt';
$datname = 'gif2dat.dat' unless $datname;
$trcolor = (defined $ARGV[2]) ? $ARGV[2] : 63;

# Left = 273, Top = 149, Width = 133, Height = 300

# *** Create text information
print "$txtname\n";
$giftext = `giftext $gifname`;
open GIFTEXT, ">$txtname" || die "Couldn't open $giftext!\n";
while ( $giftext=~/Image Size - Left = (\d+), Top = (\d+), Width = (\d+), Height = (\d+)/g ) {
	printf GIFTEXT "{ 'x'=>%3d, 'y'=>%3d, 'w'=>%3d, 'h'=>%3d },\n", $1, $2, $3, $4;
}
close GIFTEXT;

# *** Flip and append the image
# `gifflip -y $gifname > _GIF2DATFLIP.GIF`;
# `gifasm $gifname _GIF2DATFLIP.GIF > _GIF2DATASM.GIF`;

print "$datname\n";


# *** Translate transparent color to 0

print "Translating transparent color $trcolor to 0.\n";

open TRANS, ">_GIF2DATTRANS";
#print TRANS "0 $trcolor\n";
print TRANS "0 $trcolor\n";
for ($i=1; $i<256; $i++)
{
	if ($i == $trcolor) { print TRANS "$i 0\n"; }
	else { print TRANS "$i $i\n"; }
}
close TRANS;

`gifclrmp -t _GIF2DATTRANS $gifname > _GIF2DATTMP.GIF`;

# *** Split the gif up

print "Splitting up the gif.\n";

`gifasm -d _GIF2DATFRAME _GIF2DATTMP.GIF`;
while(<_GIF2DATFRAME*.gif>)
{
	$frame = $_;
	print "Converting $frame to $frame.pcx\n";
	`convert $frame $frame.pcx`;
}

while (<_GIF2DATFRAME*.pcx>)
{
	($i) = /(\d\d+)/;
	$j = sprintf( "%03d", $i );
	# print "FRAME$j.pcx\n";
	`mv $_ FRAME$j.pcx`;
}


`cp FRAME000.pcx COLORS.PCX`;

# *** dat cannot handle >256 files at a time..
# Split it up into chunks, at most 50 files at a time.

print "Saving images to .dat file with dat\n";

@frames = `ls FRAME*`;
chomp @frames;
$i = $j = 0;

while ( $i < scalar @frames )
{
	$command = "dat -v -c0 -a -t RLE $datname";
	for ( $j = $i; $j< $i + 50; ++$j )
	{
		last if $j == scalar @frames;
		$command .= " $frames[$j]";
	}
	$i = $j;
	`$command`;
}
#`dat -v -c0 -a -t RLE $datname FRAME*`;

#`dat -v -c0 -a -t RLE $datname FRAME* >&2`;
`dat -v -c0 -a -t PAL $datname COLORS.PCX >&2`;
`rm _GIF* FRAME* COLORS.PCX`;


