open $in, "<", "state_table.dat";
my $c = 0;
my @a = <$in>;
map{
	$c++;
	/(\d+)/;
	$line = $1;
	if($line > 407552) {
		print $line;
	}
}@a;
print $a[0x49];

print "count = $c";