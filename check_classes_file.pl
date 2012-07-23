use 5.016;
use warnings;
use diagnostics;

my $codesList = shift or die "Give me the file!";
open my $codes, "<", $codesList or die "$!";
open my $output, ">another_view_of_classes.txt" or die "wtf";
my %class;
while (<$codes>) {
	chomp;
	chomp (my $i = <$codes>);
	print $output ("$_ \n");
	print $output ("               $i \n");
	my $q = $_; #для передачи строки во внутренний цикл
	my @q = (); #для инициализации элемента хеша как массива
	$class{$q} = [@q];
	for(1..$i) {
		chomp(my $qq = <$codes>);
		push $class{$q}, $qq;
		print $output ("$class{$q}[-1] ");
	}
	print $output "\n";	
}

close $codes;
close $output;