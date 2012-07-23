use 5.016;
use warnings;
use diagnostics;

my $codesList = shift or die "Give me the file!";
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

foreach (<$in>) {
	chomp;
	my @byte = split /\|/, $_;
	#print "@byte";
	foreach (@byte) {
		
	}
}
close $codes;
close $in;