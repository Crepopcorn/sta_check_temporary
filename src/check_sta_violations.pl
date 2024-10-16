use strict;
use warnings;
use Parallel::ForkManager;

my $max_processes = 4;
my $pm = Parallel::ForkManager->new($max_processes);

my $sta_report = "../reports/sta_report.txt";
my $violations_file = "../reports/violations.txt";

open my $fh, '<', $sta_report or die "Cannot open STA report file: $sta_report\n";

open my $out, '>', $violations_file or die "Cannot write violations file.\n";

my $violation_count = 0;
my $total_negative_slack = 0;
my $worst_slack = 0;
my $worst_path = '';
my @violations;

while (my $line = <$fh>) {
    chomp($line);
    
    if ($line =~ /Path \d+:/) {
        $pm->start and next;
        my $slack = 0;
        my $path_name = $line;
        
        my $arrival_time_line = <$fh>;
        my $required_time_line = <$fh>;
        my $slack_line = <$fh>;

        if ($slack_line =~ /slack\s*=\s*(-?\d+\.\d+)/) {
            $slack = $1;
            
            if ($slack < 0) {
                $violation_count++;
                $total_negative_slack += $slack;
                push @violations, "Path: $path_name, Slack: $slack";
                
                if ($slack < $worst_slack) {
                    $worst_slack = $slack;
                    $worst_path = $path_name;
                }
            }
        }

        $pm->finish(0);
    }
}

$pm->wait_all_children;

print $out "Total violations: $violation_count\n";
print $out "Total negative slack: $total_negative_slack\n";
print $out "Worst path: $worst_path, Worst slack: $worst_slack\n\n";

if ($violation_count > 0) {
    print $out "Violation details:\n";
    foreach my $violation (@violations) {
        print $out "$violation\n";
    }
}

close $out;
close $fh;
