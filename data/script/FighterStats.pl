use strict;
use bytes;

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
We store both official and 3rd-party characters in the %FighterStats hash.
Official characters have ID's between 1 and 99, contributed characters have
ID's >= 100.

No file should access the %FighterStats hash directly, instead,
RegisterFighter must be used to add fighters and
GetFighterStats must be used to retrieve information.
=cut

%::FighterStats = (		

'1'=>
{	'ID'	=> 1,
	'CODENAME' => 'Ulmar',
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

	'TEAM-hu'	=>'Gonosz',
	'STYLE-hu'	=>'Bohóc-fu',
	'WEIGHT-hu'	=>'50kg + vaságy',
	'HEIGHT-hu'	=>'168cm',
	'SHOE-hu'	=>'51',
	'STORY-hu'	=>
'Miután Wasaiwa baka janaijo elszajrézta a kora legfejletebb tudományos cuccosát
(véletlenül úgy behúztak neki, hogy berepült a laboratoriumba és rátekeredett a CsUKlo(tm)),

azután minden tudását latba vetve vissza utazott a múltba (elkezdte nyomkodni a gombokat a
CsUKlo(tm)-en és ez lett belõle).

Ezekután már rögtön tudta, hogy itt el kell pusztitania a Szombatot! (õ sem tudja, hogy hol
is van éppen illetve mit is csinál...)',
	},





'2'=>
{	'ID'	=> 2,
	'CODENAME' => 'UPi',
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

	'NAME-hu'	=>'Sötét Fekete Gonosz Mágus',
	'TEAM-hu'	=>'Gonosz vezér',
	'STYLE-hu'	=>'Piro-fu',
	'AGE-hu'	=>'30',
	'WEIGHT-hu'	=>'70kg',
	'HEIGHT-hu'	=>'180cm',
	'SHOE-hu'	=>'42',
	'STORY-hu'	=>
'A Gonosz Gyilkos Fekete Ellenszenves Sötét (elméjü) tüzvarázslók ligájának
tagja, kit azzal bíztak meg, hogy elpusztítsa a szombatot egyszer, s
mindörökre. Talán van valami hátsó szándéka, amirõl senki sem tud? Nincs!',
},




'3'=>
{	'ID'	=> 3,
	'CODENAME' => 'Zoli',
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

	'NAME-hu'	=>'Boxer',
	'TEAM-hu'	=>'Gonosz',
	'STYLE-hu'	=>'Kickbox-fu',
	'AGE-hu'	=>'16',
	'WEIGHT-hu'	=>'80kg',
	'HEIGHT-hu'	=>'180cm',
	'SHOE-hu'	=>'43',
	'STORY-hu'	=>
'Boxer azért csatlakozott a Mortál Szombat csapathoz, hogy minél több
embernek besomhasson, minél többször, és minél nagyobbat. Más célja ezen
kívül nincs, de ez is remekül elszórakoztatja középtávon',
},




'4'=>
{	'ID'	=> 4,
	'CODENAME' => 'Cumi',
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

	'NAME-hu'  =>'Cumi',
	'TEAM-hu'  =>'Jo vezer',
	'STYLE-hu' =>'N/A',
	'AGE-hu'   =>'15',
	'WEIGHT-hu'=>'55',
	'HEIGHT-hu'=>'170',
	'SHOE-hu'  =>'41.5',
	'STORY-hu' =>
'Elete fo ambicioja volt, hogy autot vezessen. Most hogy ezt teljesitette, masodik fo
amibicioja fele fordult: hogy nagy harcmuvessze valjon. Kezdetben ehhez megnezte
a Kung fu sorozatot elejetol vegeig egyulteben. Kepzettsege jelenleg ebbol all.',
},




'5'=>
{	'ID'	=> 5,
	'CODENAME' => 'Sirpi',
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


	'STYLE-hu'	=> 'Nebánts-FU',
	'STORY-hu'	=>
'Sok évnyi hardcore gamerkedés után, miközben el is hízott jól,
kikapcsolták nála a villanyt.

Erre nagyon hülye pofát vágott, és ez igy is
marad mindaddig, amíg le nem számol õsellenségével (vagy még utána is).
Ezért csatlakozott a jók kicsiny csapatához... Meg amúgy is fél egyedül.',
},





'6'=>
{	'ID'	=> 6,
	'CODENAME' => 'Macy',
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

	'STORY-hu'	=>
'kb. néhány évvel ezelõtt, (vagy talán egy kicsit korábban, esetleg
késõbb) a felhõk között egy pólyában találtak rá (zuhanás közben).

A bátor
Songokutól elleste a harcmûvészet mesteri fortélyait, igy talpra esett és
nem halt meg. Azóta is halhatatlan. Ki tudja, még meddig? Talán a
következõ harcig a gonosz ellen...',
	},




