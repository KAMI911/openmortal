use strict;



# store: Convert a value into printable format for saving

sub store {
	my $x = shift;
	my ($t, $i, $key, $value);

	if ( not ref($x) ) {			# $x is scalar
		$t = "'$x'";
	}

	elsif (ref($x) eq "SCALAR"){	# $x is a scalar reference
		$t = "\\'$x'";
	}

	elsif (ref($x) eq "REF"){		# $x is ref of a reference
		$t = "\\";					# This could be wrong..
		$t .= store $$x;
		print("save: Storing ref of ref\n");
	}

	elsif (ref($x) eq "ARRAY"){		# $x is array ref. -> [...]
		$t = '[ ';
		foreach $i (@$x) {
			$t .= store( $i );
			$t .= ', ';
		}
		$t .= ' ]';
	}

	elsif (ref($x) eq "HASH"){		# $x is hash ref. -> {...}
		$t .= '{ ';
		while (($key, $value) = each %$x ) {
			$t .= "\t'$key' => ";
			$t .= store( $value );
			$t .= ", \n";
		}
		$t .= ' }';
	}

#	else {							# $x should be a Thing ref
#		foreach $key (@names) {
#			if ( $Thing::things{$key} eq $x )		# Found it!
#			{
#				$t = '$::'.$key;
#				return $t;
#			}
#		}
#		main::Mydebug "Thing::save: ERROR\n";
#		$t = "'ERROR'";
#	}

	return $t;
}

# save: Saves the game. Uses the %things array.
=comment
sub save {
	local ($fhand) = @_;
	my ( $i, $item, $name, $key, $value );
	@names = keys %things;			# Just to make sure..

	print $fhand "# Game saved by Thing::save\n\n";

	while( ($name, $item) = each %things ) {
		print $fhand "new ". ref($item)." '$name';\n";
	}

	print $fhand "\n";

	while( ($name, $item) = each %things ) {
		print $fhand "\%\$$name = ( \%\$$name,\n";
		while (($key, $value)=each %$item) {
			print $fhand "$key => ";
			print $fhand store( $value );
			print $fhand ",\n";
		}
		print $fhand ");\n\n";
	}
}
=cut


sub QuickSave($$$)
{
	my ($prefix, $frameArray, $states) = @_;

	open FILE, ">X$prefix.pl";
	
	print FILE '$f = ';
	print FILE store( $frameArray );
	print FILE ";\n\n@", , $prefix, 'Frames = @{$f}', ";\n\n";

	print FILE '$f = ';
	print FILE store( $states );
	print FILE ";\n\n%", , $prefix, 'States = %{$f}', ";\n\n";
	
	close FILE;
}

return 1;
