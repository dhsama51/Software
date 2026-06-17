project_open M3_demo
load_package flow
execute_flow -compile
set sof_file [file join output_files M3_demo.sof]
set rbf_file [file join output_files M3_demo.rbf]
execute_module -tool cpf -args "-c $sof_file $rbf_file"
project_close