'7'=>
{	'ID'	=> 7,
	'CODENAME' => 'Bence',
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

	'NAME-hu'	=>'Taka Ito',
	'TEAM-hu'	=>'Gonosz',
	'STYLE-hu'	=>'Kururin-do',
	'AGE-hu'	=>'20',
	'WEIGHT-hu' =>'85',
	'HEIGHT-hu' =>'172',
	'SHOE-hu'	=>'39',
	'STORY-hu'	=>
'A japán óriás Descant esküdt ellensége, mióta összejárta az általa frissen felmosott
verandát a 16 generáció óta családja által birtokolt foga-do-ban. Tudását a sötét
oldal szolgálatába állította. Tudását a "gondnok útján" csak mélységes megvetése a
vasaltorrú bakancsok iránt szárnyalja túl.',
	},




'8'=>
{	'ID'	=> 8,
	'CODENAME' => 'Grizli',
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

	'NAME-hu'	=>'Grizli',
	'TEAM-hu'	=>'Jó',
	'STYLE-hu'	=>'Gyakás ala Medve',
	'AGE-hu'	=>'21',
	'WEIGHT-hu'	=>'50000000',
	'HEIGHT-hu'	=>'170',
	'SHOE-hu'	=>'49',
	'STORY-hu'	=>
'Grili a lustaságáról volt hires mindig. Olyannyira, hogy amilyen szinten
azt csinálja, az már mûvészet. Az utóbbi 5 évben már a TV
nézéshez is lusta lett.

Minden Szobaton tart edzést a Különbenmegintdühbejövünk do
stilusból, amit még kezdõ korában a TV-bõl sajátított el. A stilus
tiszteletbeli nagymestere maga Bád Szpencer, de sajnos miután Bád-ról
kiderült, hogy szombatonként dolgozik, Grizli elvette tõle a cimet, s
azóta magának tartogatja.

Grizli a szombat ellenesek ádáz gyûlölõje, a jó csapat oszlopos tagja.',
	},




'9'=>
{	'ID'	=> 9,
	'CODENAME' => 'Descant',
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

	'NAME-hu'	=>'Descant',
	'TEAM-hu'	=>'Jó',
	'STYLE-hu'	=>'+halol',
	'AGE-hu'	=>'58',
	'WEIGHT-hu'=>'89',
	'HEIGHT-hu'=>'180',
	'SHOE-hu'	=>'44',
	'STORY-hu'	=>
'A Vietnámi háború során képezték ki minden ismert fegyverre és harcm~uvészetre. Már ott
is az Oroszok és az Amerikaik oldalán harcolt, már aki éppen többet fizetett. Ezután a pénzb"ol
hibernáltatta magát és csak háborúk esetén olvasztatja föl magát, vagy most mikor a szombat
bajba kerül most is azon az oldalon van, ahol vastagabb a BUKSZA, most épp a...',
},





