use strict;

sub ParseConfig($)
{
	my ($filename) = @_;
	open CONFFILE, "$filename" or return;
	
	while (<CONFFILE>)
	{
		if ($_ =~ /(\w+)\s*=\s*(\w+)/)
		{
			eval( "\$::$1 = '$2';" );
		}
	}
	
	close CONFFILE;
}


return 1;
