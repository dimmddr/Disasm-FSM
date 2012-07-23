use 5.016;
use warnings;
#use diagnostics;

#my $codesList = shift or die "Give me the file!";
my $codesList = "classes";
my $fileName = shift or die "Give me the file!";
#print $fileName;
open my $in, "<", $fileName or die "$!";
open my $codes, "<", $codesList or die "$!";
open my $output, ">result.txt" or die "wtf";
#может не стоит читать все?
my %class;
while (<$codes>) {
	chomp;
	chomp (my $i = <$codes>);
	my $q = $_; #для передачи строки во внутренний цикл
	my @q = (); #для инициализации элемента хеша как массива
	$class{$q} = [@q];
	for(1..$i) {
		chomp(my $qq = <$codes>); #иначе все оставшиеся строки файла добавит за раз в один массив
		push $class{$q}, $qq;
	}
}
#foreach(keys %class) {	say $_;	print "@{$class{$_}}\n";}
foreach (<$in>) {
	chomp;
	my @byte = split /\|/, $_;
	my @result = ();
	foreach my $bClass (@byte) {
		#say $b;
		#print "@{$class{$b}}\n";
		my @temp = ();
		foreach my $b (@{$class{$bClass}}) {
			if(@result) {
				push @temp, map {
					$_." $b";
				} @result;
			} else {
				push @temp, $b;
			}
		}
		@result = @temp;
		#say "qwer";
	}
	map {print $output "$_\n"} @result;
}
close $codes;
close $in;