'10'=>
{	'ID'	=> 10,
	'CODENAME' => 'Surba',
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

	'NAME-hu'	=>'Rising-san',
	'TEAM-hu'	=>'Gonosz',
	'STYLE-hu'	=>'Pöcc-fu',
	'AGE-hu'	=>'500',
	'WEIGHT-hu'	=>'N/A',
	'HEIGHT-hu'	=>'50',
	'SHOE-hu'	=>'Nem visel',
	'STORY-hu'	=>
'Sok-sok évvel ezel"ott eltûnt misztikus körülmények között... egy szombati napon!
És most visszatért. Senki sem tudja honnan jött, de magával hozta pusztító technikáját
melynek nincs párja a földön. Senki sem érti miért fogadta el a gonosz varázsl
megbízását...',
},





'11'=>
{	'ID'	=> 11,
	'CODENAME' => 'Ambrus',
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
	'KEYS' =>
'Down Back LPunch - Axe Toss
Back Forward HKick - Chop Chop
Forward Forward LKick - Bonesaw',

	'NAME-hu'	=>'Fûrészes Õrült',
	'TEAM-hu'	=>'Gonosz',
	'STYLE-hu'	=>'Fanyûvõ',
	'AGE-hu'	=>'35',
	'WEIGHT-hu'	=>'110',
	'HEIGHT-hu'	=>'120',
	'SHOE-hu'	=>'49',
	'STORY-hu'	=>
'Bölcsõjét egy fán találták meg. Késõbb felaprította az egész befogadó családját, és
megetette a medvékkel. Azóta a kanadai erdõkben bolyongva vágja a fákat és az
emberfejeket. Forró nyári éjszakákon mindig hallatszik õrült kacaja.',
},




'12'=>
{	'ID'	=> 12,
	'CODENAME' => 'Dani',
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
	'KEYS' =>
'Down Back LPunch - Hat
Forward Forward HPunch - Ramming Attack
Back Down Back LPunch - Stab
Back Forward LKick - Poke',

	'NAME-hu'	=>'Tökéletlen Katona',
	'TEAM-hu'	=>'Jó',
	'STYLE-hu'	=>'Kocsmabunyó',
	'AGE-hu'	=>'50',
	'WEIGHT-hu'	=>'100',
	'HEIGHT-hu'	=>'180',
	'SHOE-hu'	=>'44',
	'STORY-hu'	=>
'Gyermekkorát meghatározta Drezda lebombázása. E trauma hatására katonai
pályára állt. Immáron 30 éve a Bundeswehr kötelékében tizedes az
elõléptetés bárminem~u esélye nélkül.

Alantasait folytonosan kocsmai bunyóinak történeteivel traktálja, amíg azok
áthelyezésüket nem kérik.',
},





'13'=>
{	'ID'	=> 13,
	'CODENAME' => 'Kinga',
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

	'STORY-hu'=>
'A megprobaltatasok akkor kezdodtek, amikor anyukaja a szive alatt hordta.
Egyetlen koldokzsinoron fuggott az elete! De megszuletett vegul, ezert
kapta az Aisha ("mindent tulelo") nevet. Aztan gyermekkoratol fogva sok
termeszeti katasztrofat, terrortamadast atveszelt, es mar kezdett elege
lenni az egeszbol, igy hat az egyik alkalommal kijelentette, hogy ha ezt
tulelem, csatlakozom azokhoz a hulye Mortalosokhoz!',
},	#'





'14'=>
{	'ID'	=> 14,
	'CODENAME' => 'Misi',
	'NAME'	=>'Papatsuka Mamatsuba',
	'TEAM'	=>'Evil',
	'STYLE'	=>'Gloom',
	'AGE'	=>'Feudal Middle',
	'WEIGHT'=>'Dead',
	'HEIGHT'=>'178 cm',
	'SHOE'	=>'43,12748252',
	'STORY'	=>
'Papastuka has been raised strictly in the way of the samurai since age 4.
His father was the most famous warrior of the past 20 years. After he
learned all the jutsu from dad, he skalped him, and put the skalp on his head
to scare his enemies.

On weekdays he is seen chasing women, saturdays he
drinks a lot. Then he decided, that enjoying saturday should belong to him
alone...',

	'NAME-hu'	=>'Apatsuka Anyatsuba',
	'TEAM-hu'	=>'Gonosz',
	'STYLE-hu'	=>'Komor',
	'AGE-hu'	=>'Feudális közép...',
	'WEIGHT-hu'	=>'Nagyon súlyos!',
	'STORY-hu'	=>
'Apatsukát 4 éves kora óta nevelték szülei a szamuráj életmódra, szigor
keretek között. Apja az elmúlt 20 év leghíresebb harcosa volt. Amint
minden harci fogást elsajátított apjától, megskalpolta és a skalpját
fejére illesztette, ezzel megfélemlítve ellenségeit.

Hétközben nõket
hajszolt, szombaton szívott és berugott. Aztán úgy döntött, hogy ezen a
szombaton csak õ érezheti jól magát...',


},


);



