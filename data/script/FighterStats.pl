use strict;

=comment

FIGHTER STATS ARE:

ID			int		All fighters have an integer ID...
NAME		string	Name of the fighter displayed to the user
GENDER		int		1=male 2=female
VERSION		int		Version of the fighter's code
DATAVERSION	int		Version of the data file
DATAFILE	string	Filename of the .DAT file.
STARTCODE	sub		Executed when Reset() is called on the fighter

TEAM		string
STYLE		string
AGE			string
WEIGHT		string
HEIGHT		string
SHOE		string
STORY		string

=cut



=comment
We store "official" characters in the @FighterStats array. Non-syndicated
characters will have a unique ID and stored in a has.
=cut

@::FighterStats = (		
{},	# Dummy UNKNOWN fighter entry..

{	'ID'	=> 1,
	'NAME'	=>'Watasiwa baka janajo',
	'TEAM'	=>'Evil',
	'STYLE'	=>'Clown-fu',
	'AGE'	=>'15',
	'WEIGHT'=>'50kg',
	'HEIGHT'=>'168cm',
	'SHOE'	=>'51',
	'STORY'	=>
'After Wastasiwa baka janajo took possession of his time\'s most advanced 
technogadget (read: by accident he was punched so hard that he flew fight into
a high-tech lab, and got tangled with the WrISt(tm)), 

he used all his knowledge to travel to the past (read: he started mashing the buttons, and
this is how it ended). 

Then he knew immediately that he had to destroy
Saturday! (Read: He has absolutely no idea where he is, or what he is doing...)',
	'KEYS'	=>
'Back HPunch - Spinning headbutt
Down Back LPunch - WrISt shot
Forward Back Forward LPunch - WrISt mash',
	},

{	'ID'	=> 2,
	'NAME'	=>'Dark Black Evil Mage',
	'TEAM'	=>'Evil leader',
	'STYLE'	=>'Piro-fu',
	'AGE'	=>'30',
	'WEIGHT'=>'70kg',
	'HEIGHT'=>'180cm',
	'SHOE'	=>'42',
	'STORY'	=>
'Member of the Evil Killer Black Antipathic Dim (witted) Fire Mages Leage.
He was sent to destroy Saturday now and forever! Or maybe he has a secret
agenda that noone knows about..? Nah...', 
	'KEYS'	=>
'Back HKick - Spinkick
Forward Forward HKick - Crane kick
Down Back LPunch - Fireball
(also works while crouching)
Back Up HPunch - Burning Hands',
},

{	'ID'	=> 3,
	'NAME'	=>'Boxer',
	'TEAM'	=>'Evil',
	'STYLE'	=>'Kickbox-fu',
	'AGE'	=>'16',
	'WEIGHT'=>'80kg',
	'HEIGHT'=>'180cm',
	'SHOE'	=>'43',
	'STORY'	=>
'Boxer joined the Mortal Szombat team for the sole purpose to punch as
many people as hard as he possibly can. He has no other purpose
whatsoever, but at least this keeps him entertained for the time being.', 
	'KEYS'	=>
'Back HPunch - Spinning punch
Down Back LPunch - Weight toss
Forward Forward HPunch - Leaping punch',
},

{	'ID'	=> 4,
	'NAME'	=>'Cumi',
	'TEAM'	=>'Good Leader',
	'STYLE'	=>'N/A',
	'AGE'	=>'15',
	'WEIGHT'=>'55kg',
	'HEIGHT'=>'170cm',
	'SHOE'	=>'41.5',
	'STORY'	=>
'His life ambition was to drive a car. Now that this was accomplished,
he has turned to his second greatest ambition: to be a great martial
artist superhero. As a start, he has watched the TV series "Kung fu"
from beginning to end, in one session. His current training consists
of but this.',
	'KEYS'	=>
'Down Back LPunch - Finger Shot
Forward Forward HPunch - Spit
Back Down Forward - Baseball',
},

{	'ID'	=> 5,
	'NAME'	=>'Sirpi',
	'TEAM'	=>'Good',
	'STYLE'	=>'Don\'tHurtMe-FU',
	'AGE'	=>'24',
	'WEIGHT'=>'76kg',
	'HEIGHT'=>'170cm',
	'SHOE'	=>'41',
	'STORY'	=>
'After being a "hardcore" gamer for several years and consuming a
great amount of food, his electricity was turned off. This has caused
him to make a very stupid face which lasts till this day, and will
last until he has defeated his archenemy. This is why he resolved to
join the good team... also he is frightened alone.',
	'KEYS'	=>
'Down Forward LPunch - Surprise
Forward Forward HPunch - Applause',
},

{	'ID'	=> 6,
	'NAME'	=>'Macy',
	'TEAM'	=>'Good',
	'STYLE'	=>'Macy-fu',
	'AGE'	=>'17',
	'WEIGHT'=>'41kg',
	'HEIGHT'=>'175cm',
	'SHOE'	=>'37',
	'STORY'	=>
'A few years ago (perhaps a little earlier, or maybe later) she was
found among the clouds in a cradle (falling, of course). She learned
martial art from brave Son Goku, so she landed on her feet and didn\'t
die. She\'s been immortal ever since. Who knows for how long? Maybe
it won\'t be until the next fight agains Evil...',
	'KEYS'	=>
'Down Back LPunch - Toss
Forward Forward HKick - Scissor Kick',
	},

{	'ID'	=> 7,
	'NAME'	=>'Jan Ito',
	'TEAM'	=>'Evil',
	'STYLE'	=>'Kururin-do',
	'AGE'	=>'20',
	'WEIGHT'=>'85kg',
	'HEIGHT'=>'172cm',
	'SHOE'	=>'39',
	'STORY'	=>
'The "Japanese giant" is a sworn enemy of Descant... after he left
muddy boot marks all over the freshly mopped porch of the pub, er,
dojo which has belonged to his ancestors for 16 generations. Since
he has turned to the dark side of the janitor. His knowledge of
the "way of the concierge" matches his deep hatred towards army
boots.',
	'KEYS'	=>
'Down Back LPunch - Soap Throw
Back Fw Back Fw LPunch - Stick Spin
Back Forward HPunc - Pierce',
	},

{	'ID'	=> 8,
	'NAME'	=>'Grizzly',
	'TEAM'	=>'Good',
	'STYLE'	=>'Bear dance',
	'AGE'	=>'21',
	'WEIGHT'=>'Several tons',
	'HEIGHT'=>'170cm',
	'SHOE'	=>'49',
	'STORY'	=>
'Grizzly has been long famous for his laziness. He has made laziness a
form of art. In the past 5 years he has been to lazy to watch TV. Every
Saturday he trains in his own special fighting style, one not unlike
that of Bud Spencer, whom he holds as his honorary master. Though,
since he found out that Bud WORKS on Saturdays, he has revoked his
title, and keeps it for himself. He has joined the Good team to fight
to protect the Saturday.',
	'KEYS'	=>
'Down Back LPunch - Bear Shot
Forward Forward HPunch - Poke
Down Down LKick - Earthquake
Back Forward Back HPunch - Nunchaku',
	},

{	'ID'	=> 9,
	'NAME'	=>'Descant',
	'TEAM'	=>'Good',
	'STYLE'	=>'Murderization',
	'AGE'	=>'58',
	'WEIGHT'=>'89kg',
	'HEIGHT'=>'180cm',
	'SHOE'	=>'44',
	'STORY'	=>
'He was trained in \'Nam in every known weapon and martial art form.
He fought there on the side of the Americans and the Russians...
whoever paid more at the moment. Then he used the money to hybernate
himself until the next great war.. or until the Saturday is in
trouble. He joined the side with the more CASH...',
	'KEYS'	=>
'Down Back LPunch - Aimed Shot
Back Back LPunch - Hip Shot
Forward Down HPunch - Knife Throw
Forward Forward HPunch - Gun Hit',
},

{	'ID'	=> 10,
	'NAME'	=>'Rising-san',
	'TEAM'	=>'Evil',
	'STYLE'	=>'Flick-fu',
	'AGE'	=>'500',
	'WEIGHT'=>'N/A',
	'HEIGHT'=>'50',
	'SHOE'	=>'N/A',
	'STORY'	=>
'Mistically disappeared Aeons ago.. on a Saturday! Now he is back, and
brought back his destructive techique, unmatched on Earth. Noone knows
why he joined the Dark Evil Mage...',
},

{	'ID'	=> 11,
	'NAME'	=>'Mad Sawman',
	'TEAM'	=>'Evil',
	'STYLE'	=>'Sawing',
	'AGE'	=>'35',
	'WEIGHT'=>'110',
	'HEIGHT'=>'120',
	'SHOE'	=>'49',
	'STORY'	=>
'His cradle was found on a tree. Later he chopped up the family that
took him and fed them to the bears. He has been roaming the Canadian
forests, chopping trees and heads alike. On hot summer nights his
maniac laughter echoes far.',
},

{	'ID'	=> 12,
	'NAME'	=>'Imperfect Soldier',
	'TEAM'	=>'Good',
	'STYLE'	=>'Pub Fight',
	'AGE'	=>'50',
	'WEIGHT'=>'100',
	'HEIGHT'=>'180',
	'SHOE'	=>'44',
	'STORY'	=>
'His childhood was determined by Drezda\'s bombing. This trauma has
caused him to join the army. For the last 30 years he is corporal
without the slightest hope for advancement. He annoys his
subordinates with a constant flow of stories of pub fights, until
they ask for relocation.',
},

{	'ID'	=> 13,
	'NAME'	=>'Aisha',
	'TEAM'	=>'Good',
	'STYLE'	=>'Death Dance',
	'AGE'	=>'21',
	'WEIGHT'=>'43.5',
	'HEIGHT'=>'155',
	'SHOE'	=>'35',
	'STORY'	=>
'Her trials started right in the womb.. her life hung on a single
umbilical cord! But she was finally born, and got the name
Aisha ("survives everything"). Since her childhood she survived
natural disasters and terrorist attacks, and got frankly fed up.
So one time she said:

"If I survive this, I swear, I\'ll join those stupid Mortal guys!"',
	'STORYHU'=>
'A megprobaltatasok akkor kezdodtek, amikor anyukaja a szive alatt hordta.
Egyetlen koldokzsinoron fuggott az elete! De megszuletett vegul, ezert
kapta az Aisha ("mindent tulelo") nevet. Aztan gyermekkoratol fogva sok
termeszeti katasztrofat, terrortamadast atveszelt, es mar kezdett elege
le\
nni az egeszbol, igy hat az egyik alkalommal kijelentette, hogy ha ezt
tulelem, csatlakozom azokhoz a hulye Mortalosokhoz!',
},

{	'ID'	=> 14,
	'NAME'	=>'Misi (?)',
	'TEAM'	=>'Evil',
	'STYLE'	=>'',
	'AGE'	=>'',
	'WEIGHT'=>'',
	'HEIGHT'=>'',
	'SHOE'	=>'',
	'STORY'	=>'...',
},

);



