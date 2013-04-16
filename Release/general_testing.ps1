#set the path to the main project and to the CPP testing and autoTester
$solution_path = "C:\Users\Xi\Documents\GitHub\BasicProgram\";

#all the other paths are relative to the $solution_path
$cppunit_relative_path = "UnitTesting\";
$integration_relative_path = "IntegrationTesting\";
$autoTester_relative_path = "AutoTester\";
$exe_cppunit = "Release\UnitTesting.exe";
$exe_integration = "Release\IntegrationTesting.exe";
$exe_autoTester = "Release\AutoTester.exe";

#output filenames
$out_cppunit = $solution_path +"cppunit.out";
$out_integration = $solution_path +"integration.out";
$out_autoTester = $solution_path +"autoTester.out";

#command line arguments for each executable
$args_cppunit = @(" ");
$args_integration = @(" ");
#change directory to the $solution_path
$current_path = get-location;
chdir($solution_path);
$p = get-location;
write-output $p;

#tests that will be run
#add/remove items if you want to do only unit testing or autoTester testing
#$all_testing = @("cppunit", "integration", "autoTester");
$auto_testing = @("autoTester")

# uncomment below to run cppunit and integration testing together
$all_testing = @("cppunit", "integration")

foreach ($testing in $all_testing)
{
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}
}
foreach ($testing in $auto_testing)
{
$i = 0;
while($i -le  9)
{

$args_autoTester = @($autoTester_relative_path+"tests\simple0"+$i+".txt "+$autoTester_relative_path+"tests\query0"+$i+".txt "+$autoTester_relative_path+"tests\out0"+$i+".xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple20.txt "+$autoTester_relative_path+"tests\query2"+$i+".txt "+$autoTester_relative_path+"tests\out2"+$i+".xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple20.txt "+$autoTester_relative_path+"tests\query3"+$i+".txt "+$autoTester_relative_path+"tests\out3"+$i+".xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple40.txt "+$autoTester_relative_path+"tests\query4"+$i+".txt "+$autoTester_relative_path+"tests\out4"+$i+".xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$i = $i + 1;
}

$j = 0;
while($j -le  3)
{

$args_autoTester = @($autoTester_relative_path+"tests\simple40.txt "+$autoTester_relative_path+"tests\query5"+$j+".txt "+$autoTester_relative_path+"tests\out5"+$j+".xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$j = $j + 1;
}

$args_autoTester = @($autoTester_relative_path+"tests\simple30.txt "+$autoTester_relative_path+"tests\query54.txt "+$autoTester_relative_path+"tests\out54.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple30.txt "+$autoTester_relative_path+"tests\query55.txt "+$autoTester_relative_path+"tests\out55.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}



$args_autoTester = @($autoTester_relative_path+"tests\simple50.txt "+$autoTester_relative_path+"tests\query56.txt "+$autoTester_relative_path+"tests\out56.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}


$k = 0;
while($k -le  9)
{

$args_autoTester = @($autoTester_relative_path+"tests\simple40.txt "+$autoTester_relative_path+"tests\query6"+$k+".txt "+$autoTester_relative_path+"tests\out6"+$k+".xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$k = $k + 1;
}

$args_autoTester = @($autoTester_relative_path+"tests\simple60.txt "+$autoTester_relative_path+"tests\query70.txt "+$autoTester_relative_path+"tests\out70.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple60.txt "+$autoTester_relative_path+"tests\query71.txt "+$autoTester_relative_path+"tests\out71.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple60.txt "+$autoTester_relative_path+"tests\query72.txt "+$autoTester_relative_path+"tests\out72.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

$args_autoTester = @($autoTester_relative_path+"tests\simple60.txt "+$autoTester_relative_path+"tests\query73.txt "+$autoTester_relative_path+"tests\out73.xml");
$file = get-variable ("out_$testing") -valueOnly;
$exe = get-variable ("exe_$testing") -valueOnly;
$cmd_args = get-variable ("args_$testing") -valueOnly;

foreach ($arg in $cmd_args)
{
$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
$run_arg = $cmd_args+" > "+$file;
write-output "Running: $run_cmd `n";
invoke-expression $run_cmd;
write-output "Output: `n---`n";
get-content $file;

write-output "---------`n";
write-output "Done running $testing`n`n`n";
}

}
#return to the initial path (where the script is)
chdir($current_path);