sub RegisterFighter($)
{
	my ($reginfo) = @_;
	
	# reginfo must contain: ID, GENDER, DATAVERSION, DATASIZE, STARTCODE, FRAMES, STATES, CODENAME
	foreach my $attr (qw(ID GENDER DATAVERSION DATASIZE STARTCODE FRAMES STATES CODENAME))
	{
		die "RegisterFighter: Attribute $attr not found" unless defined $reginfo->{$attr};
	}
	# CheckStates( $reginfo->{ID}, $reginfo->{STATES} );
	
	my ($fighterenum, $fighterstats);
	$fighterenum = $reginfo->{ID};
	
	$fighterstats = $::FighterStats{$fighterenum};
	if ( not defined $fighterstats )
	{
		print "RegisterFighter: Fighter $fighterenum not found, non-syndicated?\n";
		$fighterstats = {
			'ID'		=> $fighterenum,
			'NAME'		=>'Unknown (non-syndicated)',
			'TEAM'		=>'Unknown',
			'STYLE'		=>'Unknown',
			'AGE'		=>'Unknown',
			'WEIGHT'	=>'Unknown',
		  	'HEIGHT'	=>'Unknown',
			'SHOE'		=>'Unknown',
			'STORY'		=>'...',
			'DATAFILE'	=> $reginfo->{CODENAME} . '.dat',
			%{$reginfo}
  		};
		$::FighterStats{$fighterenum} = $fighterstats;
		return;
	}
	
	# Add the reginfo to the fighter stats:
	%{$fighterstats} = ( 'DATAFILE' => $reginfo->{CODENAME}.'.dat', %{$fighterstats}, %{$reginfo} );
}



sub GetStatsTranslated($$)
{
	my ($source, $stat) = @_;
	return $source->{"${stat}-$::LanguageCode"} if defined $source->{"${stat}-$::LanguageCode"};
	return $source->{$stat};
}



sub GetFighterStats($)
{
	my ($fighterenum) = @_;
	
	my ($source) = $::FighterStats{$fighterenum};

	$::Codename	= $source->{CODENAME};
	$::Name		= GetStatsTranslated( $source, 'NAME' );
	$::Team		= GetStatsTranslated( $source, 'TEAM' );
	$::Style	= GetStatsTranslated( $source, 'STYLE' );
	$::Age		= GetStatsTranslated( $source, 'AGE' );
	$::Weight	= GetStatsTranslated( $source, 'WEIGHT' );
	$::Height	= GetStatsTranslated( $source, 'HEIGHT' );
	$::Shoe		= GetStatsTranslated( $source, 'SHOE' );
	$::Story	= GetStatsTranslated( $source, 'STORY' );
	$::Keys		= GetStatsTranslated( $source, 'KEYS' );
	$::Datafile	= $source->{'DATAFILE'};
	$::Portrait	= $::Codename . ".icon.png";
	
	$::Story =~ s/([^\n])\n([^\n])/$1 $2/gms;
	
	@::StatTags = ( 'Name: ', 'Team: ', 'Style: ', 'Age: ', 'Weight: ', 'Height: ', 'Shoe size: ' );
	
	# print "The data file of $fighterenum is '$::Datafile'\n";
	
	return $source;
}




#GetFighterStats(1);
#print "$::Name $::Team $::Style $::Age $::Weight $::Height $::Shoe\n$::Story\n";

return 1;
