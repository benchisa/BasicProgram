#set the path to the main project and to the CPP testing and autoTester
$solution_path = "C:\Users\Xi\Documents\NUS\Yr2Sem1\CS3201\cs3201g01\";

#all the other paths are relative to the $solution_path
$cppunit_relative_path = "UnitTesting\";
$integration_relative_path = "IntegrationTesting\";
$autoTester_relative_path = "AutoTester\";
$exe_cppunit = "Debug\UnitTesting.exe";
$exe_integration = "Debug\IntegrationTesting.exe";
$exe_autoTester = "Debug\AutoTester.exe";

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
while($i -le 6)
{

$args_autoTester = @($autoTester_relative_path + "tests\simple0"+ $i +".txt " + $autoTester_relative_path + "tests\query0"+$i+".txt " + $autoTester_relative_path + "tests\out0"+$i+".xml");# $autoTester_relative_path+"tests\simple01.txt "+$autoTester_relative_path+"tests\query01.txt "+$autoTester_relative_path+"tests\out01.xml");
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

}
#return to the initial path (where the script is)
chdir($current_path);