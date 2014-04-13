use 5.016;
use warnings;
#use diagnostics;

#my $codesList = shift or die "Give me the file!";
my $codesList = "classes_opcode_only";
my $fileName = shift or die "Give me the file!";
#print $fileName;
open my $in, "<", $fileName or die "$!";
open my $codes, "<", $codesList or die "$!";
open my $output, ">", "instruction_list.txt" or die "wtf";
#Считываем из файла название класса, количество элементов в нем, после чего создаем хеш, 
#где ключ - название класса, а значение - массив элементов этого класса
#В общем случае не рационально считывать все классы, но пока что их не много и используются в шаблоне почти все сразу, поэтому в данном случае раница незаметна
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
	my $imm = (/"(\d+ \/ \d+)"/x ) ?  $1 : "0/0";
	my $modRM = (/modRM/) ?  1 : 0;
	my @byte = split /\|/, $_;	#разбираем строку шаблона
	my @result = ();
	foreach my $bClass (@byte) {	#для каждого элемента в этой строке
		my @temp = ();
		foreach my $b (@{$class{$bClass}}) { #перебираем все элементы соответствующего класса
			if(@result) {
				push @temp, map {
					$_." $b";
				} @result; 
			} else {
				push @temp, $b;
			}
		}
		@result = @temp;
	}
	map {print $output "$_ modRM=$modRM immediate=$imm\n"} @result;
}
close $codes;
close $in;