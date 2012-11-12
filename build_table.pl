use 5.016;
use warnings;
use diagnostics;

#my $fileName = shift or die "Give me the file!";

#open my $in, "<", $fileName or die "$!";
open my $in, "<", "instruction_list.txt" or die "$!";
open my $outputState, ">", "state_table.dat" or die "wtf";
open my $outputTail, ">", "modRM_and_immediate_table.dat" or die "wtf";

my $state = 0;
my $maxstate = 0;
push my @stateTable, [&getClearState];
my @tailTable;

while (<$in>) {
	chomp;
	/modRM=(\d+)/;
	my $modRM = $1;
	my @imm = ();
	push @imm, $1, $2 if(m'immediate=(\d+)\/(\d+)');
	s@modRM.*@@;
	my @bytes = split " ";
	foreach my $b (@bytes) {
		$b = hex $b;
		if(0 != $stateTable[$state][$b]){
			$state = $stateTable[$state][$b];
		} else {
			$maxstate++;
			push @stateTable, [&getClearState];
			$stateTable[$state][$b] = $maxstate;
			$state = $maxstate;
		}
	}
	$tailTable[$state] = [&getClearTail];
	$tailTable[$state][0] = $modRM;
	$tailTable[$state][1] = $imm[0];
	$tailTable[$state][2] = $imm[1];
	$state = 0;
}
print $outputState "opcodeState";
#вложенные foreach, мрак и ужас
	my $max = 0;
foreach(@stateTable) {
	#print $outputState ;
	foreach(@$_) {
		if($_ > $max) {$max = $_;}
		#$_ *= 512;
		print $outputState " dw "."$_"." \n";
		}
		
	#print $outputState "\n";
}
print $max;
foreach(@tailTable) {
	if($_) {
		print $outputTail "@$_";
		print $outputTail "\n";
	}
}

close $in;
close $outputState;
close $outputTail;
sub getClearState {
	my @res;
	for(0x00..0xff) {
	push @res, 0;
	}
	@res
}
sub getClearTail {
	my @res;
	for(0..2) {
	push @res, 0;
	}
	@res
}