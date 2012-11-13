﻿use 5.016;
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
my @prefixStateOperand = (1, 2, 3, 4, 6, 7, 10, 11, 13, 16); #состояния КА префиксов, если в префиксах был operand size префикс
my @prefixStateAddress = (2, 4, 7, 8, 11, 12, 13, 15); #состояния КА префиксов, если в префиксах был address size префикс

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
	$tailTable[$state] = [&getClearState];
	for(0 + 17..17 + 17) {
		$tailTable[$state][$_] = $imm[0];
	}
	foreach(@prefixStateOperand) {
		$tailTable[$state][$_] = $imm[1];
	}
	for(0..16) {
		$tailTable[$state][$_] = $modRM;
	}
	foreach(@prefixStateAddress) {
		$tailTable[$state][$_]++ unless(0 == $modRM);
	}
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
print $outputTail "AvailabilityModrmImm";
foreach(@tailTable) {
	foreach(@$_) {
		print $outputTail " db "."$_"." \n";
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