sub RegisterFighter($)
{
	my ($reginfo) = @_;
	
	# reginfo must contain: ID, GENDER, DATAVERSION, DATASIZE, STARTCODE, FRAMES, STATES, DATAFILE
	foreach my $attr (qw(ID GENDER DATAVERSION DATASIZE STARTCODE FRAMES STATES DATAFILE))
	{
		die "RegisterFighter: Attribute $attr not found" unless defined $reginfo->{$attr};
	}
	
	my ($fighterenum, $fighterstats);
	$fighterenum = $reginfo->{ID};
	$fighterstats = GetFighterStats($fighterenum);
	die "RegisterFighter: Unable to locate fighter $fighterenum\n" unless defined $fighterstats;
	
	# Add the reginfo to the fighter stats:
	%{$fighterstats} = ( %{$fighterstats}, %{$reginfo} );
	
#	$::FighterStat
}



sub GetFighterStats($)
{
	my ($fighterenum) = @_;
	
	my ($source) = $::FighterStats[$fighterenum];
	
	$::Name		= $source->{NAME};
	$::Team		= $source->{TEAM};
	$::Style	= $source->{STYLE};
	$::Age		= $source->{AGE};
	$::Weight	= $source->{WEIGHT};
	$::Height	= $source->{HEIGHT};
	$::Shoe		= $source->{SHOE};
	$::Story	= $source->{STORY};
	$::Keys		= $source->{KEYS};
	$::Datafile	= $source->{DATAFILE};
	
	$::Story =~ s/([^\n])\n([^\n])/$1 $2/gms;
	
	@::StatTags = ( 'Name: ', 'Team: ', 'Style: ', 'Age: ', 'Weight: ', 'Height: ', 'Shoe size: ' );
	#@::StatTags	= ( "Név: ", "Csapat: ", "Stílus: ", "Kor: ", "Súly: ", "Magasság: ", "Cipõméret: " );
	
	return $source;
}




#GetFighterStats(1);
#print "$::Name $::Team $::Style $::Age $::Weight $::Height $::Shoe\n$::Story\n";

return 1;
