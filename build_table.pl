use 5.016;
use warnings;
use diagnostics;

my $fileName = shift or die "Give me the file!";

open my $in, "<", $fileName or die "$!";
open my $output, ">result_table.txt" or die "wtf";

my $state = 0;
my $maxstate = 0;
push my @table, [&getClearState];

while (<$in>) {
	chomp;
	my @bytes = split " ";
	foreach my $b (@bytes) {
		$b = hex $b;
		if(0 != $table[$state][$b]){
			$state = $table[$state][$b];
		} else {
			$maxstate++;
			push @table, [&getClearState];
			$table[$state][$b] = $maxstate;
			$state = $maxstate;
		}
	}
	#push my @table, [&getClearState];
	#print $output @$_ foreach ($table[0]);
	#print $output "\n";
	#last;
}

foreach(@table) {
	print $output "@$_";
	print $output "\n";
}

close $in;
close $output;
sub getClearState {
	my @res;
	for(0x00..0xff) {
	push @res, 0;
	}
	@res
}