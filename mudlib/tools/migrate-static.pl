#!/usr/bin/perl
use strict;
use warnings;

# Migrate MudOS static keyword usage for modern FluffOS (lib-modern only).
# Converts static functions to protected, static variables to nosave.
# Does not handle static+private pairs perfectly; review illegal nosave
# on functions after a bulk run. See driver-upgrade-notes.md.

my @files;
if (@ARGV) {
    @files = @ARGV;
} else {
    @files = split(/\n/, `find /home/thurtea/aethermud/lib-modern \\( -name '*.c' -o -name '*.h' \\) ! -path '*/doc/*'`);
}

for my $file (@files) {
    next unless $file && -f $file;
    next if $file =~ m{/doc/};

    open my $fh, '<', $file or next;
    my @lines = <$fh>;
    close $fh;

    my $changed = 0;
    for my $i (0 .. $#lines) {
        my $line = $lines[$i];
        my $orig = $line;

        $line =~ s/\bnomask\s+static\b/nomask protected/g;
        $line =~ s/\bstatic\s+private\b/private nosave/g;
        $line =~ s/\bprivate\s+static\b/private nosave/g;

        if ($line =~ /\bstatic\b/) {
            if ($line =~ /\([^{[]/ || ($i + 1 < @lines && $lines[$i + 1] =~ /^\s*\w+\s*\(/)) {
                $line =~ s/\bstatic\b/protected/g;
            } elsif ($line =~ /[,;=]/ || ($i + 1 < @lines && $lines[$i + 1] =~ /^\s*;/)) {
                $line =~ s/\bstatic\b/nosave/g;
            } else {
                $line =~ s/\bstatic\b/protected/g;
            }
        }

        if ($line ne $orig) {
            $lines[$i] = $line;
            $changed = 1;
        }
    }

    if ($changed) {
        open $fh, '>', $file or die $!;
        print $fh @lines;
        close $fh;
        print "updated $file\n";
    }
}
