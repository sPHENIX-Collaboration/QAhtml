#! /usr/bin/perl

use warnings;
use strict;
use Getopt::Long;
use File::Basename;

my $notest;
GetOptions("notest"=>\$notest);

my $intopdir = sprintf("/sphenix/data/data02/sphnxpro/calohist");
my $aggdir = sprintf("/sphenix/data/data02/sphnxpro/QAhtml/aggregated");
my %runfiles = ();
open(F,"find $intopdir -type f | sort |");
    while (my $file = <F>)
{
    chomp $file;
    my $lfn = basename($file);
    if ($file =~ /(\S+)-(\d+)-(\d+).*\..*/ )
    {
	my $runnumber = int($2);
	push@{$runfiles{$runnumber}->{"files"}},$file;
    }
}
close (F);
foreach my $run (sort keys %runfiles)
{
    my $cmd = sprintf("hadd -ff %s/HIST_CALO_run2pp_new_2024p001-%08d-9000.root ",$aggdir,$run);
    my $array_ref = $runfiles{$run}->{"files"};
    foreach my $file (sort @$array_ref)
    {
	$cmd = sprintf("%s %s",$cmd,$file);
    }
    if (defined $notest)
    {
	print "executing $cmd\n";
	system($cmd);
    }
    else
    {
	print "would execute $cmd\n";
    }
}
