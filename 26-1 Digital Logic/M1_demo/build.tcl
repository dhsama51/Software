project_open rbf_demo
execute_flow -compile
load_package flow
set sof_file [file join output_files rbf_demo.sof]
set rbf_file [file join output_files rbf_demo.rbf]
execute_module -tool cpf -args "-c $sof_file $rbf_file"
project_close
