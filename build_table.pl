use 5.016;
use warnings;
use diagnostics;

my $codesList = shift or die "Give me the file!";
my $fileName = shift or die "Give me the file!";
#print $fileName;
open my $in, "<", $fileName or die "$!";
open my $codes, "<", $codesList or die "$!";
open my $output, ">result" or die "wtf";
#может не стоит читать все?
my @a = <$codes>;
foreach (<$in>) {
	chomp;
	
}
close $codes;
close $in;